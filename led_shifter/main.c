/*
 * lab1.c
 *
 * Created: 04.03.2025 13:54:39
 * Author : Student
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>


int main(void)
{
    DDRA = 255;
	uint8_t LED = 0b01111111;
	int c = 0;
    while (1) 
    {
// 		PORTA = 0;
//  		_delay_ms(500);
//  		PORTA = 255;
//  		_delay_ms(500);
		 PORTA = LED;
		 _delay_ms(100);
		 if(c <= 7){
			 LED = (LED >> 7) | (LED << 1);
			 c++;
		 } else {
			 LED = (LED >> 1) | (LED << 7);
			 c--;
		 }
		 _delay_ms(100);
    }
}

