/*
 * main.h
 *
 * Created: 24.01.2022 8:51:37
 *  Author: User
 */ 
#ifndef MAIN_H_
#define MAIN_H_

#include <avr/wdt.h>
#include "MicroMenu.h"    //���������� ��������� ���� (� �������)#include "Buzzer.h"#include "Buttons.h"#include "Indication.h"#include "MenuSettings.h"#include "EEPROM.h"#include "SysTick.h"#include "DS18B20.h"      //������ ����������� 1-wire ds18b20#include "LED.h"static volatile uint16_t delay_for_exit;	//������� �� ��� ������ �� ���� ��������
static volatile uint16_t delay_animation;
static volatile uint16_t therm_ready_delay;
static uint16_t temp_data;			//�������� ������ ����������� 0b 01010101 00010001  - ������ ���� ����� �����, ������ ���� ���������� 85.17
static uint8_t start_program_flag;
static uint8_t single_animation_flag;static uint8_t therm_ready_flag;uint16_t correct_us;	//��������� � ��� �� ������� �������
uint32_t total_us;	//������� ���������������� ����������� 
uint8_t start_correct_flag;	//����� �� ������������� �� 1 ������� � time_manager()//static uint8_t full_animation_flag;static uint8_t buzzer_flag;			//���� ������� �������uint8_t flag_settings_delay;	//���� ��������� �������� ��� ������ �� ��������uint8_t mcusr_mirror __attribute__ ((section (".noinit")));void get_mcusr(void) __attribute__((naked)) __attribute__((section(".init3")));
static void single_animation_start(void);static void single_animation(void);//static void full_animation_start(void);//static void full_animation(void);static void button_processing(void);void time_manager(void);

static void start_delay(void);		//������ �������� � 5 ������ ��� ������ �� ������ ���������  
static void stop_delay(void);		//�������� ������ �� �������� 

static void ports_init(void);

/************************************************************************/
/*					   ������� ������� ����                             */
/************************************************************************//*������� ����                    Encoder+     Encoder-     Enter          Enter        
                     ���          ���������    ����������   ���������      �������       */       
		MENU_ITEM(   Main_menu,   NULL_MENU,   NULL_MENU,   NULL_MENU,     show_main);
		
//���� �������� ����� ����� �� ������ enter �� �������� �������		
		MENU_ITEM(   am_pm,       am_pm,       am_pm,       hours,         set_am_pm);    //��������� �������, ����, ����
		MENU_ITEM(   hours,       hours,       hours,       minuts,        set_hours);    //��������� �������, ����, ����
		MENU_ITEM(   minuts,      minuts,      minuts,      day,           set_minuts);
		MENU_ITEM(   day,         day,         day,         month,         set_day);
		MENU_ITEM(   month,       month,       month,       year,          set_month);
		MENU_ITEM(   year,        year,        year,        show_data,     set_year);		MENU_ITEM(   show_data,   show_data,   show_data,   show_temp,     show_data_on);   //����������� ���� 1-�������� 0-������		MENU_ITEM(   show_temp,   show_temp,   show_temp,   correct,       show_temp_on);   //����������� ����������� 1-�������� 0-������
		MENU_ITEM(   correct,     correct,     correct,     Main_menu,     correction);     //��������� ��������� ������� �/����  
				                        
//��������� ���������� �� ������ minus �� �������� �������								
		MENU_ITEM(   num_alarm,   num_alarm,   num_alarm,   activation,    number_al);
  		MENU_ITEM(   activation,  activation,  activation,  s_hours,       activ_alarm);
 		MENU_ITEM(   s_hours,     s_hours,     s_hours,     s_minuts,      hour_alarm);
 		MENU_ITEM(   s_minuts,    s_minuts,    s_minuts,    Main_menu,     min_alarm);

//��������� ��������� �� ������ plus �� �������� �������		 
		MENU_ITEM(   mode_light,  mode_light,  mode_light,  time_light,    mode_led);  
		MENU_ITEM(   time_light,  time_light,  time_light,  level_bright,  speed_led);
		MENU_ITEM(   level_bright,level_bright,level_bright,Main_menu,     bright_led);  

#endif /* MAIN_H_ */


