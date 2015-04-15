/*
* GccApplication1.c
*
* Created: 08.04.2015 17:29:34
*  Author: dkozh_000
*/
#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define FLIPBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))

#define F_CPU			1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LEDind.h"
// Количество замеров
unsigned int NxValue = 0;
// Количество тактов
unsigned long nxValue = 0;

unsigned int N0Value = 1;
unsigned long n0Value = 50;

unsigned char inVect = 0;

int main(void)
{
	//DDRD   = 0b00111100;
	//PORTD = 0b10111100;  // 0b10111100
	//SETBIT(PORTD,6);
	//DDRB   = 0xFF;  // 0b11111111
	//PORTB = 0x55;  // 0b01010101
	//_delay_ms(3000);
	
	

	TIMSK = (1<<ICIE1)/* | (1<<TOIE1)*/;
	
	TCCR1A=(0<<WGM11)|(0<<WGM10);
	TCCR1B=(0<<ICNC1)|(0<<ICES1)|(0<<WGM13)|(0<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10);
	TCNT1 = 0;
	
	TCCR0A = 0;
	TCCR0B = (1<<CS02)|(0<<CS01)|(1<<CS00);
	SETBIT(TIMSK,TOIE0);
	
	sei();
	
	sevseg_init();
	
	CLEARBIT(DDRD, PD0);
	SETBIT(PORTD, PD0);
	
	
	
	
	
	//setDisplayValue(12.34);
	//writeToDisplay();
	/**/
	/*char counter = 0;
	float valtoDisp = -1234;
	setDisplayValue(valtoDisp);
	*/
	//PORTB = 0xff;
	
	/*SETBIT(PORTD, PD5);
	SETBIT(PORTD, PD4);
	*/
	
	while(1)
	{
		/*counter++;
		if(counter == 10)
		{
			valtoDisp = valtoDisp/10;
			setDisplayValue(valtoDisp);
			counter = 0;
		}*/
		//_delay_ms(50);
		writeToDisplay();
	}
}
/*
ISR(TIMER1_OVF_vect)
{
	SETBIT(PORTD, PD5);
	CLEARBIT(PORTB, PB0);
	_delay_ms(5);
	CLEARBIT(PORTD, PD5);
	SETBIT(PORTB, PB0);
}*/

ISR(TIMER1_CAPT_vect)
{
	if(!inVect && ICR1L > 1)
	{
		inVect = 1;
		NxValue++;
		nxValue+= ICR1L;
		TCNT1 = 0;         //обнуляем счетный регистр
		inVect = 0;
	}
}


unsigned char tm0_diviser = 0;

ISR(TIMER0_OVF_vect)
{
	if(tm0_diviser == 20)
	{
		if(!inVect)
		{
			inVect = 1;
			if((PIND & (1 <<  PD0)) == 0)
			{
				//CLEARBIT(PORTB, PB0);
				//_delay_ms(10);
				//SETBIT(PORTB, PB0);
				n0Value = nxValue;
				N0Value = NxValue;
			}
			
				
			//float lValue = (nxValue*N0Value/n0Value*NxValue)*(nxValue*N0Value/n0Value*NxValue)-1;
			//setDisplayValue((nxValue*N0Value/n0Value*NxValue)*(nxValue*N0Value/n0Value*NxValue)-1);
			//setDisplayValue((nxValue * 1.0)/NxValue  - (n0Value * 1.0)/N0Value);
			setDisplayValue((nxValue * 1.0 - n0Value)/(NxValue - N0Value) /** 81.0223305447599 * 76.13826709304096*/);
			nxValue = 0;
			NxValue = 0;
			
			
			tm0_diviser = 0;
			inVect = 0;
		}
	}
	else
	{
		tm0_diviser++;
	}
	
}