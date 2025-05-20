#include <avr/io.h>
#define F_CPU 11059200UL
#include <util/delay.h>
#include <avr/interrupt.h>

//LCD PORTA: LED|RS|RW|DB7|DB6|DB5|DB4|E
void LCD_init();
void LCD_home();
void LCD_znak(uint8_t upper, uint8_t lower);

#define zero LCD_znak(0b00000110, 0b00000000);
#define jeden LCD_znak(0b00000110, 0b00000010);
#define dwa LCD_znak(0b00000110, 0b00000100);
#define trzy LCD_znak(0b00000110, 0b00000110);
#define cztery LCD_znak(0b00000110, 0b00001000);
#define piec LCD_znak(0b00000110, 0b00001010);
#define szesc LCD_znak(0b00000110, 0b00001100);
#define siedem LCD_znak(0b00000110, 0b00001110);
#define osiem LCD_znak(0b00000110, 0b00010000);
#define dziewiec LCD_znak(0b00000110, 0b00010010);

int main(void)
{
	uint8_t TH, TL;
	uint16_t temp;
	DDRA = 255;
	DDRB = 255;
	DDRB |= (0 << PB6);
	PORTB |= (1 << PB4);
	DDRC = 255; //LED
	PORTC = 255;
	
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR1);
	
	LCD_init();

	
    while (1) 
    {
		PORTB &= ~(1 << PB4);
		SPDR = 8;
		while(!(SPSR & (1<<SPIF)));
		TH = SPDR;
		
		SPDR = 4;
		while(!(SPSR & (1<<SPIF)));
		TL = SPDR;
		PORTB |= (1 << PB4);
		
		temp = (TH*256 + TL) >> 7;
		
		uint16_t dziesiatki = temp / 10;
		uint16_t jednosci = temp - dziesiatki*10;
		
		switch(dziesiatki){
			case 0:
				LCD_home();
				zero;
				break;
			case 1:
				LCD_home();
				jeden;
				break;
			case 2:
				LCD_home();
				dwa;
				break;
			case 3:
				LCD_home();
				trzy;
				break;
			case 4:
				LCD_home();
				cztery;
				break;
			case 5:
				LCD_home();
				piec;
				break;
			case 6:
				LCD_home();
				szesc;
				break;
			case 7:
				LCD_home();
				siedem;
				break;
			case 8:
				LCD_home();
				osiem;
				break;
			case 9:
				LCD_home();
				dziewiec;
				break;
		}
		switch(jednosci){
			case 0:
			zero;
			break;
			case 1:
			jeden;
			break;
			case 2:
			dwa;
			break;
			case 3:
			trzy;
			break;
			case 4:
			cztery;
			break;
			case 5:
			piec;
			break;
			case 6:
			szesc;
			break;
			case 7:
			siedem;
			break;
			case 8:
			osiem;
			break;
			case 9:
			dziewiec;
			break;
		}
		
		_delay_ms(500);
		
    }
}
//void drugi_znak(){
//	if()
//}

void LCD_znak(uint8_t upper, uint8_t lower){
	//LED|RS|RW|DB7|DB6|DB5|DB4|E
	//Wysy³anie upper
	PORTA = 0b11000000 + upper + 1;
	PORTA = 0b10000000;
	//Wysy³anie lower
	PORTA = 0b11000000 + lower + 1;
	PORTA = 0b10000000;
	_delay_ms(2);
}

void LCD_home(){
		//Return home
		PORTA = 0b00000001;
		PORTA = 0b10000000;

		PORTA = 0b00000011;
		PORTA = 0b10000100;
		_delay_ms(2);
}

void LCD_init(){
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
	    PORTA = 0b10000000;
		
		PORTA = 0b00000011;
		PORTA = 0b10000010;
		_delay_ms(2);
}
