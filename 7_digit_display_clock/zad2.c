#include <avr/io.h>
#define F_CPU 11059200UL
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t zero = 0b11000000;
uint8_t jeden = 0b11111001;
uint8_t dwa = 0b10100100;
uint8_t trzy = 0b10110000;
uint8_t cztery = 0b10011001;
uint8_t piec = 0b10010010;
uint8_t szesc = 0b10000010;
uint8_t siedem = 0b11111000;
uint8_t osiem = 0b10000000;
uint8_t dziewiec = 0b10010000;
uint8_t time = 0;



int main(void)
{
    DDRA = 255;
	DDRC = 255;
	
	//Konfiguracja licznika 1 na CTC
	TCCR1A = 0b00000000;
	TCCR1B = 0b00001100; //preskaler 256
	TIMSK = 0b00010000;
	OCR1A = 43200;
	
	PORTC = 0b11111110;
	PORTA = 0b01111111;
	
	sei();
	
    while (1) 
    {
		switch(time){
			case 0:
				PORTA = zero;
				break;
			case 1:
				PORTA = jeden;
				break;
			case 2:
				PORTA = dwa;
				break;
			case 3:
				PORTA = trzy;
				break;
			case 4:
				PORTA = cztery;
				break;
			case 5:
				PORTA = piec;
				break;
			case 6:
				PORTA = szesc;
				break;
			case 7:
				PORTA = siedem;
				break;
			case 8:
				PORTA = osiem;
				break;
			case 9:
				PORTA = dziewiec;
				break;
			case 10:
				time = 0;
				break;
		}
	}
}

ISR(TIMER1_COMPA_vect, ISR_NOBLOCK){
	time = time + 1;
}