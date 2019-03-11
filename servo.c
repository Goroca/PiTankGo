/*
 * servo.c
 *
 *  Created on: 9 mar. 2019
 *      Author: Carlos
 */
#include "servo.h"
#include "piTankGoLib.h"
#include "piTankGo_1.h"
#include <time.h>
#include "tmr.h"
#include <wiringPi.h>

int InicializaServo (TipoServo *p_servo) {
	int result = 0;

	wiringPiSetupGpio();

	p_servo->incremento = SERVO_INCREMENTO;
	p_servo->minimo 	= SERVO_MINIMO;
	p_servo->maximo 	= SERVO_MAXIMO;

	p_servo->inicio 	= SERVO_MINIMO + (SERVO_MAXIMO - SERVO_MINIMO)/2;
	p_servo->posicion 	= p_servo->inicio;

	if(p_servo->posicion > p_servo->maximo)
		p_servo->posicion = p_servo->maximo;

	if(p_servo->posicion < p_servo->minimo)
		p_servo->posicion = p_servo->minimo;

	softPwmCreate (SERVO_PIN, p_servo->inicio, SERVO_PWM_RANGE); // Internamente ya hace: piHiPri (90) ;
	softPwmWrite(SERVO_PIN, p_servo->posicion);

	return result;
}

int CompruebaIzquierda (fsm_t* this) {
	int result = 0;
	result = (flags_servo & FLAG_SERVO_LEFT);
	return result;
}

int CompruebaDerecha (fsm_t* this) {
	int result = 0;
	piLock(SERVO_FLAGS_KEY);
	result = (flags_servo & FLAG_SERVO_RIGHT);
	piUnlock(SERVO_FLAGS_KEY);
	return result;
}

void MueveServoIzquierda (fsm_t* this) {
	TipoServo *p_servo;
	p_servo = (TipoServo*)(this->user_data);
	piLock(SERVO_FLAGS_KEY);

	flags_servo &= (~FLAG_SERVO_LEFT);
	piUnlock(SERVO_FLAGS_KEY);

	if(p_servo->posicion - p_servo->incremento >= p_servo->minimo) {
		p_servo->posicion = p_servo->posicion - p_servo->incremento;

		softPwmWrite(SERVO_PIN, p_servo->posicion);

		printf("[SERVO][POSICION]=[%d]\n", p_servo->posicion);
		fflush(stdout);
	}
}

void MueveServoDerecha (fsm_t* this) {
	TipoServo *p_servo;
	p_servo = (TipoServo*)(this->user_data);
	piLock(SERVO_FLAGS_KEY);
	flags_servo &= (~FLAG_SERVO_RIGHT);
	piUnlock(SERVO_FLAGS_KEY);

	if(p_servo->posicion + p_servo->incremento <= p_servo->maximo) {
		p_servo->posicion = p_servo->posicion + p_servo->incremento;

		softPwmWrite(SERVO_PIN, p_servo->posicion);

		printf("[SERVO][POSICION]=[%d]\n", p_servo->posicion);
		fflush(stdout);
	}
}

int comprueba_teclado_PC (fsm_t* this) {
	if(kbhit())
		return 1;

	return 0;
}

void procesa_teclado_PC (fsm_t* this) {
	int teclaPulsada;

	teclaPulsada = kbread();

	switch(teclaPulsada) {
		case 'o':

			flags_servo |= FLAG_SERVO_LEFT;

			printf("\n[PULSACION][SERVO LEFT!!!!]\n");
			fflush(stdout);
			break;

		case 'p':
			flags_servo |= FLAG_SERVO_RIGHT;

			printf("\n[PULSACION][SERVO RIGHT!!!!]\n");
			fflush(stdout);
			break;

		case 'q':
			printf("\n[EXIT]\n");
			fflush(stdout);
			exit(0);

		default:
			printf("INVALID KEY!!!\n");
			break;
	}
}
