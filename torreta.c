
#include "torreta.h"
#include "piTankGo_1.h"
#include "piTankGoLib.h"
#include "piTankGo_1.h"
#include <time.h>
#include "tmr.h"
#include <wiringPi.h>
#include <wiringSerial.h>


//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTorreta (TipoTorreta *p_torreta) {
	//HORIZONTAL
	p_torreta->marcador=0;
	p_torreta->servo_x.incremento = SERVO_INCREMENTO;
	p_torreta->servo_x.minimo 	= SERVO_MINIMO;
	p_torreta->servo_x.maximo 	= SERVO_MAXIMO;

	p_torreta->servo_x.inicio 	= SERVO_MINIMO + (SERVO_MAXIMO - SERVO_MINIMO)/2;
	p_torreta->servo_x.posicion 	= p_torreta->servo_x.inicio;

	if(p_torreta->servo_x.posicion > p_torreta->servo_x.maximo)
		p_torreta->servo_x.posicion = p_torreta->servo_x.maximo;

	if(p_torreta->servo_x.posicion < p_torreta->servo_x.minimo)
		p_torreta->servo_x.posicion = p_torreta->servo_x.minimo;

	softPwmCreate (SERVO_HORIZONTAL_PIN, p_torreta->servo_x.inicio, SERVO_PWM_RANGE); // Internamente ya hace: piHiPri (90) ;
	softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->servo_x.posicion);

	//VERTICAL
	p_torreta->servo_y.incremento = SERVO_INCREMENTO;
	p_torreta->servo_y.minimo 	= SERVO_MINIMO;
	p_torreta->servo_y.maximo 	= SERVO_MAXIMO;

	p_torreta->servo_y.inicio 	= SERVO_MINIMO + (SERVO_MAXIMO - SERVO_MINIMO)/2;
	p_torreta->servo_y.posicion 	= p_torreta->servo_y.inicio;

	if(p_torreta->servo_y.posicion > p_torreta->servo_y.maximo)
		p_torreta->servo_y.posicion = p_torreta->servo_y.maximo;

	if(p_torreta->servo_y.posicion < p_torreta->servo_y.minimo)
		p_torreta->servo_y.posicion = p_torreta->servo_y.minimo;

	softPwmCreate (SERVO_VERTICAL_PIN, p_torreta->servo_y.inicio, SERVO_PWM_RANGE); // Internamente ya hace: piHiPri (90) ;
	softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->servo_y.posicion);

	//DISPARO
	pinMode(IR_TX_PIN, OUTPUT);
	digitalWrite(IR_TX_PIN, LOW);

	pinMode(IR_RX_PIN, INPUT);
	pullUpDnControl(IR_RX_PIN, PUD_DOWN);
	wiringPiISR(IR_RX_PIN, INT_EDGE_RISING, impacto);

	piLock(TORRETA_FLAGS_KEY);
	flags_torreta |= FLAG_SYSTEM_START;
	piUnlock(TORRETA_FLAGS_KEY);


}

//---------------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//---------------------------------------------------------------

int CompruebaComienzo (fsm_t* this) {
	int result = 0;
	piLock(TORRETA_FLAGS_KEY);
	result = (flags_torreta & FLAG_SYSTEM_START);
	piUnlock(TORRETA_FLAGS_KEY);
	return result;
}

int CompruebaJoystickUp (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAGS_KEY);
	result = (flags_torreta & FLAG_JOYSTICK_UP);
	piUnlock(TORRETA_FLAGS_KEY);

	return result;
}

int CompruebaJoystickDown (fsm_t* fsm_player){
	int result = 0;

	piLock(TORRETA_FLAGS_KEY);
	result = (flags_torreta & FLAG_JOYSTICK_DOWN);
	piUnlock(TORRETA_FLAGS_KEY);

	return result;
}

int CompruebaJoystickLeft (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAGS_KEY);
	result = (flags_torreta & FLAG_JOYSTICK_LEFT);
	piUnlock(TORRETA_FLAGS_KEY);

	return result;
}

int CompruebaJoystickRight (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAGS_KEY);
	result = (flags_torreta & FLAG_JOYSTICK_RIGHT);
	piUnlock(TORRETA_FLAGS_KEY);
	return result;
}

int CompruebaTimeoutDisparo (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAGS_KEY);
	result = (flags_torreta & FLAG_SHOOT_TIMEOUT);
	piUnlock(TORRETA_FLAGS_KEY);

	return result;
}

int CompruebaImpacto (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAGS_KEY);
	result = (flags_torreta & FLAG_TARGET_DONE);
	piUnlock(TORRETA_FLAGS_KEY);

	return result;
}

int CompruebaTriggerButton (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAGS_KEY);
	result = (flags_torreta & FLAG_TRIGGER_BUTTON);
	piUnlock(TORRETA_FLAGS_KEY);

	return result;
}

int CompruebaFinalJuego (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAGS_KEY);
	result = (flags_torreta & FLAG_SYSTEM_END);
	piUnlock(TORRETA_FLAGS_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ComienzaSistema (fsm_t* this) {
	piLock(TORRETA_FLAGS_KEY);
	flags_torreta &= ~FLAG_SYSTEM_START;
	piUnlock(TORRETA_FLAGS_KEY);
	printf("Torreta Inicializada!\n");
	serialPrintf  (fd, "TORRETA INICIADA!");

	fflush(stdout);
}

void MueveTorretaArriba (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);
	piLock(TORRETA_FLAGS_KEY);
	flags_torreta &= ~FLAG_JOYSTICK_UP;
	piUnlock(TORRETA_FLAGS_KEY);

	if(p_torreta->servo_y.posicion + p_torreta->servo_y.incremento <= p_torreta->servo_y.maximo) {
		p_torreta->servo_y.posicion = p_torreta->servo_y.posicion + p_torreta->servo_y.incremento;

		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->servo_y.posicion);
		serialPrintf  (fd, "ARRIBA");
		delay(100);
		printf("[SERVO VERTICAL][POSICION]=[%d]\n", p_torreta->servo_y.posicion);
		fflush(stdout);
	}
}

void MueveTorretaAbajo (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);
	piLock(TORRETA_FLAGS_KEY);
	flags_torreta &= ~FLAG_JOYSTICK_DOWN;
	piUnlock(TORRETA_FLAGS_KEY);

	if(p_torreta->servo_y.posicion + p_torreta->servo_y.incremento >= p_torreta->servo_y.minimo) {
		p_torreta->servo_y.posicion = p_torreta->servo_y.posicion - p_torreta->servo_y.incremento;

		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->servo_y.posicion);
		serialPrintf  (fd, "ABAJO");
		delay(100);
		printf("[SERVO VERTICAL][POSICION]=[%d]\n", p_torreta->servo_y.posicion);
		fflush(stdout);
	}
}

void MueveTorretaIzquierda (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);
	piLock(TORRETA_FLAGS_KEY);
	flags_torreta &= ~FLAG_JOYSTICK_LEFT;
	piUnlock(TORRETA_FLAGS_KEY);

	if(p_torreta->servo_x.posicion + p_torreta->servo_x.incremento >= p_torreta->servo_x.minimo) {
		p_torreta->servo_x.posicion = p_torreta->servo_x.posicion - p_torreta->servo_x.incremento;

		softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->servo_x.posicion);
		serialPrintf  (fd, "IZQUIERDA");
		delay(100);
		printf("[SERVO HORIZONTAL][POSICION]=[%d]\n", p_torreta->servo_x.posicion);
		fflush(stdout);
	}
}

void MueveTorretaDerecha (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(TORRETA_FLAGS_KEY);
	flags_torreta &= ~FLAG_JOYSTICK_RIGHT;
	piUnlock(TORRETA_FLAGS_KEY);

	if(p_torreta->servo_x.posicion + p_torreta->servo_x.incremento <= p_torreta->servo_x.maximo) {
		p_torreta->servo_x.posicion = p_torreta->servo_x.posicion + p_torreta->servo_x.incremento;

		softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->servo_x.posicion);
		serialPrintf  (fd, "DERECHA");
		delay(100);
		printf("[SERVO HORIZONTAL][POSICION]=[%d]\n", p_torreta->servo_x.posicion);
		fflush(stdout);
	}
}

void DisparoIR (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);
	piLock(TORRETA_FLAGS_KEY);
	flags_torreta = 0;
	piUnlock(TORRETA_FLAGS_KEY);

	piLock(PLAYER_FLAGS_KEY);
	flags_player|=FLAG_START_DISPARO;
	piUnlock(PLAYER_FLAGS_KEY);

	digitalWrite(IR_TX_PIN, HIGH);

	tmr_startms(p_torreta->tmr, SHOOTING_PERIOD);
	serialPrintf  (fd, "DISPARO");
	printf("DISPARO!\n");
	fflush(stdout);
	delay(2000);
}

void FinalDisparoIR (fsm_t* this) {
	digitalWrite(IR_TX_PIN, LOW);
	piLock(TORRETA_FLAGS_KEY);
	flags_torreta = 0;
	piUnlock(TORRETA_FLAGS_KEY);
	serialPrintf  (fd, "FINAL TIEMPO");
	printf("TIEMPO ACABADO!\n");
	fflush(stdout);

}

void ImpactoDetectado (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);
	p_torreta->marcador++;
	digitalWrite(IR_TX_PIN, LOW);
	piLock(TORRETA_FLAGS_KEY);
	flags_torreta = 0;
	piUnlock(TORRETA_FLAGS_KEY);
	printf("Marcador = %d\n",p_torreta->marcador);
	fflush(stdout);
	serialPrintf  (fd, "IMPACTO");
	delay(2000);
	serialPrintf  (fd, "x");



	//Activa la musica del impacto
	piLock(PLAYER_FLAGS_KEY);
	flags_player|=FLAG_START_IMPACTO;
	piUnlock(PLAYER_FLAGS_KEY);

}

void FinalizaJuego (fsm_t* this) {
	piLock(TORRETA_FLAGS_KEY);
	flags_torreta &= ~FLAG_SYSTEM_END;
	piUnlock(TORRETA_FLAGS_KEY);

}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_duracion_disparo_isr (union sigval value){
	piLock(TORRETA_FLAGS_KEY);
	flags_torreta|=FLAG_SHOOT_TIMEOUT;
	piUnlock(TORRETA_FLAGS_KEY);

}

void impacto(void){
	piLock(TORRETA_FLAGS_KEY);
	flags_torreta|=FLAG_TARGET_DONE;
	piUnlock(TORRETA_FLAGS_KEY);

}
