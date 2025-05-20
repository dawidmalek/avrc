#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>

int main(void)
{
    DDRA = 255;
    while (1) 
    {
		PORTA = 255;
		_delay_ms(1000);
		PORTA = 0;
		_delay_ms(1000);
    }
}

