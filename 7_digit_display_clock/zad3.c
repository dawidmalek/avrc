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

uint8_t zerok = 0b01000000;
uint8_t jedenk = 0b01111001;
uint8_t dwak = 0b00100100;
uint8_t trzyk = 0b00110000;
uint8_t czteryk = 0b00011001;
uint8_t pieck = 0b00010010;
uint8_t szesck = 0b00000010;
uint8_t siedemk = 0b01111000;
uint8_t osiemk = 0b00000000;
uint8_t dziewieck = 0b00010000;

uint8_t sek1 = 0;
uint8_t sek2 = 0;
uint8_t min1 = 0;
uint8_t min2 = 0;



int main(void)
{
    DDRA = 255;
	DDRC = 255;
	
	//Konfiguracja licznika 1 na CTC
	TCCR1A = 0b00000000;
	TCCR1B = 0b00001100; //preskaler 256
	TIMSK = 0b00010000;
	OCR1A = 43200;
	
	PORTC = 0b11110000;
	PORTA = 0b01111111;
	
	sei();
	
    while (1) 
    {
		//jedności sekund
		PORTC = 0b11110111;
		switch(sek1){
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
		}
		
		//dziesiątki sekund
		_delay_us(500);
		PORTC = 0b11111011;
		switch(sek2){
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
			}
			
			//jedności minut
			_delay_us(500);
			PORTC = 0b11111101;
			switch(min1){
				case 0:
				PORTA = zerok;
				break;
				case 1:
				PORTA = jedenk;
				break;
				case 2:
				PORTA = dwak;
				break;
				case 3:
				PORTA = trzyk;
				break;
				case 4:
				PORTA = czteryk;
				break;
				case 5:
				PORTA = pieck;
				break;
				case 6:
				PORTA = szesck;
				break;
				case 7:
				PORTA = siedemk;
				break;
				case 8:
				PORTA = osiemk;
				break;
				case 9:
				PORTA = dziewieck;
				break;
			}

			//dziesiątki minut
			_delay_us(500);
			PORTC = 0b11111110;
			switch(min2){
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
			}
			_delay_us(500);
			
	}
}

ISR(TIMER1_COMPA_vect, ISR_NOBLOCK){
	sek1 = sek1 + 1;
	
	if(sek1 == 10){
		sek2 = sek2 + 1;
		sek1 = 0;
	}
	
	if(sek2 == 6){
		min1 = min1 + 1;
		sek2 = 0;
	}
	
	if(min1 == 10){
		min2 = min2 + 1;
		min1 = 0;
	}
}