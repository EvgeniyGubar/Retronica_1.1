/*
 * main.h
 *
 * Created: 24.01.2022 8:51:37
 *  Author: User
 */ 
#ifndef MAIN_H_
#define MAIN_H_

#include <avr/wdt.h>
#include "MicroMenu.h"    //библиотека обработки меню (с гитхаба)#include "Buzzer.h"#include "Buttons.h"#include "Indication.h"#include "MenuSettings.h"#include "EEPROM.h"#include "SysTick.h"#include "LED.h"#include "bmp180.h"static volatile uint16_t delay_for_exit;	//счетчик мс для выхода из меню настроек
static volatile uint16_t delay_animation;
static volatile uint16_t bmp180_save_ticks;
static volatile uint8_t bmp180_delay;
//static uint16_t temp_data;			//получает данные температуры 0b 01010101 00010001  - первый байт целая часть, второй байт десятичная 85.17
static uint8_t start_program_flag;
static uint8_t single_animation_flag;static uint8_t get_temp_press_flag;uint16_t correct_us;	//коррекция в мкс на секунду времени
uint32_t total_us;	//счетчик корректировочных микросекунд 
uint8_t start_correct_flag;	//старт на корректировку на 1 секунду в time_manager()//static uint8_t full_animation_flag;static uint8_t buzzer_flag;			//флаг запуска зуммераuint8_t flag_settings_delay;	//флаг включения задержки для выхода из настроекuint8_t mcusr_mirror __attribute__ ((section (".noinit")));void get_mcusr(void) __attribute__((naked)) __attribute__((section(".init3")));
static void single_animation_start(void);static void single_animation(void);//static void full_animation_start(void);//static void full_animation(void);static void button_processing(void);void time_manager(void);

static void start_delay(void);		//Запуск задержки в 5 секунд для выхода из режимы настройки  
static void stop_delay(void);		//Задержка выхода из настроек 

static void ports_init(void);

/************************************************************************/
/*					   Макросы пунктов меню                             */
/************************************************************************//*ГЛАВНОЕ МЕНЮ                    Encoder+     Encoder-     Enter          Enter        
                     Имя          Следующий    Предыдущий   Вложенное      Функция       */       
		MENU_ITEM(   Main_menu,   NULL_MENU,   NULL_MENU,   NULL_MENU,     show_main);
		
//Меню настроек часов лежит на кнопке enter по длинному нажатию		
		MENU_ITEM(   am_pm,       am_pm,       am_pm,       hours,         set_am_pm);    //установка времени, даты, года
		MENU_ITEM(   hours,       hours,       hours,       minuts,        set_hours);    //установка времени, даты, года
		MENU_ITEM(   minuts,      minuts,      minuts,      day,           set_minuts);
		MENU_ITEM(   day,         day,         day,         month,         set_day);
		MENU_ITEM(   month,       month,       month,       year,          set_month);
		MENU_ITEM(   year,        year,        year,        show_data,     set_year);		MENU_ITEM(   show_data,   show_data,   show_data,   show_temp,     show_data_on);   //отображение даты 1-показать 0-скрыть		MENU_ITEM(   show_temp,   show_temp,   show_temp,   show_press,    show_temp_on);   //отображение температуры 1-показать 0-скрыть
		MENU_ITEM(   show_press,  show_press,  show_press,  correct,       show_press_on);   //отображение температуры 1-показать 0-скрыть
		MENU_ITEM(   correct,     correct,     correct,     Main_menu,     correction);     //установка коррекции времени с/день  
				                        
//Настройка будильника на кнопке minus по длинному нажатию								
		MENU_ITEM(   num_alarm,   num_alarm,   num_alarm,   activation,    number_al);
  		MENU_ITEM(   activation,  activation,  activation,  s_hours,       activ_alarm);
 		MENU_ITEM(   s_hours,     s_hours,     s_hours,     s_minuts,      hour_alarm);
 		MENU_ITEM(   s_minuts,    s_minuts,    s_minuts,    Main_menu,     min_alarm);

//Настройка подсветки на кнопке plus по длинному нажатию		 
		MENU_ITEM(   mode_light,  mode_light,  mode_light,  time_light,    mode_led);  
		MENU_ITEM(   time_light,  time_light,  time_light,  level_bright,  speed_led);
		MENU_ITEM(   level_bright,level_bright,level_bright,Main_menu,     bright_led);  

#endif /* MAIN_H_ */


