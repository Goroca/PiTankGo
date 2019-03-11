/*
 * teclado.h
 *
 *  Created on: 4 mar. 2019
 *      Author: Carlos
 */

#ifndef TECLADO_H_
#define TECLADO_H_


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "fsm.h"
#include "tmr.h"



#define NUM_COLS 4
#define NUM_ROWS 4

//FLAGS TECLADO
#define FLAG_KEY_PRESSED 0x01
#define FLAG_TMR_TIMEOUT 0x02

enum columns_values {
	COL_1,
	COL_2,
	COL_3,
	COL_4,
};

enum rows_values {
	ROW_1,
	ROW_2,
	ROW_3,
	ROW_4
};

enum column_state {
	KEY_COL_1,
	KEY_COL_2,
	KEY_COL_3,
	KEY_COL_4
};

enum keypad_state {
	KEY_WAITING
};

typedef struct {
	int col;
	int row;
} TipoTecla;

typedef struct {
	int columna_actual;
	tmr_t* tmr_duracion_columna;
	TipoTecla teclaPulsada;
	char ultimaTeclaPulsada;  // Variable que almacena la ultima tecla pulsada
} TipoTeclado;

extern TipoTeclado* teclado;

int InicializaTeclado(TipoTeclado *p_teclado);
int CompruebaColumnTimeout (fsm_t* this);
void timer_duracion_columna_isr (union sigval value);

void col_1 (fsm_t* this);
void col_2 (fsm_t* this);
void col_3 (fsm_t* this);
void col_4 (fsm_t* this);

void row_1_isr (void);
void row_2_isr (void);
void row_3_isr (void);
void row_4_isr (void);

void process_key (fsm_t* this);
int key_pressed (fsm_t* this);

#endif /* TECLADO_H_ */
