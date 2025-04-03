#include <avr/io.h>
#define F_CPU 11059200UL
#include <util/delay.h>
#include <avr/interrupt.h>

//LCD PORTA: LED|RS|RW|DB7|DB6|DB5|DB4|E
void LCD_znak(uint8_t upper, uint8_t lower);

volatile uint8_t LED = 254; //volatile potrzbne jako most ³¹cz¹cy ram z rejetrami w przypadku zmiennych globalnych

int main(void)
{
	DDRA = 255;
	DDRB = 255;
	DDRD = 0;
	
	//Przerwanie do enkodera
	GICR = 1<<INT0;
	MCUCR = 1<<ISC01 | 0<<ISC00;
	sei();
	
    _delay_ms(20);
	
	//Inicjalizacja
	PORTA = 0b00000111;
	PORTA = 0b00000110;
	_delay_ms(10);
	PORTA = 0b00000111;
	PORTA = 0b00000110;
	_delay_ms(10);
	PORTA = 0b00000111;
	PORTA = 0b00000110;
	_delay_ms(10);
	PORTA = 0b00000101;
	PORTA = 0b00000100;
	_delay_ms(10);
	
	//Function set
	PORTA = 0b00000101;
	PORTA = 0b00000100;
	
	PORTA = 0b00011001;
	PORTA = 0b00000100;
	_delay_ms(2);
	
	//Entry mode set
	PORTA = 0b00000001;
	PORTA = 0b00000000;
	
	PORTA = 0b00001101;
	PORTA = 0b00001100;
	_delay_ms(2);
	
	//Display on/off
	PORTA = 0b00000001;
	PORTA = 0b00000000;
	
	PORTA = 0b00011111;
	PORTA = 0b00011110;
	_delay_ms(2);
	
	//Clear display
	PORTA = 0b00000001;
	PORTA = 0b00000000;
	
	PORTA = 0b00000011;
	PORTA = 0b00000010;
	_delay_ms(2);
	
	//Wysy³anie znaku D
	PORTA = 0b11001001;
	PORTA = 0b11001000;
	
	PORTA = 0b11001001;
	PORTA = 0b11001000;
	_delay_ms(2);
	
	//Wysy³anie znaku a
	PORTA = 0b11001101;
	PORTA = 0b11001000;
	
	PORTA = 0b11000011;
	PORTA = 0b11001000;
	_delay_ms(2);
	
	//Wysy³anie znaku w
	PORTA = 0b11001111;
	PORTA = 0b11001000;
	
	PORTA = 0b11001111;
	PORTA = 0b11001000;
	_delay_ms(2);
	
	//Wysy³anie znaku i
	PORTA = 0b11001101;
	PORTA = 0b11001000;
	
	PORTA = 0b11010011;
	PORTA = 0b11001000;
	_delay_ms(2);
	
	//Wysy³anie znaku d
	PORTA = 0b11001101;
	PORTA = 0b11001000;
	
	PORTA = 0b11001001;
	PORTA = 0b11001000;
	_delay_ms(2);
	
	//Wysy³anie znaku spacja
	PORTA = 0b11010001;
	PORTA = 0b11001000;
	
	PORTA = 0b11000001;
	PORTA = 0b11001000;
	_delay_ms(2);
	
	//Wysy³anie znaku M
	PORTA = 0b11001001;
	PORTA = 0b11001000;
	
	PORTA = 0b11011011;
	PORTA = 0b11001000;
	_delay_ms(2);
	
	//Wysy³anie znaku a
	PORTA = 0b11001101;
	PORTA = 0b11001000;
	
	PORTA = 0b11000011;
	PORTA = 0b11001000;
	_delay_ms(2);
	
	//Wysy³anie znaku l
	PORTA = 0b11001101;
	PORTA = 0b11001000;
	
	PORTA = 0b11011001;
	PORTA = 0b11001000;
	_delay_ms(2);
	
	//Wysy³anie znaku e
	PORTA = 0b11001101;
	PORTA = 0b11001000;
	
	PORTA = 0b11001011;
	PORTA = 0b11001000;
	_delay_ms(2);
	
	//Wysy³anie znaku k
	PORTA = 0b11001101;
	PORTA = 0b11001000;
	
	PORTA = 0b11010111;
	PORTA = 0b11001000;
	_delay_ms(2);
	
	LCD_znak(0b01110, 0b10000);
	
	
    while (1) 
    {
		PORTB = LED;
		_delay_ms(1);
    }
}

ISR(INT0_vect){
	if(PIND & (1<<3)) {
		LED = (LED<<1) | (LED>>7);
	} else {
		LED = (LED>>1) | (LED<<7);
	}
}

void LCD_znak(uint8_t upper, uint8_t lower){
	//Wysy³anie upper
	PORTA = 0b11000000 + upper + 1;
	PORTA = 0b10000000;
	//Wysy³anie lower
	PORTA = 0b11000000 + lower + 1;
	PORTA = 0b10000000;
	_delay_ms(2);
}