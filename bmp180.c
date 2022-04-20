#include <i2c.h>
#include <delay.h>
#include <math.h>

typedef long int BMP280_S32_t;
typedef unsigned long int BMP280_U32_t;


// переменные калибровки
unsigned int dig_T1=0;
int dig_T2=0;
int dig_T3=0;

unsigned int dig_P1=0;
int dig_P2=0; 
int dig_P3=0; 
int dig_P4=0;
int dig_P5=0;
int dig_P6=0;
int dig_P7=0; 
int dig_P8=0;
int dig_P9=0;


// ========================================================================================================================
// ========================================================================================================================
// BMP280

// промежуточные переменные
BMP280_S32_t t_fine;

#define BMP180_R 0xED
#define BMP180_W 0xEC

// чтение регистра 16 бит
unsigned int Read(char address) {
 
   unsigned int msb=0;
   unsigned int lsb=0;
   unsigned int data;
 
   i2c_start();    // запуск i2c
   i2c_write(BMP180_W);    // передача адреса устройства, режим записи
   i2c_write(address);   // передача адреса памяти 
   i2c_stop();    // остановка i2c
 
   i2c_start();    // запуск i2c
   i2c_write(BMP180_R);    // передача адреса устройства, режим чтения
   msb = i2c_read(1);
   lsb = i2c_read(0);
   i2c_stop();    // остановка i2c
 
   data = (msb << 8) + lsb;
   return data;
}

unsigned int ReadCalib(char address) {
 
   unsigned int msb=0;
   unsigned int lsb=0;
   unsigned int data;
 
   i2c_start();    // запуск i2c
   i2c_write(BMP180_W);    // передача адреса устройства, режим записи
   i2c_write(address);   // передача адреса памяти 
   i2c_stop();    // остановка i2c
 
   i2c_start();    // запуск i2c
   i2c_write(BMP180_R);    // передача адреса устройства, режим чтения
   lsb = i2c_read(1);
   msb = i2c_read(0);
   i2c_stop();    // остановка i2c
 
   data = (msb << 8) + lsb;
   return data;
}

// получить данные для калибровки
void BMP180_Calibration (void) {
	dig_T1 = ReadCalib(0x88);
	dig_T2 = ReadCalib(0x8A); 
	dig_T3 = ReadCalib(0x8C); 

	dig_P1 = ReadCalib(0x8E);
	dig_P2 = ReadCalib(0x90); 
	dig_P3 = ReadCalib(0x92); 
	dig_P4 = ReadCalib(0x94);
	dig_P5 = ReadCalib(0x96);
	dig_P6 = ReadCalib(0x98);
	dig_P7  = ReadCalib(0x9A); 
	dig_P8  = ReadCalib(0x9C);
	dig_P9  = ReadCalib(0x9E);
 
}

void Measure(BMP280_S32_t *adc_T, BMP280_S32_t *adc_P) {
	unsigned char msb=0;
	unsigned char lsb=0;
	unsigned char xlsb=0;
	i2c_start();    // запуск i2c
	i2c_write(BMP180_W);    // передача адреса устройства, режим записи
	i2c_write(0xF4);   // передача адреса памяти
	i2c_write(0xFD);   // передача разрешения (oss) адреса памяти температуры
	i2c_stop();    // остановка i2c
	
	delay_ms(100); 
	
	i2c_start();    // запуск i2c
	i2c_write(BMP180_W);    // передача адреса устройства, режим записи
	i2c_write(0xF7);   // передача адреса памяти 
	i2c_stop();    // остановка i2c

	i2c_start();    // запуск i2c
	i2c_write(BMP180_R);    // передача адреса устройства, режим чтения
	msb = i2c_read(1);
	lsb = i2c_read(1);
	xlsb = i2c_read(1);
	*adc_P = msb;
	*adc_P = (*adc_P << 8) | lsb;
	*adc_P = (*adc_P << 4) | (xlsb >> 4);
	
	msb = i2c_read(1);
	lsb = i2c_read(1);
	xlsb = i2c_read(0);
	*adc_T = msb;
	*adc_T = (*adc_T << 8) | lsb;
	*adc_T = (*adc_T << 4) | (xlsb >> 4);
	i2c_stop();    // остановка i2c
}

float bmp280_compensate_T_double(BMP280_S32_t adc_T) 
{ 
	float var1, var2, T; 
	var1  = (((float)adc_T)/16384.0 - ((float)dig_T1)/1024.0) * ((float)dig_T2); 
	var2  = ((((float)adc_T)/131072.0 - ((float)dig_T1)/8192.0) * 
	(((float)adc_T)/131072.0 - ((float) dig_T1)/8192.0)) * ((float)dig_T3); 
	t_fine = (BMP280_S32_t)(var1 + var2); 
	T  = (var1 + var2) / 5120.0; 
	return T; 
}

float bmp280_compensate_P_double(BMP280_S32_t adc_P) 
{ 
	float var1, var2, p; 
	var1 = ((float)t_fine/2.0) - 64000.0; 
	var2 = var1 * var1 * ((float)dig_P6) / 32768.0; 
	var2 = var2 + var1 * ((float)dig_P5) * 2.0; 
	var2 = (var2/4.0)+(((float)dig_P4) * 65536.0); 
	var1 = (((float)dig_P3) * var1 * var1 / 524288.0 + ((float)dig_P2) * var1) / 524288.0; 
	var1 = (1.0 + var1 / 32768.0)*((float)dig_P1); 
	if (var1 == 0.0) { 
		return 0; // avoid exception caused by division by zero 
	} 
	p = 1048576.0 - (float)adc_P; 
	p = (p - (var2 / 4096.0)) * 6250.0 / var1; 
	var1 = ((float)dig_P9) * p * p / 2147483648.0; 
	var2 = p * ((float)dig_P8) / 32768.0; 
	p = 	p -566+ (var1 + var2 + ((float)dig_P7)) / 16.0; 
	return p; 
}