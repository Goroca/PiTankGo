/*
 * NO SE USA
 * CODIGO PARA JOYSTICK CON INTERRUPCIONES
 *
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

typedef struct {

} TipoJoystick;


int InicializaJoystick(TipoJoystick *p_joystick);
void up_isr (void);
void down_isr (void);
void left_isr (void);
void right_isr (void);
void center_isr (void);

#endif /* JOYSTICK_H_ */
