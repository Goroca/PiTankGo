#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "piTankGoLib.h"
#include <string.h>
#include <wiringPi.h>
#include <stdlib.h>
#include "kbhit.h"
#include <softTone.h>
#include <pthread.h>
#include "fsm.h"
#include "tmr.h"

#define PIN 1;
//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

//------------------------------------------------------
//void InicializaEfecto (TipoEfecto *p_efecto)
//
// Descripcion: inicializa los parametros del objeto efecto.
// Ejemplo de uso:
//
// ...
//
// TipoEfecto efecto_demo;
//
// if ( InicializaEfecto (&efecto_demo, "DISPARO", frecuenciasDisparo, tiemposDisparo, 16) < 0 ) {
// 		printf("\n[ERROR!!!][InicializaEfecto]\n");
// 		fflush(stdout);
// 		}
//------------------------------------------------------
int InicializaEfecto(TipoEfecto *p_efecto, char *nombre, int *array_frecuencias,
		int *array_duraciones, int num_notas) {
	strcpy(p_efecto->nombre, nombre);

	p_efecto->num_notas = num_notas;
	int i;
	for (i = 0; i < num_notas; i++) {
		p_efecto->frecuencias[i] = array_frecuencias[i];
	}
	int j;
	for (j = 0; j < num_notas; j++) {
		p_efecto->duraciones[j] = array_duraciones[j];
	}
	return p_efecto->num_notas;
}

// Procedimiento de inicializacion del objeto especifico
// Nota: parte inicialización común a InicializaPlayDisparo y InicializaPlayImpacto
void InicializaPlayer(TipoPlayer *p_player) {

	p_player->posicion_nota_actual = 0;
	p_player->frecuencia_nota_actual =
			p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];
	p_player->duracion_nota_actual =
			p_player->p_efecto->duraciones[p_player->posicion_nota_actual];
}

//---------------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//---------------------------------------------------------------

int CompruebaStartDisparo(fsm_t* this) {

	piLock(PLAYER_FLAGS_KEY);
	int result = 0;
	result = (flags_player & FLAG_START_DISPARO);
	piUnlock(PLAYER_FLAGS_KEY);
	return result;
}

int CompruebaStartImpacto(fsm_t* this) {
	int result = 0;
	piLock(PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_START_IMPACTO);
	piUnlock(PLAYER_FLAGS_KEY);
	return result;
}

int CompruebaNuevaNota(fsm_t* this) {
	TipoPlayer* p_player = (TipoPlayer*) this->user_data;
	int result = 0;

	if ((p_player->posicion_nota_actual + 1) <= p_player->p_efecto->num_notas) {
		piLock(PLAYER_FLAGS_KEY);
		result = flags_player & FLAG_PLAYER_END;
		piUnlock(PLAYER_FLAGS_KEY);
	}
	return !result;
}

int CompruebaNotaTimeout(fsm_t* this) {
	int result = 0;
	piLock(PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_NOTA_TIMEOUT);
	piUnlock(PLAYER_FLAGS_KEY);
	return result;

}

int CompruebaFinalEfecto(fsm_t* this) {

	int result = 0;
	TipoPlayer* p_player = (TipoPlayer*) this->user_data;
	if (p_player->posicion_nota_actual + 1 > p_player->p_efecto->num_notas){
		piLock(PLAYER_FLAGS_KEY);
		result = (flags_player & FLAG_PLAYER_END);
		piUnlock(PLAYER_FLAGS_KEY);
	}
	return result;

}

//-----------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//-----------------------------------------------------------

void InicializaPlayDisparo(fsm_t* this) {

	TipoPlayer* p_player = (TipoPlayer*) this->user_data;
	piLock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_DISPARO;
	piUnlock(PLAYER_FLAGS_KEY);
	p_player->p_efecto = &p_player->efecto_disparo;

	InicializaPlayer(p_player);

	ComienzaNuevaNota(this);
}

void InicializaPlayImpacto(fsm_t* this) {

	piLock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_IMPACTO;
	piUnlock(PLAYER_FLAGS_KEY);

	InicializaPlayer(this->user_data);
	TipoPlayer* p_player = (TipoPlayer*) this->user_data;
	InicializaPlayer(p_player);
	ComienzaNuevaNota(this);
}

void ComienzaNuevaNota(fsm_t* this) {

	TipoPlayer* p_player = (TipoPlayer*) this->user_data;
	piLock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_PLAYER_END;
	piUnlock(PLAYER_FLAGS_KEY);

	softToneCreate(PLAYER_PWM_PIN);
	softToneWrite(PLAYER_PWM_PIN, p_player->frecuencia_nota_actual);

	tmr_startms(p_player->tmr, p_player->duracion_nota_actual);

	piLock(STD_IO_BUFFER_KEY);
	printf("[PLAYER][InicializaPlayer][NOTA %d][FREC %d][DURA %d]\n",
			p_player->posicion_nota_actual + 1,
			p_player->frecuencia_nota_actual, p_player->duracion_nota_actual);
	piUnlock(STD_IO_BUFFER_KEY);

}

void ActualizaPlayer(fsm_t* this) {

	TipoPlayer* p_player = (TipoPlayer*) this->user_data;
	p_player->posicion_nota_actual++;
	p_player->frecuencia_nota_actual =
			p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];
	p_player->duracion_nota_actual =
			p_player->p_efecto->duraciones[p_player->posicion_nota_actual];

	piLock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_NOTA_TIMEOUT;
	piUnlock(PLAYER_FLAGS_KEY);

	if (p_player->posicion_nota_actual + 1 > p_player->p_efecto->num_notas){
		piLock(PLAYER_FLAGS_KEY);
		flags_player |= FLAG_PLAYER_END;
		piUnlock(PLAYER_FLAGS_KEY);

	}
	piLock(STD_IO_BUFFER_KEY);
	if (p_player->posicion_nota_actual + 1 <= p_player->p_efecto->num_notas)
		printf("[PLAYER][ActualizaPlayer][Nueva Nota (%d DE %d)]\n",
				p_player->posicion_nota_actual + 1,
				p_player->p_efecto->num_notas);
	piUnlock(STD_IO_BUFFER_KEY);
}

void FinalEfecto(fsm_t* this) {
	piLock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_PLAYER_END;
	piUnlock(PLAYER_FLAGS_KEY);

	softToneCreate(PLAYER_PWM_PIN);
	softToneWrite(PLAYER_PWM_PIN, 0);

	piLock(STD_IO_BUFFER_KEY);
	printf("FINAL MELOD�A\n");
	piUnlock(STD_IO_BUFFER_KEY);
}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_player_duracion_nota_actual_isr(union sigval value) {
	piLock(PLAYER_FLAGS_KEY);
	flags_player |= FLAG_NOTA_TIMEOUT;
	piUnlock(PLAYER_FLAGS_KEY);
}

