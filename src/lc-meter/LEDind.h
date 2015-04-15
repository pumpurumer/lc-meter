/*
sevseg lib 0x03

copyright (c) Davide Gironi, 2012

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/


#ifndef SEVSEG_H_
#define SEVSEG_H_

#include <avr/io.h>



//definitions
#define SEVSEG_TYPECC  0 //common cathode
#define SEVSEG_TYPECA  1 //common anode type
#define SEVSEG_TYPECCT 2 //common cathode / full transistor type
#define SEVSEG_TYPECAT 3 //common anode type / full transistor type

//set type
#define SEVSEG_TYPE SEVSEG_TYPECA
#define DISPLAY_COUNT 4

//set display pin, set on same port and ordered
#define SEVSEG_DDR DDRB
#define SEVSEG_PORT PORTB
#define SEVSEG_PINA PB0
#define SEVSEG_PINB PB1
#define SEVSEG_PINC PB2
#define SEVSEG_PIND PB3
#define SEVSEG_PINE PB4
#define SEVSEG_PINF PB5
#define SEVSEG_PING PB6
#define SEVSEG_PINDOT PB7

//set display power port
#define SEVSEG_DDRDISP DDRD
#define SEVSEG_PORTDISP PORTD
#define SEVSEG_PINDISP1 PD2
#define SEVSEG_PINDISP2 PD3
#define SEVSEG_PINDISP3 PD4
#define SEVSEG_PINDISP4 PD5

//set display array (of pin)
#define SEVSEG_PINDISPA {SEVSEG_PINDISP1, SEVSEG_PINDISP2, SEVSEG_PINDISP3,SEVSEG_PINDISP4}

//set minimum power on display time (us)
#define SEVSEG_USONTIME 1000

//functions
extern void sevseg_init();
/*extern void sevseg_setfirstdisplayactive();
extern void sevseg_putc(uint8_t c, uint8_t dot);
extern void sevseg_puthex(uint8_t h);
extern uint8_t sevseg_selnextdisplay();
*/
extern void writeToDisplay();
extern void setDisplayValue(float val);
#endif
