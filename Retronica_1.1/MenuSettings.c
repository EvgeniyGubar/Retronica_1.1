/*
 * Menu_items.c
 *
 * Created: 18.05.2022 10:36:41
 *  Author: User
 */ 

#include "MenuSettings.h"

static void constrain(int8_t *param, int8_t min, int8_t max);//���������� ����������� �������� ���������, ������������ min � max

/************************************************************************/
/*        ������� ������ � �������� ����� - ����������� �������         */
/************************************************************************/void show_main(void){
	if (time_change_flag)    //���� ���� ���������
	{
		RTC_SetValue(RTC_SQWE_ADR, 0x12);	//���������� ������ � �������� 1��
		RTC_SetValue(RTC_SEC_ADR, 00);	//�������� 0 � ��������� ����
		time_change_flag = 0;
		total_us = 0;	//�������� ������� ���������������� ����������� 
	}
	Ind[0].blink=Ind[1].blink=Ind[2].blink=Ind[3].blink=0;  
	flag_settings_delay = 0;	
	selectable_alarm_point = 0;	//��������� ����� �� �����������. ���� Al_status �� ����� ����, �� ��� ����� ����� ������ �����.	no_points_global = 0;		//�������� ����� �� �����������	correct_us = (uint16_t)abs(Time.Correct_sec*10000000/86400);  //���������	Status = STATUS_MAIN;	save_to_eeprom();			//���� ���� �����-�� ��������� � ����������, �� ��� ���������� � EEPROM	time_manager();}




/************************************************************************/
/*                     ���������� ����� �����������                     */
/************************************************************************/void set_am_pm(void){			Status = STATUS_SETTINGS;		no_points_global = 1;  //������ ����� � �����������		if (i != 0)	Time.am_pm^=(0x01); //�������� ���� �������		Time.Hour=RTC_get(RTC_HOUR_ADR);		DIVIDING_POINT_OFF;  //����� �����	Ind[0].blink=Ind[1].blink=1;   //������� ������
 	if (Time.am_pm==0)
 	{
 		Ind[0].lamp = 2;
 		Ind[1].lamp = 4;
 	}
 	else
 	{
 		Ind[0].lamp = 1;
 		Ind[1].lamp = 2;
 	}
 	Ind[2].lamp = Ind[3].lamp = 10;
}
/************************************************************************/
/*                      ���������� ����                                 */
/************************************************************************/void set_hours(void){	Time.Hour=RTC_get(RTC_HOUR_ADR);	Time.Min=RTC_get(RTC_MIN_ADR);		Time.Hour = ((Time.Hour>>4)*10) + (Time.Hour&0x0F);		if (i!=0)
	{
		time_change_flag=1;  //���� �������������� �������
		RTC_SetValue(RTC_SEC_ADR, 0x80); //������������� ����
		Time.Hour+=i;
		constrain((int8_t*)&Time.Hour,0,23);
		RTC_SetValue(RTC_HOUR_ADR, Time.Hour);
	}		DIVIDING_POINT_ON;//�������� �����	
	if (Time.am_pm==0)   //���� 24, �� �������  ��� ����� �����
	{
		Ind[0].blink = Ind[1].blink = 1;         //������� ������
	}
	else                 //���� 12, �� ������� � ����������� �� �� � ��
	{
		if (Time.Hour>11)
		{
			Time.Hour -= 12;
			Ind[0].blink=0;
			Ind[1].blink=1; // ������� 2 ����� - PM
		}		else		{			Ind[0].blink=1; // ������� 1 ����� - �M
			Ind[1].blink=0;		}	}
	Ind[0].lamp=Time.Hour/10;
	Ind[1].lamp=Time.Hour%10;
	Ind[2].lamp=Time.Min>>4;
	Ind[3].lamp=Time.Min&0x0F;}/************************************************************************/
/*                                                                      */
/************************************************************************/void set_minuts(void){	Time.Min=RTC_get(RTC_MIN_ADR);		Time.Min=((Time.Min>>4)*10)+(Time.Min&0x0F);		if (i!=0)
	{
		time_change_flag=1;  //���� �������������� �������
		RTC_SetValue(RTC_SEC_ADR, 0x80);   //������������� ����
		Time.Min+=i;
		constrain((int8_t*)&Time.Min,0,59);
		RTC_SetValue(RTC_MIN_ADR, Time.Min);
	}		if (Time.am_pm && (Time.Hour>11))	Time.Hour -= 12;//���� ����� �� ��
		Ind[0].blink=Ind[1].blink=0;	Ind[2].blink=Ind[3].blink=1; //������� ��������	Ind[0].lamp=Time.Hour/10;
	Ind[1].lamp=Time.Hour%10;
	Ind[2].lamp=Time.Min/10;
	Ind[3].lamp=Time.Min%10;}/************************************************************************/
/*                                                                      */
/************************************************************************/void set_day(void){	if (time_change_flag)  //���� ������������� ����� � ���������� ������
	{
		//RTC_SetValue(RTC_SQWE_ADR, 0x12);//���������� ������ � �������� 1��  �� ������������ ������. ������� ���� �����, �������� �������� ������
		RTC_SetValue(RTC_SEC_ADR, 00);    //���������� ������� � 0 � ���������
		time_change_flag=0;
	}	
	Time.Day=RTC_get(RTC_DAY_ADR);
	Time.Month=RTC_get(RTC_MONTH_ADR);
	Time.Day=((Time.Day>>4)*10)+(Time.Day&0x0F);
	
	if (i!=0)
	{
		Time.Day+=i;
		constrain((int8_t*)&Time.Day,1,31);
		RTC_SetValue(RTC_DAY_ADR, Time.Day);
	}	Ind[0].blink=Ind[1].blink=1;	Ind[2].blink=Ind[3].blink=0;	Ind[0].lamp=Time.Day/10;	Ind[1].lamp=Time.Day%10;	Ind[2].lamp=Time.Month>>4;	Ind[3].lamp=Time.Month&0x0f;}/************************************************************************/
/*                                                                      */
/************************************************************************/void set_month(void){
	Time.Month=RTC_get(RTC_MONTH_ADR);	Time.Month=((Time.Month>>4)*10)+(Time.Month&0x0F);	
	if (i!=0)
	{
		Time.Month+=i;
		constrain((int8_t*)&Time.Month,1,12);
		RTC_SetValue(RTC_MONTH_ADR, Time.Month);
	}	Ind[0].blink=Ind[1].blink=0;	Ind[2].blink=Ind[3].blink=1;	Ind[0].lamp=Time.Day/10;	Ind[1].lamp=Time.Day%10;	Ind[2].lamp=Time.Month/10;	Ind[3].lamp=Time.Month%10;}/************************************************************************/
/*                                                                      */
/************************************************************************/void set_year(void){	Time.Year=RTC_get(RTC_YEAR_ADR);	Time.Year=((Time.Year>>4)*10)+(Time.Year&0x0F);	
	if (i!=0)
	{
		Time.Year+=i;
		constrain((int8_t*)&Time.Year,0,99);
		RTC_SetValue(RTC_YEAR_ADR, Time.Year);
	}	DIVIDING_POINT_OFF; //����� �����	Ind[0].lamp=2;	Ind[1].lamp=0;	Ind[2].lamp=Time.Year/10;	Ind[3].lamp=Time.Year%10;}/************************************************************************/
/*                                                                      */
/************************************************************************/void show_data_on(void){	if (i != 0)	Time.data_on^=(0x01); //�������� ���� �������	Ind[0].lamp=1;	Ind[1].lamp=10;	Ind[2].lamp=10;	Ind[3].lamp=Time.data_on;}/************************************************************************/
/*                                                                      */
/************************************************************************/void show_temp_on(void){	if (i != 0)	Time.temp_on^=(0x01); //�������� ���� �������	Ind[0].lamp=2;	Ind[1].lamp=10;	Ind[2].lamp=10;	Ind[3].lamp=Time.temp_on;}/************************************************************************/
/*                                                                      */
/************************************************************************/void correction(void){	if (i!=0)
	{
		Time.Correct_sec+=i;
		constrain((int8_t*)&Time.Correct_sec,-99,99);
	}		if (Time.Correct_sec<0)	DIVIDING_POINT_ON;	else	DIVIDING_POINT_OFF;		Ind[0].lamp=3;	Ind[1].lamp=10;	Ind[2].lamp=abs(Time.Correct_sec/10);  //������	Ind[3].lamp=abs(Time.Correct_sec%10);  //������}
/************************************************************************/
/*		  	   ����� ��� ������ �� ��������� �����������                */
/************************************************************************/void number_al(void){	DIVIDING_POINT_ON;	Status = STATUS_SETTINGS;	selectable_alarm_point = 1;  //���������� ����� ���������� ����������. ���� ������� ������������ � ����������� ������� ���������	if (i!=0)
	{
		alarm_number+=i;
		constrain((int8_t*)&alarm_number,0,3);
	}	if ((Time.am_pm)&&(Alarm[alarm_number].Al_hour>11))  //���� 24, �� ������ �� 12
	{
		Ind[0].lamp=(Alarm[alarm_number].Al_hour-12)/10;
		Ind[1].lamp=(Alarm[alarm_number].Al_hour-12)%10;
	}
	else	{		Ind[0].lamp=Alarm[alarm_number].Al_hour/10;
		Ind[1].lamp=Alarm[alarm_number].Al_hour%10;	}	
	Ind[2].lamp=Alarm[alarm_number].Al_min/10;
	Ind[3].lamp=Alarm[alarm_number].Al_min%10;
}/************************************************************************/
/*			  ���������/���������� ���������� ����������                */
/************************************************************************/void activ_alarm(void){	DIVIDING_POINT_OFF;	Status = STATUS_ALARM;	selectable_alarm_point = 0;	no_points_global = 1;  //������ ����� � �����������	if (i!=0)	{		Alarm[alarm_number].Al_status^=(0x01); //�������� ���� �������	}	Ind[0].lamp=10;	Ind[1].lamp=10;	Ind[2].lamp=10;	Ind[3].lamp=Alarm[alarm_number].Al_status;}/************************************************************************/
/*				       ��������� ����� ����������                       */
/************************************************************************/void hour_alarm(void){	DIVIDING_POINT_ON;	Status = STATUS_SETTINGS;	if (i!=0)
	{
		Alarm[alarm_number].Al_hour+=i;
		constrain((int8_t*)&Alarm[alarm_number].Al_hour,0,23);
	}	//�������� �� �� ��	if (Time.am_pm==0)   //���� 24, �� �������  ��� ����� �����
	{
		Ind[0].blink=Ind[1].blink=1;         //������� ������
		Ind[0].lamp=Alarm[alarm_number].Al_hour/10;		Ind[1].lamp=Alarm[alarm_number].Al_hour%10;
	}
	else                 //���� 12, �� ������� � ����������� �� �� � ��
	{
		if (Alarm[alarm_number].Al_hour>11)
		{
			Ind[0].blink=0;
			Ind[1].blink=1; // ������� 2 ����� - PM
			Ind[0].lamp=(Alarm[alarm_number].Al_hour-12)/10;			Ind[1].lamp=(Alarm[alarm_number].Al_hour-12)%10;
		}		else		{			Ind[0].blink=1;
			Ind[1].blink=0; // ������� 1 ����� - �M			Ind[0].lamp=Alarm[alarm_number].Al_hour/10;			Ind[1].lamp=Alarm[alarm_number].Al_hour%10;		}	}	Ind[2].lamp=Alarm[alarm_number].Al_min/10;	Ind[3].lamp=Alarm[alarm_number].Al_min%10;}/************************************************************************/
/*				     ��������� ����� ����������                         */
/************************************************************************/void min_alarm(void){	if (i!=0)
	{
		Alarm[alarm_number].Al_min+=i;
		constrain((int8_t*)&Alarm[alarm_number].Al_min,0,59);
	}	Ind[0].blink=Ind[1].blink=0;	Ind[2].blink=Ind[3].blink=1;	if ((Time.am_pm)&&(Alarm[alarm_number].Al_hour>11))  //���� 24, �� ������ �� 12
	{
		Ind[0].lamp=(Alarm[alarm_number].Al_hour-12)/10;
		Ind[1].lamp=(Alarm[alarm_number].Al_hour-12)%10;
	}
	else	{		Ind[0].lamp=Alarm[alarm_number].Al_hour/10;
		Ind[1].lamp=Alarm[alarm_number].Al_hour%10;	}	Ind[2].lamp=Alarm[alarm_number].Al_min/10;
	Ind[3].lamp=Alarm[alarm_number].Al_min%10;}/************************************************************************/
/*						����� ������ ���������                          */
/************************************************************************/void mode_led(void)  //������ ���������. 1-��� ��������, 2-����������, 3-���������
{
	Status = STATUS_LED;
	no_points_global = 1;  //������ ����� � �����������
	DIVIDING_POINT_OFF;
	if (i|=0)
	{
 		if ((LED.mode != 1)&&(flag_start_led == 0))	led_color_run();	//���� ���� ��� ����������, �� ���������
		LED.mode+=i;
		constrain((int8_t*)&LED.mode,1,3);
		led_config();	//���������������� ���������
	}
	Ind[3].blink=1;
	Ind[0].lamp=1;
	Ind[1].lamp=10;
	Ind[2].lamp=10;
	Ind[3].lamp=LED.mode;
}
/************************************************************************/
/*				�������� ������� ���������� �����                       */
/************************************************************************/
void speed_led(void)  //������ ������ ����� ������
{
	Status = STATUS_SETTINGS;
	if (i|=0)
	{
		LED.speed+=i;
		constrain((int8_t*)&LED.speed,1,4);
		led_config();	//���������������� ���������
	}
	Ind[0].lamp=2;
	Ind[1].lamp=10;
	Ind[2].lamp=10;
	Ind[3].lamp=LED.speed;	
}
/************************************************************************/
/*					��������� ������� ���������                         */
/************************************************************************/
void bright_led(void)
{
	if (i|=0)
	{
		LED.bright+=i;
		constrain((int8_t*)&LED.bright,1,4);
		led_config();	//���������������� ���������
	}
	Ind[0].lamp=3;
	Ind[1].lamp=10;
	Ind[2].lamp=10;
	Ind[3].lamp=LED.bright;	
}




/************************************************************************/
/*	���������� ����������� �������� ���������, ������������ min � max   */
/************************************************************************/
static void constrain(int8_t *parameter, int8_t min, int8_t max)
{
	if (*parameter<min)
	{
		*parameter=max;
		return;
	}
	if (*parameter>max)
	{
		*parameter=min;
		return;
	}
	return;
}