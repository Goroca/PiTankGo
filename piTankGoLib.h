
#ifndef _PITANKGOLIB_H_
#define _PITANKGOLIB_H_
#include <stdio.h>

#include "fsm.h"
#include "tmr.h"
#include "teclado.h"
#define CLK_MS 25

// ATENCION: Valores a modificar por el alumno
// INTERVALO DE GUARDA ANTI-REBOTES
#define	DEBOUNCE_TIME	200

#define COL_REFRESH_TIME 25
// DURACION DISPARO IR
#define SHOOTING_PERIOD 0

// CLAVES PARA MUTEX
// ATENCION: Valores a modificar por el alumno
//#define	SYSTEM_FLAGS_KEY	0
#define	PLAYER_FLAGS_KEY	1
#define	STD_IO_BUFFER_KEY	2
#define TECLADO_FLAGS_KEY   3


// Distribucion de pines GPIO empleada para el teclado
// ATENCION: Valores a modificar por el alumno
#define	TECLADO_ROW_1	26
#define	TECLADO_ROW_2	19
#define	TECLADO_ROW_3	13
#define	TECLADO_ROW_4	6

#define	TECLADO_COL_1	5
#define	TECLADO_COL_2	21
#define	TECLADO_COL_3	20
#define	TECLADO_COL_4	16

//#define JOY_PIN_UP	0
//#define JOY_PIN_DOWN	0
//#define JOY_PIN_LEFT	0
//#define JOY_PIN_RIGHT	0
//#define JOY_PIN_CENTER 0

// Distribucion de pines GPIO empleada para el enlace IR
// ATENCION: Valores a modificar por el alumno
#define	IR_TX_PIN		0
#define	IR_RX_PIN		0

// Distribucion de pines GPIO empleada para la reproducción de efectos
// ATENCION: Valores a modificar por el alumno
#define PLAYER_PWM_PIN 	12

// Distribucion de pines GPIO empleada para el control de los servos
// ATENCION: Valores a modificar por el alumno
#define	SERVO_VERTICAL_PIN		0
#define	SERVO_HORIZONTAL_PIN	0

//FLAGS TECLADO
#define FLAG_KEY_PRESSED 0x01
#define FLAG_TMR_TIMEOUT 0x02

// FLAGS FSM CONTROL DE JUEGO Y TORRETA
// ATENCION: Valores a modificar por el alumno
#define FLAG_SYSTEM_START 		0x01 //0
#define FLAG_JOYSTICK_UP 		0x02 //1
#define FLAG_JOYSTICK_DOWN 		0x04 //2
#define FLAG_JOYSTICK_LEFT		0x08 //3
#define FLAG_JOYSTICK_RIGHT		0x10 //4
#define FLAG_TRIGGER_BUTTON		0x20 //5
#define FLAG_SHOOT_TIMEOUT		0x40 //6
#define FLAG_TARGET_DONE		0x80 //7
#define FLAG_SYSTEM_END			0x256//8
// FLAGS FSM REPRODUCCION DE EFECTOS DE SONIDO
// ATENCION: Valores a modificar por el alumno
#define FLAG_START_DISPARO 		0x01 //9
#define FLAG_START_IMPACTO 		0x02 //10
#define FLAG_PLAYER_STOP 		0x04 //11
#define FLAG_PLAYER_END			0x08 //12
#define FLAG_NOTA_TIMEOUT		0x10 //13

enum interruption_sources {
	TARGET_ISR,
	TECLADO_FILA_1,
	TECLADO_FILA_2,
	TECLADO_FILA_3,
	TECLADO_FILA_4,
	//JOYSTICK_UP_ISR,
	//JOYSTICK_DOWN_ISR,
	//JOYSTICK_LEFT_ISR,
	//JOYSTICK_RIGHT_ISR,
	//JOYSTICK_CENTER_ISR
};

extern int flags_system;
extern int flags_player;
extern int flags_teclado;

extern int frecuenciaDespacito[];
extern int tiempoDespacito[];
extern int frecuenciaGOT[];
extern int tiempoGOT[];
extern int frecuenciaTetris[];
extern int tiempoTetris[];
extern int frecuenciaStarwars[];
extern int tiempoStarwars[];

extern int frecuenciasDisparo[];
extern int tiemposDisparo[];
extern int frecuenciasImpacto[];
extern int tiemposImpacto[];

#endif /* _PITANKGOLIB_H_ */
