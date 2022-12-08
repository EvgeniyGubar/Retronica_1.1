/*
 * MENUSETTINGS.h
 *
 * Created: 18.05.2022 10:34:10
 *  Author: User
 */ 


#ifndef MENUSETTINGS_H_
#define MENUSETTINGS_H_

#define DIVIDING_POINT_ON  PORTC|=(1<<PC0)     //���-1 - �������������� �����
#define DIVIDING_POINT_OFF PORTC&=~(1<<PC0)

#include <stdlib.h>  //��� abs �������
#include "RTC.h"
#include "LED.h"
#include "Indication.h"
#include "EEPROM.h"

extern void time_manager(void);
extern uint8_t flag_settings_delay;		//���� ��������� �������� ��� ������ �� ��������
extern uint16_t correct_us;	//��������� � ��� �� ������� �������
extern uint32_t total_us;	//������� ���������������� ����������� 
uint8_t time_change_flag;  //���� �������������� ������� � ����������uint8_t alarm_number;      //����� ���������� (�� 1 �� 4�)uint8_t selectable_alarm_point;        //������ � ������ ��������� ����������, ��� �������������� 4� �����������, ����� ���������� ����� � ������ ���������� �� ������uint8_t no_points_global;  //�� ���������� ����� ����������� � ������� ������ ��������� ������ � ����������

 int8_t i;             //���������/��������� ���������� �� ������ ��� ��������� ����������

//��������� �����
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
	uint8_t     am_pm;
	 int8_t		Correct_sec;
} Time;

//��������� �����������
struct
{
	uint8_t		Al_hour;
	uint8_t		Al_min;
	uint8_t		Al_status;
} Alarm[4];	

//��������� ���������
struct
{
	uint8_t  mode;
	uint8_t  speed;
	uint8_t  bright;
} LED;

//����� ������� ������
enum ModeStatus
{
	STATUS_MAIN,
	STATUS_LED,
	STATUS_ALARM,
	STATUS_SETTINGS
};

enum ModeStatus Status;

void show_main(void);

void set_am_pm(void);void set_hours(void);void set_minuts(void);void set_day(void);void set_month(void);void set_year(void);void show_data_on(void);void show_temp_on(void);void correction(void);

void number_al(void);	//����� ��� ������ �� ��������� �����������void activ_alarm(void);	//���������/���������� ���������� ����������void hour_alarm(void);	//��������� ����� ����������void min_alarm(void);	//��������� ����� ����������

void mode_led(void);	  //����� ���� 1
void speed_led(void);	  //����� ���� 2
void bright_led(void);	  //����� ���� 3

#endif /* MENUSETTINGS_H_ */