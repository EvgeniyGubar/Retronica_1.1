/*
 * MENUSETTINGS.h
 *
 * Created: 18.05.2022 10:34:10
 *  Author: User
 */ 


#ifndef MENUSETTINGS_H_
#define MENUSETTINGS_H_

#define DIVIDING_POINT_ON  PORTC|=(1<<PC0)     //ИНС-1 - разделительная точка
#define DIVIDING_POINT_OFF PORTC&=~(1<<PC0)

#include <stdlib.h>  //для abs функции
#include "RTC.h"
#include "LED.h"
#include "Indication.h"
#include "EEPROM.h"

extern void time_manager(void);
extern uint8_t flag_settings_delay;		//флаг включения задержки для выхода из настроек
extern uint16_t correct_us;	//коррекция в мкс на секунду времени
extern uint32_t total_us;	//счетчик корректировочных микросекунд 
uint8_t time_change_flag;  //флаг редактирования времени в настройкахuint8_t alarm_number;      //номер будильника (от 1 до 4х)uint8_t selectable_alarm_point;        //только в режиме настройки будильника, при перелистывании 4х будильников, будет переходить точка с одного индикатора на другойuint8_t no_points_global;  //Не показывать точки будильников в режимах показа различных данных и настройках

 int8_t i;             //инкремент/декремент получаемый от кнопок при настройке параметров

//параметры часов
struct
{
	uint8_t		Sec;
	uint8_t		Hour;
	uint8_t		Min;
	uint8_t		Month;
	uint8_t		Day;
	uint8_t		Year;
	uint8_t     data_on;
	uint8_t     temp_on;
	uint8_t		press_on;
	uint8_t     am_pm;
	 int8_t		Correct_sec;
} Time;

//параметры будильников
struct
{
	uint8_t		Al_hour;
	uint8_t		Al_min;
	uint8_t		Al_status;
} Alarm[4];	

//параметры подсветки
struct
{
	uint8_t  mode;
	uint8_t  speed;
	uint8_t  bright;
} LED;

//флаги режимов работы
enum ModeStatus
{
	STATUS_MAIN,
	STATUS_LED,
	STATUS_ALARM,
	STATUS_SETTINGS
};

enum ModeStatus Status;

void show_main(void);

void set_am_pm(void);void set_hours(void);void set_minuts(void);void set_day(void);void set_month(void);void set_year(void);void show_data_on(void);void show_temp_on(void);void show_press_on(void);void correction(void);

void number_al(void);	//Далее ряд фукций по настройке будильниковvoid activ_alarm(void);	//Включение/выключение выбранного будильникаvoid hour_alarm(void);	//Установка часов будильникаvoid min_alarm(void);	//Установка минут будильника

void mode_led(void);	  //пункт меню 1
void speed_led(void);	  //пункт меню 2
void bright_led(void);	  //пункт меню 3

#endif /* MENUSETTINGS_H_ */