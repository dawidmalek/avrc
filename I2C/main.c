#include <avr/io.h>
#define F_CPU 11059200UL
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
	float temperatura_obliczona;
	float wilgotnosc_obliczona;
	
	uint16_t temperatura_suma;
	uint16_t wilgotnosc_suma;
	
	uint8_t temperatura1;
	uint8_t temperatura2;
	uint8_t wilgotnosc1;
	uint8_t wilgotnosc2;
	
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
	#define procent LCD_znak(0b00000100, 0b00001010);
	#define stopien LCD_znak(0b00011010, 0b00011110);
	#define litera_C LCD_znak(0b00001000, 0b00000110);
	#define spacja LCD_znak(0b00010000, 0b00000000);
	
	DDRD = 255;
    	DDRA = 255;
	PORTA = 255;
	DDRC = 0;
	PORTC = 255;
	
	LCD_init();
	//I2C inicjalizacja
	//preskaler = 1
	TWSR = 0;
	
	TWBR = 23;
	//f i2c = 102400 Hz
	
	//W³¹czenie magistrali i wys³anie bitu start
	TWCR = (1<<TWEN) | (1<<TWSTA);
	while(!(TWCR & (1<<TWINT)));
	
	//adress czujnika i nadawanie
	TWDR = (0x10 << 1) | 0;
	TWCR = (1<<TWEN) | (1<<TWINT);
	while(!(TWCR & (1<<TWINT)));
	
	//Koñczenie transmisji, wysy³anie stop
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTO);
	
	//RECIVER MODE
	_delay_ms(40);
	//W³¹czenie magistrali i wys³anie bitu start
	TWCR = (1<<TWEN) | (1<<TWSTA);
	while(!(TWCR & (1<<TWINT)));
	
	//adress czujnika i odbieranie
	TWDR = (0x10 << 1) | 1;
	TWCR = (1<<TWEN) | (1<<TWINT);
	while(!(TWCR & (1<<TWINT)));
	
	//odes³anie ack, odczyt danych
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));
	wilgotnosc1 = TWDR;
	
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));
	wilgotnosc2 = TWDR;
	
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));
	temperatura1 = TWDR;
	
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));
	temperatura2 = TWDR;
	_delay_ms(40);
	
	wilgotnosc_suma = ((wilgotnosc1 & 0b00111111) << 8) | wilgotnosc2;
	temperatura_suma = (temperatura1 << 6) | (temperatura2 >> 2);
	
	wilgotnosc_obliczona = ((float)wilgotnosc_suma/16382.0)*100.0;
	temperatura_obliczona = ((float)temperatura_suma/16382.0)*165.0-40.0;
	
    while (1) 
    {
		uint8_t dziesiatki_temperatura = (uint8_t)temperatura_obliczona/10;
		uint8_t jednosci_temperatura = (uint8_t)temperatura_obliczona-dziesiatki_temperatura*10;
		PORTD = ~temperatura_obliczona;
		_delay_ms(1000);
		PORTD = ~dziesiatki_temperatura;
		_delay_ms(1000);
		PORTD = 255;
		_delay_ms(1000);
		
		switch(dziesiatki_temperatura){
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
    }
}


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
