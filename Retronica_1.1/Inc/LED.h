/*
 * led_setting.h
 *
 * Created: 23.01.2022 20:00:43
 *  Author: Evgeniy
 */ 


#ifndef LED_H_
#define LED_H_

#include <avr/eeprom.h>
#include "SysTick.h"
#include "ws2812.h"
#include "MenuSettings.h"
#include "EEPROM.h"

extern uint16_t color_eeprom;	//адрес в памяти EEPROM
uint8_t flag_start_led;			//флаг для запуска подсветки
uint16_t color_angle;			//угол цвета

void led_delay(void);		//Неблокирующая задержка между процессом вывода цвета    void led_processing(void);	//Процесс вывода цвета на светодиоды		void led_config(void);		//Конфигурация подсветки       void led_color_stop(void);	//Сохранение выбранного цвета в память и его фиксация void led_color_run(void);	//Запуск смены цвета и удаление из eeprom


#endif /* LED_H_ */