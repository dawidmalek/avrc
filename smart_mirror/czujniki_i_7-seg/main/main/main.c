#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#define zero		PORTD = 0b00111111;
#define jeden		PORTD = 0b00001100;
#define dwa			PORTD = 0b01110110;
#define trzy		PORTD = 0b01011110;
#define cztery		PORTD = 0b01001101;
#define piec		PORTD = 0b01011011;
#define szesc		PORTD = 0b01111011;
#define siedem		PORTD = 0b00001110;
#define osiem		PORTD = 0b01111111;
#define dziewiec	PORTD = 0b01011111;
#define litera_r	PORTD = 0b01100000;
#define litera_H	PORTD = 0b01101101;
#define stopien		PORTD = 0b01000111;
#define litera_C	PORTD = 0b00110011;
#define litera_A	PORTD = 0b01101111;
#define litera_b	PORTD = 0b01111001;
#define pusty		PORTD = 0;

#define Seg1		PORTB = 0b00001110;
#define Seg2		PORTB = 0b00001101;
#define Seg3		PORTB = 0b00001011;
#define Seg4		PORTB = 0b00000111;
#define Segoff		PORTB = 0b00001111;

void wait (void){
	_delay_ms(1);
}

void wyswietlCyfre(uint8_t cyfra) {
	switch (cyfra) {
		case 0: zero; break;
		case 1: jeden; break;
		case 2: dwa; break;
		case 3: trzy; break;
		case 4: cztery; break;
		case 5: piec; break;
		case 6: szesc; break;
		case 7: siedem; break;
		case 8: osiem; break;
		case 9: dziewiec; break;
		case 10: litera_A; break;
		case 11: litera_b; break;
		case 12: litera_C; break;
		case 13: litera_r; break;
		case 14: litera_H; break;
		case 15: stopien; break;
	}
}

volatile uint8_t data[9];

volatile uint32_t temp_sum;
volatile float temp;
volatile uint8_t temp_10;
volatile uint8_t temp_1;

volatile uint32_t wilg_sum;
volatile float wilg;
volatile uint8_t wilg_10;
volatile uint8_t wilg_1;

volatile uint32_t cis_sum;
volatile float cis;
volatile uint8_t cis_1000;
volatile uint8_t cis_100;
volatile uint8_t cis_10;
volatile uint8_t cis_1;

volatile uint8_t zmiana = 0;
volatile uint8_t need_sensor_update = 0;

uint16_t dig_T1;
int16_t dig_T2, dig_T3;
uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
int32_t t_fine;

void i2c_start(void){
	TWCR = (1<<TWEN) | (1<<TWSTA) | (1<<TWINT);
	while(!(TWCR & (1<<TWINT)));
}

void i2c_write(uint8_t data){
	TWDR = data;
	TWCR = (1<<TWEN) | (1<<TWINT);
	while(!(TWCR & (1<<TWINT)));
}

uint8_t i2c_read_ack(void){
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}

uint8_t i2c_read_nack(void){
	TWCR = (1<<TWEN) | (1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}

void i2c_stop(void){
	TWCR = (1<<TWEN) | (1<<TWSTO) | (1<<TWINT);
}

void AHT20_init(void){
	i2c_start();
	i2c_write((0x38 << 1) | 0);
	i2c_write(0xBE);
	i2c_write(0xAC);
	i2c_write(0x00);
	i2c_stop();
	_delay_ms(80);
}

void AHT20_pomiar(void){
	i2c_start();
	i2c_write((0x38 << 1) | 0);
	i2c_write(0xAC);
	i2c_write(0x33);
	i2c_write(0x00);
	i2c_stop();
	_delay_ms(80);
}

void AHT20_odczyt(void){
	i2c_start();
	i2c_write((0x38 << 1) | 1);
	for(int i = 0; i < 5; i++){
		data[i] = i2c_read_ack();
	}
	data[5] = i2c_read_nack();
	
	temp_sum = ((uint32_t)(data[3]&0x0F) << 16) | ((uint32_t)data[4] << 8) | ((uint32_t)data[5]);
	temp = ((float)temp_sum/1048576.0)*200.0 - 50.0;
	temp_10 = (uint8_t)(temp / 10);
	temp_1 = (uint8_t)(temp - temp_10 * 10);
	
	wilg_sum = ((uint32_t)(data[1]) << 12) | ((uint32_t)(data[2]) << 4) | ((data[3] & 0xF0) >> 4);
	wilg = ((float)wilg_sum / 1048576.0) * 100.0;
	wilg_10 = (uint8_t)(wilg / 10);
	wilg_1 = (uint8_t)(wilg - wilg_10 * 10);
}

void BMP280_init(void) {
	i2c_start();
	i2c_write((0x77 << 1) | 0);
	i2c_write(0xF4); // ctrl_meas
	i2c_write(0b00100111); // temp x1, press x1, mode normal
	i2c_stop();

	i2c_start();
	i2c_write((0x77 << 1) | 0);
	i2c_write(0xF5); // config
	i2c_write(0x00); // t_sb=0.5ms, filter off, spi3w off
	i2c_stop();
}

void BMP280_read_calib(void) {
	i2c_start();
	i2c_write((0x77 << 1) | 0);
	i2c_write(0x88);
	i2c_start();
	i2c_write((0x77 << 1) | 1);

	uint8_t calib[24];
	for (uint8_t i = 0; i < 23; i++) {
		calib[i] = i2c_read_ack();
	}
	calib[23] = i2c_read_nack();
	i2c_stop();

	dig_T1 = (uint16_t)(calib[1] << 8 | calib[0]);
	dig_T2 = (int16_t)(calib[3] << 8 | calib[2]);
	dig_T3 = (int16_t)(calib[5] << 8 | calib[4]);
	dig_P1 = (uint16_t)(calib[7] << 8 | calib[6]);
	dig_P2 = (int16_t)(calib[9] << 8 | calib[8]);
	dig_P3 = (int16_t)(calib[11] << 8 | calib[10]);
	dig_P4 = (int16_t)(calib[13] << 8 | calib[12]);
	dig_P5 = (int16_t)(calib[15] << 8 | calib[14]);
	dig_P6 = (int16_t)(calib[17] << 8 | calib[16]);
	dig_P7 = (int16_t)(calib[19] << 8 | calib[18]);
	dig_P8 = (int16_t)(calib[21] << 8 | calib[20]);
	dig_P9 = (int16_t)(calib[23] << 8 | calib[22]);
}


void BMP280_odczyt(void) {
	uint8_t buf[6];

	i2c_start();
	i2c_write((0x77 << 1) | 0);
	i2c_write(0xF7); // start od PRESS_MSB
	i2c_start();
	i2c_write((0x77 << 1) | 1);

	for (uint8_t i = 0; i < 5; i++) {
		buf[i] = i2c_read_ack();
	}
	buf[5] = i2c_read_nack();
	i2c_stop();

	uint32_t adc_P = ((uint32_t)buf[0] << 12) | ((uint32_t)buf[1] << 4) | (buf[2] >> 4);
	uint32_t adc_T = ((uint32_t)buf[3] << 12) | ((uint32_t)buf[4] << 4) | (buf[5] >> 4);

	// temperatura (potrzebna do t_fine)
	int32_t var1, var2;
	var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
	var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
	t_fine = var1 + var2;

	// kompensacja ciœnienia
	int64_t var1p, var2p, p;
	var1p = ((int64_t)t_fine) - 128000;
	var2p = var1p * var1p * (int64_t)dig_P6;
	var2p = var2p + ((var1p * (int64_t)dig_P5) << 17);
	var2p = var2p + (((int64_t)dig_P4) << 35);
	var1p = ((var1p * var1p * (int64_t)dig_P3) >> 8) + ((var1p * (int64_t)dig_P2) << 12);
	var1p = (((((int64_t)1) << 47) + var1p)) * ((int64_t)dig_P1) >> 33;

	if (var1p == 0) {
		return;
	}

	p = 1048576 - adc_P;
	p = (((p << 31) - var2p) * 3125) / var1p;
	var1p = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	var2p = (((int64_t)dig_P8) * p) >> 19;

	p = ((p + var1p + var2p) >> 8) + (((int64_t)dig_P7) << 4);
	cis = (float)p / 25600.0; // w hPa

	// podzia³ na cyfry
	cis_1000 = (uint8_t)(cis / 1000);
	cis_100 = (uint8_t)((cis - cis_1000 * 1000) / 100);
	cis_10  = (uint8_t)((cis - cis_1000 * 1000 - cis_100 * 100) / 10);
	cis_1   = (uint8_t)(cis - cis_1000 * 1000 - cis_100 * 100 - cis_10 * 10);
}

int main(void)
{
	DDRD = 255;
	DDRB = 255;
	
	//inicjalizacja i2c
	TWSR = 0;
	TWBR = 23;
	
	TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS12) | (1 << CS10);
	TCCR1B &= ~(1 << CS11);
	OCR1A = 3904;
	TIMSK |= (1 << OCIE1A);
	sei();
	
	AHT20_init();
	AHT20_pomiar();
	AHT20_odczyt();
	
	BMP280_read_calib();
	BMP280_init();
	BMP280_odczyt();


	while (1)
	{
		if (need_sensor_update) {
			pusty; Segoff;
			AHT20_pomiar();
			AHT20_odczyt();
			BMP280_odczyt();
			need_sensor_update = 0;
		}
		
		if(zmiana == 0){
			pusty; Seg1; wyswietlCyfre(temp_10); wait(); Segoff;
			pusty; Seg2; wyswietlCyfre(temp_1); wait(); Segoff;
			pusty; Seg3; stopien; wait(); Segoff;
			pusty; Seg4; litera_C; wait(); Segoff;
		} else if(zmiana == 1){
			pusty; Seg1; wyswietlCyfre(wilg_10); wait(); Segoff;
			pusty; Seg2; wyswietlCyfre(wilg_1); wait(); Segoff;
			pusty; Seg3; litera_r; wait(); Segoff;
			pusty; Seg4; litera_H; wait(); Segoff;
		} else {
			pusty; Seg1; wyswietlCyfre(cis_1000); wait(); Segoff;
			pusty; Seg2; wyswietlCyfre(cis_100); wait(); Segoff;
			pusty; Seg3; wyswietlCyfre(cis_10); wait(); Segoff;
			pusty; Seg4; wyswietlCyfre(cis_1); wait(); Segoff;
		}
		
	}
}

ISR(TIMER1_COMPA_vect) {
	if(zmiana < 2){
		zmiana++;
	} else {
		zmiana = 0;
		need_sensor_update = 1;
	}
}