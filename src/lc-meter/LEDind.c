/*
sevseg lib 0x03

copyright (c) Davide Gironi, 2012

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/
#define F_CPU			1000000UL
#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define FLIPBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))

#include <stdio.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
//#include <math.h>

#include "LEDind.h"

uint8_t sevseg_displays[] = SEVSEG_PINDISPA;

//active display
//volatile uint8_t sevseg_displayactive = 0;

//lookup table ascii char to 7 segment
#define SEGN 0xFF
#define SEGU 0xF7 //unknown way of display this character
/*static const uint8_t PROGMEM sevsegascii_table[] = {
	SEGN, SEGN, SEGN, SEGN,
	SEGN, SEGN, SEGN, SEGN,
	SEGN, SEGN, SEGN, SEGN,
	SEGN, SEGN, SEGN, SEGN,
	SEGN, SEGN, SEGN, SEGN,
	SEGN, SEGN, SEGN, SEGN,
	SEGN, SEGN, SEGN, SEGN,
	SEGN, SEGN, SEGN, SEGN,
	SEGN, SEGN, SEGN, SEGN,
	SEGN, SEGN, SEGN, SEGN,
	SEGN, SEGN, SEGN, SEGN,
	SEGN, SEGN, SEGN, SEGN,
	0xc0, 0xf9, 0xa4, 0xb0, // 0 1 2 3
	0x99, 0x92, 0x82, 0xf8, // 4 5 6 7
	0x80, 0x90, SEGN, SEGN, // 8 9
	SEGN, SEGN, SEGN, SEGN,
	SEGN, 0x88, 0x83, 0xc6, //   A B C
	0xa1, 0x86, 0x8e, 0xc2, // D E F G
	0x8b, 0xcf, 0xe1, SEGU, // H I J _
	0xc7, 0xc8, 0xab, 0xc0, // L M N O
	0x8c, 0x98, 0xaf, 0x92, // P Q R S
	0x87, 0xc1, 0xe3, SEGU, // T U V _
	SEGU, 0x91, 0xa4, SEGN, // _ Y Z
	SEGN, SEGN, SEGN, SEGN,
	SEGN, 0x88, 0x83, 0xc6, //   A B C
	0xa1, 0x86, 0x8e, 0xc2, // D E F G
	0x8b, 0xcf, 0xe1, SEGU, // H I J _
	0xc7, 0xc8, 0xab, 0xc0, // L M N O
	0x8c, 0x98, 0xaf, 0x92, // P Q R S
	0x87, 0xc1, 0xe3, SEGU, // T U V _
	SEGU, 0x91, 0xa4, SEGN, // _ Y Z
	SEGN, SEGN, SEGN, SEGN
};*/

static const uint8_t segments_int_table[] = {
	0xc0, 0xf9, 0xa4, 0xb0, // 0 1 2 3
	0x99, 0x92, 0x82, 0xf8, // 4 5 6 7
	0x80, 0x90 // 8 9
};
//#define DISPTYPE float

float lastDataValue = 0;

void setDisplayValue(float val)
{
	/*if(val < 0)
	{
		val = 0;
	}*/
	lastDataValue = val;
}
#define DISPHISTORYSIZE 10
//unsigned int displayHistory[DISPHISTORYSIZE];
unsigned char currentHistoryPosition = 0;

unsigned numDigits(const unsigned n) {
	if (n < 10) return 1;
	return 1 + numDigits(n / 10);
}

/*unsigned numPow(const unsigned n) {
	if (n <=1) return 10;
	return 10 * numPow(n-1);
}
*/
void writeToDisplay()
{
	//unsigned int IntPart = lastDataValue;
	//volatile unsigned int dispValue = lastDataValue;
	//unsigned int dotValue = 0;
	
	if(SEVSEG_TYPE == SEVSEG_TYPECC || SEVSEG_TYPE == SEVSEG_TYPECAT)
	SEVSEG_PORT = 0X00;
	else if(SEVSEG_TYPE == SEVSEG_TYPECA || SEVSEG_TYPE == SEVSEG_TYPECCT)
	SEVSEG_PORT = 0xFF;
	
	unsigned char tmp;
	unsigned char len = numDigits(lastDataValue);
	unsigned int dotValue = 5-len;
	float blabla = lastDataValue;
	
	for(char bla =1; bla < dotValue; bla++)
	{
		blabla = blabla * 10;
	}
	unsigned int dispValue = blabla > 0 ? blabla : 0;
	
	
	for(char a =1; a <= DISPLAY_COUNT; a++)
	{
		tmp = dispValue % 10;
		dispValue = dispValue/10;
		
		SEVSEG_PORT = segments_int_table[tmp];
		if(dotValue == a)
		{
			CLEARBIT(SEVSEG_PORT, SEVSEG_PINDOT);
		}
		//_delay_ms(20);
		SETBIT(SEVSEG_PORTDISP,sevseg_displays[a-1]);
		_delay_ms(20);
		CLEARBIT(SEVSEG_PORTDISP,sevseg_displays[a-1]);
		//_delay_ms(20);
	}
}



#define AMOUNT_NUM 4

//volatile unsigned char buf[AMOUNT_NUM];




/*
 * init the seven segment ports
 */
void sevseg_init() {
	//output
	SEVSEG_DDR = 0xff;
	/*SEVSEG_DDR |= (1 << SEVSEG_PINA);
	SEVSEG_DDR |= (1 << SEVSEG_PINB);
	SEVSEG_DDR |= (1 << SEVSEG_PINC);
	SEVSEG_DDR |= (1 << SEVSEG_PIND);
	SEVSEG_DDR |= (1 << SEVSEG_PINE);
	SEVSEG_DDR |= (1 << SEVSEG_PINF);
	SEVSEG_DDR |= (1 << SEVSEG_PING);
	SEVSEG_DDR |= (1 << SEVSEG_PINDOT);*/
	//off
	/*if(SEVSEG_TYPE == SEVSEG_TYPECC || SEVSEG_TYPE == SEVSEG_TYPECAT)
		SEVSEG_PORT = 0x00;
	else if(SEVSEG_TYPE == SEVSEG_TYPECA || SEVSEG_TYPE == SEVSEG_TYPECCT)
		SEVSEG_PORT = 0xFF;
*/
	/*SEVSEG_PORTDISP =*/ SEVSEG_DDRDISP = (1 << SEVSEG_PINDISP1) |(1 << SEVSEG_PINDISP2) |(1 << SEVSEG_PINDISP3) |(1 << SEVSEG_PINDISP4)  ;
	/*SEVSEG_DDRDISP |= (1 << SEVSEG_PINDISP1);
	SEVSEG_DDRDISP |= (1 << SEVSEG_PINDISP2);
	SEVSEG_DDRDISP |= (1 << SEVSEG_PINDISP3);
	SEVSEG_DDRDISP |= (1 << SEVSEG_PINDISP4);
	
	SEVSEG_PORTDISP |= (1 << SEVSEG_PINDISP1);
	SEVSEG_PORTDISP |= (1 << SEVSEG_PINDISP2);
	SEVSEG_PORTDISP |= (1 << SEVSEG_PINDISP3);
	SEVSEG_PORTDISP |= (1 << SEVSEG_PINDISP4);
	*/
	/*for(uint8_t i = 0; i< DISPLAY_COUNT; i++) {
		SEVSEG_DDRDISP |= (1 << sevseg_displays[i]);
		if(SEVSEG_TYPE == SEVSEG_TYPECA || SEVSEG_TYPE == SEVSEG_TYPECAT)
			SEVSEG_PORTDISP |= (1 << sevseg_displays[i]);
		else if(SEVSEG_TYPE == SEVSEG_TYPECC  || SEVSEG_TYPE == SEVSEG_TYPECCT)
			SEVSEG_PORTDISP &= ~(1 << sevseg_displays[i]);
	}*/
}

/*
 * reset to first display active
 */
/*void sevseg_setfirstdisplayactive() {
	sevseg_displayactive = 0;
}*/

/*
 * print a character, and eventually the dot
 */
/*void sevseg_putc(uint8_t c, uint8_t dot) {
	uint8_t cdisp = pgm_read_byte(&sevsegascii_table[c]);
	if(dot)
		cdisp &= ~(1<<7);
	if(SEVSEG_TYPE == SEVSEG_TYPECC|| SEVSEG_TYPE == SEVSEG_TYPECAT)
		SEVSEG_PORT = 0XFF ^ cdisp;
	else if(SEVSEG_TYPE == SEVSEG_TYPECA || SEVSEG_TYPE == SEVSEG_TYPECCT)
		SEVSEG_PORT = cdisp;
	_delay_us(SEVSEG_USONTIME); //time for the led to show
}*/

/*
 * print an hex value
 */
/*void sevseg_puthex(uint8_t hex) {
	SEVSEG_PORT = hex;
	_delay_us(SEVSEG_USONTIME); //time for the led to show
}
*/
/*
 * select which display to run, continuous iteration to next display
 */
/*uint8_t sevseg_selnextdisplay() {
	//off digit
	if(SEVSEG_TYPE == SEVSEG_TYPECC || SEVSEG_TYPE == SEVSEG_TYPECAT)
		SEVSEG_PORT = 0X00;
	else if(SEVSEG_TYPE == SEVSEG_TYPECA || SEVSEG_TYPE == SEVSEG_TYPECCT)
		SEVSEG_PORT = 0xFF;
	//off display
	if(SEVSEG_TYPE == SEVSEG_TYPECA || SEVSEG_TYPE == SEVSEG_TYPECAT) {
		SEVSEG_PORTDISP |= (1 << sevseg_displays[sevseg_displayactive]);
	} else if(SEVSEG_TYPE == SEVSEG_TYPECC || SEVSEG_TYPE == SEVSEG_TYPECCT) {
		SEVSEG_PORTDISP &= ~(1 << sevseg_displays[sevseg_displayactive]);
	}
	if(sevseg_displayactive < sizeof(sevseg_displays)-1)
		sevseg_displayactive++;
	else
		sevseg_displayactive = 0;
	 //on display
	if(SEVSEG_TYPE == SEVSEG_TYPECA || SEVSEG_TYPE == SEVSEG_TYPECAT) {
		SEVSEG_PORTDISP &= ~(1 << sevseg_displays[sevseg_displayactive]);
	} else if(SEVSEG_TYPE == SEVSEG_TYPECC || SEVSEG_TYPE == SEVSEG_TYPECCT) {
		SEVSEG_PORTDISP |= (1 << sevseg_displays[sevseg_displayactive]);
	}
	return sevseg_displayactive;
}
*/