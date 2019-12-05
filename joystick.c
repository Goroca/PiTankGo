/*
 * NO SE USA
 * CODIGO PARA JOYSTICK CON INTERRUPCIONES
 *
 */
#include "joystick.h"
#include "piTankGoLib.h"
#include "piTankGo_1.h"
#include <time.h>
#include "tmr.h"
#include <wiringPi.h>
#include <wiringSerial.h>


int InicializaJoystick(TipoJoystick *p_joystick) {


	/*

	wiringPiISR(JOY_PIN_UP, INT_EDGE_RISING, up_isr);

	pinMode(JOY_PIN_DOWN, INPUT);
	pullUpDnControl(JOY_PIN_DOWN, PUD_DOWN);
	wiringPiISR(JOY_PIN_DOWN, INT_EDGE_RISING, down_isr);

	pinMode(JOY_PIN_LEFT, INPUT);
	pullUpDnControl(JOY_PIN_LEFT, PUD_DOWN);
	wiringPiISR(JOY_PIN_LEFT, INT_EDGE_RISING, left_isr);

	pinMode(JOY_PIN_RIGHT, INPUT);
	pullUpDnControl(JOY_PIN_RIGHT, PUD_DOWN);
	wiringPiISR(JOY_PIN_RIGHT, INT_EDGE_RISING, right_isr);

	pinMode(JOY_PIN_CENTER, INPUT);
	pullUpDnControl(JOY_PIN_CENTER, PUD_DOWN);
	wiringPiISR(JOY_PIN_CENTER, INT_EDGE_RISING, center_isr);

	*/
	printf("\nJoystick ready!!!\n");
	fflush(stdout);

	return 0;
}

void up_isr(void) {
	flags_torreta |= FLAG_JOYSTICK_UP;
}
void down_isr(void) {
	flags_torreta |= FLAG_JOYSTICK_DOWN;
}
void left_isr(void) {
	flags_torreta |= FLAG_JOYSTICK_LEFT;
}
void right_isr(void) {
	flags_torreta |= FLAG_JOYSTICK_RIGHT;
}
void center_isr(void) {
	flags_torreta |= FLAG_TRIGGER_BUTTON;
}

