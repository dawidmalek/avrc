#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>

// Port TM1637
#define TM1637_DDR DDRA
#define TM1637_PORT PORTA
#define TM1637_PIN PINA
#define DIO PA1
#define CLK PA0

void TM1637_start() {
	TM1637_PORT |= (1 << DIO);
	TM1637_PORT |= (1 << CLK);
	_delay_us(2);
	TM1637_PORT &= ~(1 << DIO);
	_delay_us(2);
	TM1637_PORT &= ~(1 << CLK);
}

void TM1637_stop() {
	TM1637_PORT &= ~(1 << CLK);
	_delay_us(2);
	TM1637_PORT &= ~(1 << DIO);
	_delay_us(2);
	TM1637_PORT |= (1 << CLK);
	_delay_us(2);
	TM1637_PORT |= (1 << DIO);
}

void TM1637_writeByte(uint8_t b) {
	for (uint8_t i = 0; i < 8; i++) {
		if (b & 0x01)
		TM1637_PORT |= (1 << DIO);
		else
		TM1637_PORT &= ~(1 << DIO);

		TM1637_PORT |= (1 << CLK);
		_delay_us(3);
		TM1637_PORT &= ~(1 << CLK);
		_delay_us(3);
		b >>= 1;
	}

	// ACK
	TM1637_DDR &= ~(1 << DIO); // Input
	TM1637_PORT |= (1 << DIO); // Pull-up
	TM1637_PORT |= (1 << CLK);
	_delay_us(5);
	TM1637_PORT &= ~(1 << CLK);
	TM1637_DDR |= (1 << DIO); // Output again
}

void TM1637_display(uint8_t *data) {
	TM1637_start();
	TM1637_writeByte(0x40); // Auto increment mode
	TM1637_stop();

	TM1637_start();
	TM1637_writeByte(0xC0); // Start address
	for (uint8_t i = 0; i < 4; i++)
	TM1637_writeByte(data[i]+0b10000000);
	TM1637_stop();
}

void TM1637_setBrightness(uint8_t brightness, bool displayOn) {
	if (brightness > 7) brightness = 7; // zakres 0–7

	uint8_t command = 0x88 | (brightness & 0x07); // 0x88 = display control
	if (!displayOn) {
		command &= ~0x08; // wy³¹cz wyœwietlacz
	}

	TM1637_start();
	TM1637_writeByte(command);
	TM1637_stop();
}

// Funkcja koduj¹ca znak na kod segmentowy
uint8_t encodeChar(char c) {
	switch (c) {
		// Cyfry
		case '0': return 0x3F;
		case '1': return 0x06;
		case '2': return 0x5B;
		case '3': return 0x4F;
		case '4': return 0x66;
		case '5': return 0x6D;
		case '6': return 0x7D;
		case '7': return 0x07;
		case '8': return 0x7F;
		case '9': return 0x6F;

		// Litery du¿e
		case 'A': return 0x77;
		case 'C': return 0x39;
		case 'E': return 0x79;
		case 'F': return 0x71;
		case 'G': return 0b00111101;
		case 'H': return 0x76;
		case 'I': return 0x06;
		case 'J': return 0x1E;
		case 'K': return 0x75;
		case 'L': return 0x38;
		case 'N': return 0x37;
		case 'O': return 0x3F;
		case 'P': return 0x73;
		case 'R': return 0b00110001;
		case 'S': return 0x6D;

		//Litery ma³e
		case 'b': return 0x7C;
		case 'c': return 0b01011000;
		case 'd': return 0x5E;
		case 'g': return 0x6F;
		case 'h': return 0b01110100;
		case 'i': return 0b00000100;
		case 'n': return 0b01010100;
		case 'o': return 0b01011100;
		case 'q': return 0x67;
		case 'r': return 0b01010000;
		case 't': return 0b01111000;
		case 'u': return 0b00011100;
		case 'y': return 0x6E;
		
		// Znaki specjalne
		case '-': return 0x40;  // Myœlnik
		case ' ': return 0x00;  // Pusty znak
		case ':': return 0x80;  // Dwukropek
		case '.': return 0b11111111; // Kropka
		case '^': return 0b01100011; // Stopieñ

		default: return 0x00;  // Domyœlnie pusty znak
	}
}

void TM1637_displayText(char *text) {
	uint8_t encoded[4];
	
	// Zamiana ka¿dego znaku na odpowiadaj¹cy kod segmentu
	for (uint8_t i = 0; i < 4; i++) {
		if (text[i] != '\0') {
			encoded[i] = encodeChar(text[i]);
			} else {
			encoded[i] = 0x00;  // Pusty znak, je¿eli jest mniej ni¿ 4 znaki
		}
	}

	TM1637_display(encoded);  // Wyœwietl zakodowane znaki na wyœwietlaczu
}

int main(void) {
	TM1637_DDR |= (1 << CLK) | (1 << DIO);  // CLK i DIO jako wyjœcia
	TM1637_PORT |= (1 << CLK) | (1 << DIO); // Ustawienie wysokiego stanu


	char text[] = "1234";
	char text2[] = "32^C";
	char text3[] = "45rH";
	char text4[] = "8888";

	while (1) {
		TM1637_setBrightness(0, true);
		TM1637_displayText(text);  // Wyœwietl tekst na wyœwietlaczu
		_delay_ms(1000);  // Czekaj 1 sekundê
		
		TM1637_displayText(text4);
		TM1637_setBrightness(2, true);
		_delay_ms(1000);
		
		TM1637_displayText(text2);
		TM1637_setBrightness(4, true);
		_delay_ms(1000);
		
		TM1637_displayText(text3);
		TM1637_setBrightness(6, true);
		_delay_ms(1000);
	}
}
