#include <avr/io.h>
#define F_CPU 11059200UL
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t jeden = 0b11111001;
uint8_t siedem = 0b11111000;
uint8_t osiem = 0b10000000;

int main(void)
{
    DDRA = 255;
	DDRC = 255;
	
	PORTC = 255;
    while (1) 
    {
		PORTA = osiem;
		PORTC = 0b11111110;
		_delay_ms(5);
		
		PORTA = jeden;
		PORTC = 0b11111101;
		_delay_ms(5);
		
		PORTA = siedem;
		PORTC = 0b11111011;
		_delay_ms(5);
    }
}