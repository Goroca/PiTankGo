
#ifndef _PITANKGOLIB_H_
#define _PITANKGOLIB_H_
#include <stdio.h>

#include "fsm.h"
#include "tmr.h"
#include "teclado.h"
#define CLK_MS 25

// INTERVALO DE GUARDA ANTI-REBOTES
#define	DEBOUNCE_TIME	200

#define COL_REFRESH_TIME 25



// CLAVES PARA MUTEX
#define	PLAYER_FLAGS_KEY	1
#define	STD_IO_BUFFER_KEY	2
#define TECLADO_FLAGS_KEY   3
#define TORRETA_FLAGS_KEY   4


// PINES TECLADO
#define	TECLADO_ROW_1	26
#define	TECLADO_ROW_2	19
#define	TECLADO_ROW_3	13
#define	TECLADO_ROW_4	6

#define	TECLADO_COL_1	5
#define	TECLADO_COL_2	21
#define	TECLADO_COL_3	20
#define	TECLADO_COL_4	16

//FLAGS TECLADO
#define FLAG_KEY_PRESSED 0x01
#define FLAG_TMR_TIMEOUT 0x02

//FLAGS JOYSTICK
#define JOY_PIN_UP	4
#define JOY_PIN_DOWN	18
#define JOY_PIN_LEFT	23
#define JOY_PIN_RIGHT	24
#define JOY_PIN_CENTER 25

// PINES AURICULARES
#define PLAYER_PWM_PIN 	12

// FLAGS TORRETA
#define FLAG_SYSTEM_START 		0x01 //0 1
#define FLAG_JOYSTICK_UP 		0x02 //1 2
#define FLAG_JOYSTICK_DOWN 		0x04 //2 4
#define FLAG_JOYSTICK_LEFT		0x08 //3 8
#define FLAG_JOYSTICK_RIGHT		0x10 //4 16
#define FLAG_TRIGGER_BUTTON		0x20 //5 32
#define FLAG_SHOOT_TIMEOUT		0x40 //6 64
#define FLAG_TARGET_DONE		0x80 //7 128
#define FLAG_SYSTEM_END			0x100//8 256

// FLAGS PLAYER
#define FLAG_START_DISPARO 		0x01 //9
#define FLAG_START_IMPACTO 		0x02 //10
#define FLAG_PLAYER_STOP 		0x04 //11
#define FLAG_PLAYER_END			0x08 //12
#define FLAG_NOTA_TIMEOUT		0x10 //13

// TORRETA
#define SERVO_PWM_RANGE		400 // 100 * 400 = 40,000 µS = 40 ms // 25 Hz
#define SERVO_INCREMENTO	1
#define SERVO_MINIMO		9
#define SERVO_MAXIMO	    22
#define SHOOTING_PERIOD 	5000 // DURACION DISPARO IR

#define	SERVO_VERTICAL_PIN		17
#define	SERVO_HORIZONTAL_PIN	27

//PINES DISPARO
#define	IR_TX_PIN	24
#define	IR_RX_PIN	23

enum interruption_sources {
	TARGET_ISR,
	TECLADO_FILA_1,
	TECLADO_FILA_2,
	TECLADO_FILA_3,
	TECLADO_FILA_4,
	DETECTOR_IMPACTO,
	JOYSTICK_UP_ISR,
	JOYSTICK_DOWN_ISR,
	JOYSTICK_LEFT_ISR,
	JOYSTICK_RIGHT_ISR,
	JOYSTICK_CENTER_ISR
};

extern int flags_system;
extern int flags_player;
extern int flags_teclado;
extern int flags_torreta;

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
