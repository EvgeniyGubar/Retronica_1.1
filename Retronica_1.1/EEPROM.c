/*
 * EEPROM.c
 *
 * Created: 23.01.2022 0:02:02
 *  Author: Evgeniy
 */ 
#include "EEPROM.h"
uint16_t color_eeprom EEMEM = 0;static uint16_t key_eeprom EEMEM = 0;static uint8_t correction_eeprom EEMEM = 0;static uint8_t setting_eeprom[18] EEMEM = {};

/************************************************************************/
/*			„тение настроек из EEPROM, если пам€ть чиста€,				*/
/*					то запишет стандартные настройки					*/
/************************************************************************/
void read_eeprom(void)
{	
	uint16_t control_key = 0x1234;
	//проверка ключа в eeprom
	if (control_key != eeprom_read_word(& key_eeprom))  //если не совпал, то записать исходные данные
	{
		PORTB&=0xf0;      //отключаем аноды
		PORTD&=0xf0;      //отключаем катоды
	
		for (uint8_t i=0;i<4;i++)
		{
			eeprom_write_byte(& setting_eeprom[i], 7);     //часы
			eeprom_write_byte(& setting_eeprom[i+4], 30);	//минуты
			eeprom_write_byte(& setting_eeprom[i+8], 0);	//статус
		}
		eeprom_write_byte(& setting_eeprom[12], 0);	//отображение температуры
		eeprom_write_byte(& setting_eeprom[13], 0);	//отображение даты
		eeprom_write_byte(& setting_eeprom[14], 0);	//режим 24/12
		eeprom_write_byte(& setting_eeprom[15], 1);	//режим подсветки
		eeprom_write_byte(& setting_eeprom[16], 1);	//скорость подсветки
		eeprom_write_byte(& setting_eeprom[17], 1);	//€ркость подсветки
		eeprom_write_byte(& correction_eeprom, 0);	//коррекци€ времени
		eeprom_write_word(& color_eeprom, 0);	//угол цвета
		eeprom_write_word((uint16_t *)50, 0);  //€чейка содержащее количество сбросов по watchdog (дл€ тестировки)
		eeprom_write_word(& key_eeprom, control_key);  //после инициализации EEPROM записать ключ
	}
	//чтение данных
	for (uint8_t i=0;i<4;i++)
	{
		Alarm[i].Al_hour = eeprom_read_byte(& setting_eeprom[i]);
		Alarm[i].Al_min = eeprom_read_byte(& setting_eeprom[i+4]);
		Alarm[i].Al_status = eeprom_read_byte(& setting_eeprom[i+8]);
	}
	Time.temp_on = eeprom_read_byte(& setting_eeprom[12]);
	Time.data_on = eeprom_read_byte(& setting_eeprom[13]);
	Time.am_pm = eeprom_read_byte(& setting_eeprom[14]);
	LED.mode = eeprom_read_byte(& setting_eeprom[15]);
	LED.speed = eeprom_read_byte(& setting_eeprom[16]);
	LED.bright = eeprom_read_byte(& setting_eeprom[17]);
	Time.Correct_sec = (int8_t) eeprom_read_byte(& correction_eeprom);
	color_angle=eeprom_read_word(& color_eeprom);	
}
/************************************************************************/
/*			—охранение изменненых настроек в EEPROM                     */
/************************************************************************/
void save_to_eeprom(void)
{
	PORTB&=0xf0;      //отключаем аноды
	PORTD&=0xf0;      //отключаем катоды
	
	for (uint8_t i=0;i<4;i++)
	{
		if (Alarm[i].Al_hour != eeprom_read_byte(& setting_eeprom[i]))		eeprom_write_byte(& setting_eeprom[i], Alarm[i].Al_hour);
		if (Alarm[i].Al_min != eeprom_read_byte(& setting_eeprom[i+4]))	eeprom_write_byte(& setting_eeprom[i+4], Alarm[i].Al_min);
		if (Alarm[i].Al_status != eeprom_read_byte(& setting_eeprom[i+8])) eeprom_write_byte(& setting_eeprom[i+8], Alarm[i].Al_status);
	}
	if (Time.temp_on != eeprom_read_byte(& setting_eeprom[12]))	eeprom_write_byte(& setting_eeprom[12], Time.temp_on);
	if (Time.data_on != eeprom_read_byte(& setting_eeprom[13]))	eeprom_write_byte(& setting_eeprom[13], Time.data_on);
	if (Time.am_pm != eeprom_read_byte(& setting_eeprom[14]))		eeprom_write_byte(& setting_eeprom[14], Time.am_pm);
	if (LED.mode != eeprom_read_byte(& setting_eeprom[15]))		eeprom_write_byte(& setting_eeprom[15], LED.mode);
	if (LED.speed != eeprom_read_byte(& setting_eeprom[16]))		eeprom_write_byte(& setting_eeprom[16], LED.speed);
	if (LED.bright != eeprom_read_byte(& setting_eeprom[17]))		eeprom_write_byte(& setting_eeprom[17], LED.bright);
	if (Time.Correct_sec != (int8_t)eeprom_read_byte(& correction_eeprom))	eeprom_write_byte(& correction_eeprom, (uint8_t)Time.Correct_sec);
}



