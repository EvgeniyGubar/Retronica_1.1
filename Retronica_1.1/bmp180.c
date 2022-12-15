/*
 * bmp180.c
 *
 * Created: 10.12.2022 12:26:07
 *  Author: Evgeniy
 */ 

#include "bmp180.h"
#include "TWI.h"

//#define OSS			3		//самый точный режим датчика (8 измерений с усреднением)
#define BMP180_R	0xEF	//команда на чтение
#define BMP180_W	0xEE	//команда на запись

// переменные калибровки
int16_t ac1;
int16_t ac2;
int16_t ac3;
uint16_t ac4;
uint16_t ac5;
uint16_t ac6;
int16_t b1;
int16_t b2;
int16_t mb;
int16_t mc;
int16_t md;

//–егистры данных
uint32_t MSB;
uint32_t LSB;
uint32_t XSB;

// ѕеременные дл€ расчета
int32_t x1, x2, x3, b5, b6, b3, p;
uint32_t b4, b7;

void BMP180_read_N_byte(uint8_t address, uint8_t amount, uint8_t * byte);

//static uint8_t* BMP180_read_N_byte(uint8_t address, uint8_t amount);
void BMP180_start_conversion(uint8_t param);
uint32_t BMP180_calc_temp(void);
void BMP180_start_press_conversion();
uint32_t BMP180_calc_press(void);

/************************************************************************
 * ‘ункци€ конечного автомата. 
 * ¬озращает значение в мс, сколько нужно ждать до следующего обращени€
 * либо 0, если данные готовы
 * ѕараметры: param 0 - нужна температура, 1 - нужно давление
 * result указатель на результат (температура либо давление), если данные готовы
 * иначе 0, если нужно еще ждать т.е. return != 0
 ************************************************************************/
uint8_t BMP180_get_temp_press(uint8_t param, uint32_t* result)
{
	static uint8_t state = 0; 
	//uint8_t delay = 0;
	
	switch (state){
		case 0: {						//запускаем преобразование температуры и возвращаем 5 (мс врем€ ожидани€)
			BMP180_start_conversion(0); 
			state = 1;
			*result = 0;
			return 5;
		}	break;
		case 1: {	
			if (param == 0){
				state = 0;
				*result = BMP180_calc_temp();
				return 0;
			}
			else{
				BMP180_start_conversion(1);
				state = 2;
				*result = 0;
				return 26;
			}
		}	break;
		case 2:	{
			state = 0;
			BMP180_calc_temp();
			*result = BMP180_calc_press();
			return 0;
		}
	}	
	return 0; 
}

/************************************************************************/
/*	«апустить оцифровку			                                        */
/************************************************************************/
void BMP180_start_conversion(uint8_t param)
{
	i2c_start_cond();               
	i2c_send_byte(BMP180_W);        // передача адреса устройства, режим записи
	i2c_send_byte(0xF4);	        // передача адреса пам€ти
	param ? i2c_send_byte(0xF4) : i2c_send_byte(0x2E);	//если параметр 1, то замер давлени€, иначе температуры
	i2c_stop_cond();                
}

/************************************************************************/
/*	ѕреобразовать данные с датчика в значение температуры.				*/
/*	«апускать через 5мс после BMP180_start_temp_conversion()            */
/************************************************************************/
uint32_t BMP180_calc_temp(void)
{
	int32_t temperature;
	int32_t ut;
	uint8_t	temp[2] = {0};
	
	BMP180_read_N_byte(0xF6, 2, temp);    // получить данные о температуре
	ut = (temp[0] << 8) + temp[1];
		
	x1 = ((int32_t)ut - (int32_t)ac6) * (int32_t)ac5 >> 15;
	x2 = ((int32_t)mc << 11) / (x1 + md);
	b5 = x1 + x2;
	temperature = (b5 + 8) >> 4;
	
	return temperature;
}
/************************************************************************/
/*	ѕреобразовать данные с датчика в значение давлени€.					*/
/*	«апускать через 26мс после BMP180_start_temp_conversion()           */
/************************************************************************/
uint32_t BMP180_calc_press(void)
{		
	int32_t pressure;
	int32_t up;
	uint8_t temp[3] = {0};
	
	BMP180_read_N_byte(0xF6,3, temp);

	up = (((uint32_t)temp[0] << 16) + (temp[1] << 8) + temp[2]) >> 5;  // получить 
		
	//b6 = b5 - 4000;
	//x1 = (b2 * ((b6 * b6) >> 12)) >> 11;
	//x2 = (ac2 * b6) >> 11;
	//x3 = x1 + x2;
	//b3 = (((((int32_t) ac1) * 4 + x3)<<3) + 2)>> 2;
	//x1 = (ac3 * b6) >> 13;
	//x2 = (b1 * ((b6 * b6) >> 12)) >> 16;
	//x3 = ((x1 + x2) + 2) >> 2;
	//b4 = (ac4 * (uint32_t) (x3 + 32768)) >> 15;
	//b7 = ((uint32_t) (up - b3) * (50000 >> 3));
		//
	//if (b7 < 0x80000000)
		//p = (b7 << 1) / b4;
	//else
		//p = (b7 / b4) << 1;
	//x1 = (p >> 8) * (p >> 8);
	//x1 = (x1 * 3038) >> 16;
	//x2 = (-7357 * p) >> 16;
	//pressure = p + ((x1 + x2 + 3791) >> 4);
	//pressure = pressure / 133.3;
		
	return pressure;	
}

/************************************************************************/
/*	ѕолучает данные калибровки из пам€ти датчика		                */
/************************************************************************/
void BMP180_init(void)
{
	uint8_t temp[22] = {0};
	BMP180_read_N_byte(0xAA, 22, temp);
	ac1 = (temp[0] << 8) | (temp[1]);
	ac2 = (temp[2] << 8) | (temp[3]);
	ac3 = (temp[4] << 8) | (temp[5]);
	ac4 = (temp[6] << 8) | (temp[7]);
	ac5 = (temp[8] << 8) | (temp[9]);
	ac6 = (temp[10] << 8) | (temp[11]);
	b1  = (temp[12] << 8) | (temp[13]);
	b2  = (temp[14] << 8) | (temp[15]);
	mb  = (temp[16] << 8) | (temp[17]);
	mc  = (temp[18] << 8) | (temp[19]);
	md  = (temp[20] << 8) | (temp[21]);
}


/************************************************************************/
/*	‘ункци€ чтени€ N байт данных по address, возращает указатель на MSB */
/************************************************************************/ 
void BMP180_read_N_byte(uint8_t address, uint8_t amount, uint8_t * byte)
{
	i2c_start_cond();               
	i2c_send_byte(BMP180_W);        // передача адреса устройства, режим записи
	i2c_send_byte(address);	        // передача адреса пам€ти
	i2c_stop_cond();               
	
	i2c_start_cond();               
	i2c_send_byte(BMP180_R);        // передача адреса устройства, режим чтени€
	for (uint8_t i = 0; i < amount; ++i)
	{
		if (i == (amount-1))	byte[i] = i2c_get_byte(0); 
		else byte[i] = i2c_get_byte(1);		
	}
	i2c_stop_cond();               
}
