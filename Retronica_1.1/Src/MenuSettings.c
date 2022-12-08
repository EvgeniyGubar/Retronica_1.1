/*
 * Menu_items.c
 *
 * Created: 18.05.2022 10:36:41
 *  Author: User
 */ 

#include "MenuSettings.h"

static void constrain(int8_t *param, int8_t min, int8_t max);//возвращает зациклинное значение параметра, ограниченное min и max

/************************************************************************/
/*        Функция выхода в основной режим - отображение времени         */
/************************************************************************/void show_main(void){
	if (time_change_flag)    //если были настройки
	{
		RTC_SetValue(RTC_SQWE_ADR, 0x12);	//разрешение мендра с частотой 1Гц
		RTC_SetValue(RTC_SEC_ADR, 00);	//записать 0 и запустить счет
		time_change_flag = 0;
		total_us = 0;	//обнулить счетчик корректировочных микросекунд 
	}
	Ind[0].blink=Ind[1].blink=Ind[2].blink=Ind[3].blink=0;  
	flag_settings_delay = 0;	
	selectable_alarm_point = 0;	//отключаем точки на будильниках. Если Al_status не равен нулю, то все равно точка гореть будет.	no_points_global = 0;		//показать точки на будильниках	correct_us = (uint16_t)abs(Time.Correct_sec*10000000/86400);  //константа	Status = STATUS_MAIN;	save_to_eeprom();			//если были какие-то изменения в настройках, то они сохранятся в EEPROM	time_manager();}




/************************************************************************/
/*                     Установить режим отображения                     */
/************************************************************************/void set_am_pm(void){			Status = STATUS_SETTINGS;		no_points_global = 1;  //убрать точки с будильников		if (i != 0)	Time.am_pm^=(0x01); //инверсия бита статуса		Time.Hour=RTC_get(RTC_HOUR_ADR);		DIVIDING_POINT_OFF;  //гасим точку	Ind[0].blink=Ind[1].blink=1;   //моргаем часами
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
/*                      Установить часы                                 */
/************************************************************************/void set_hours(void){	Time.Hour=RTC_get(RTC_HOUR_ADR);	Time.Min=RTC_get(RTC_MIN_ADR);		Time.Hour = ((Time.Hour>>4)*10) + (Time.Hour&0x0F);		if (i!=0)
	{
		time_change_flag=1;  //флаг редактирования времени
		RTC_SetValue(RTC_SEC_ADR, 0x80); //останавливаем счет
		Time.Hour+=i;
		constrain((int8_t*)&Time.Hour,0,23);
		RTC_SetValue(RTC_HOUR_ADR, Time.Hour);
	}		DIVIDING_POINT_ON;//включаем точку	
	if (Time.am_pm==0)   //если 24, то моргаем  обе цифры часов
	{
		Ind[0].blink = Ind[1].blink = 1;         //моргаем часами
	}
	else                 //если 12, то моргаем в зависимости от АМ и РМ
	{
		if (Time.Hour>11)
		{
			Time.Hour -= 12;
			Ind[0].blink=0;
			Ind[1].blink=1; // моргает 2 цифра - PM
		}		else		{			Ind[0].blink=1; // моргает 1 цифра - АM
			Ind[1].blink=0;		}	}
	Ind[0].lamp=Time.Hour/10;
	Ind[1].lamp=Time.Hour%10;
	Ind[2].lamp=Time.Min>>4;
	Ind[3].lamp=Time.Min&0x0F;}/************************************************************************/
/*                                                                      */
/************************************************************************/void set_minuts(void){	Time.Min=RTC_get(RTC_MIN_ADR);		Time.Min=((Time.Min>>4)*10)+(Time.Min&0x0F);		if (i!=0)
	{
		time_change_flag=1;  //флаг редактирования времени
		RTC_SetValue(RTC_SEC_ADR, 0x80);   //останавливаем счет
		Time.Min+=i;
		constrain((int8_t*)&Time.Min,0,59);
		RTC_SetValue(RTC_MIN_ADR, Time.Min);
	}		if (Time.am_pm && (Time.Hour>11))	Time.Hour -= 12;//если режим АМ РМ
		Ind[0].blink=Ind[1].blink=0;	Ind[2].blink=Ind[3].blink=1; //моргаем минутами	Ind[0].lamp=Time.Hour/10;
	Ind[1].lamp=Time.Hour%10;
	Ind[2].lamp=Time.Min/10;
	Ind[3].lamp=Time.Min%10;}/************************************************************************/
/*                                                                      */
/************************************************************************/void set_day(void){	if (time_change_flag)  //если редактировали время в предыдущем пункте
	{
		//RTC_SetValue(RTC_SQWE_ADR, 0x12);//разрешение мендра с частотой 1Гц  НЕ ОБЯЗАТЕЛЬНАЯ СТРОКА. ГЛЮЧИЛА ОДНА ПЛАТА, ПРИШЛОСЬ ДОБАВИТЬ СТРОКУ
		RTC_SetValue(RTC_SEC_ADR, 00);    //сбрасываем секунды в 0 и запускаем
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
	}	DIVIDING_POINT_OFF; //гасим точку	Ind[0].lamp=2;	Ind[1].lamp=0;	Ind[2].lamp=Time.Year/10;	Ind[3].lamp=Time.Year%10;}/************************************************************************/
/*                                                                      */
/************************************************************************/void show_data_on(void){	if (i != 0)	Time.data_on^=(0x01); //инверсия бита статуса	Ind[0].lamp=1;	Ind[1].lamp=10;	Ind[2].lamp=10;	Ind[3].lamp=Time.data_on;}/************************************************************************/
/*                                                                      */
/************************************************************************/void show_temp_on(void){	if (i != 0)	Time.temp_on^=(0x01); //инверсия бита статуса	Ind[0].lamp=2;	Ind[1].lamp=10;	Ind[2].lamp=10;	Ind[3].lamp=Time.temp_on;}/************************************************************************/
/*                                                                      */
/************************************************************************/void correction(void){	if (i!=0)
	{
		Time.Correct_sec+=i;
		constrain((int8_t*)&Time.Correct_sec,-99,99);
	}		if (Time.Correct_sec<0)	DIVIDING_POINT_ON;	else	DIVIDING_POINT_OFF;		Ind[0].lamp=3;	Ind[1].lamp=10;	Ind[2].lamp=abs(Time.Correct_sec/10);  //модуль	Ind[3].lamp=abs(Time.Correct_sec%10);  //модуль}
/************************************************************************/
/*		  	   Далее ряд фукций по настройке будильников                */
/************************************************************************/void number_al(void){	DIVIDING_POINT_ON;	Status = STATUS_SETTINGS;	selectable_alarm_point = 1;  //показываем точку выбранного будильника. флаг который используется в обработчике таймера индикации	if (i!=0)
	{
		alarm_number+=i;
		constrain((int8_t*)&alarm_number,0,3);
	}	if ((Time.am_pm)&&(Alarm[alarm_number].Al_hour>11))  //если 24, то меняем на 12
	{
		Ind[0].lamp=(Alarm[alarm_number].Al_hour-12)/10;
		Ind[1].lamp=(Alarm[alarm_number].Al_hour-12)%10;
	}
	else	{		Ind[0].lamp=Alarm[alarm_number].Al_hour/10;
		Ind[1].lamp=Alarm[alarm_number].Al_hour%10;	}	
	Ind[2].lamp=Alarm[alarm_number].Al_min/10;
	Ind[3].lamp=Alarm[alarm_number].Al_min%10;
}/************************************************************************/
/*			  Включение/выключение выбранного будильника                */
/************************************************************************/void activ_alarm(void){	DIVIDING_POINT_OFF;	Status = STATUS_ALARM;	selectable_alarm_point = 0;	no_points_global = 1;  //убрать точки с будильников	if (i!=0)	{		Alarm[alarm_number].Al_status^=(0x01); //инверсия бита статуса	}	Ind[0].lamp=10;	Ind[1].lamp=10;	Ind[2].lamp=10;	Ind[3].lamp=Alarm[alarm_number].Al_status;}/************************************************************************/
/*				       Установка часов будильника                       */
/************************************************************************/void hour_alarm(void){	DIVIDING_POINT_ON;	Status = STATUS_SETTINGS;	if (i!=0)
	{
		Alarm[alarm_number].Al_hour+=i;
		constrain((int8_t*)&Alarm[alarm_number].Al_hour,0,23);
	}	//проверка на АМ РМ	if (Time.am_pm==0)   //если 24, то моргаем  обе цифры часов
	{
		Ind[0].blink=Ind[1].blink=1;         //моргаем часами
		Ind[0].lamp=Alarm[alarm_number].Al_hour/10;		Ind[1].lamp=Alarm[alarm_number].Al_hour%10;
	}
	else                 //если 12, то моргаем в зависимости от АМ и РМ
	{
		if (Alarm[alarm_number].Al_hour>11)
		{
			Ind[0].blink=0;
			Ind[1].blink=1; // моргает 2 цифра - PM
			Ind[0].lamp=(Alarm[alarm_number].Al_hour-12)/10;			Ind[1].lamp=(Alarm[alarm_number].Al_hour-12)%10;
		}		else		{			Ind[0].blink=1;
			Ind[1].blink=0; // моргает 1 цифра - АM			Ind[0].lamp=Alarm[alarm_number].Al_hour/10;			Ind[1].lamp=Alarm[alarm_number].Al_hour%10;		}	}	Ind[2].lamp=Alarm[alarm_number].Al_min/10;	Ind[3].lamp=Alarm[alarm_number].Al_min%10;}/************************************************************************/
/*				     Установка минут будильника                         */
/************************************************************************/void min_alarm(void){	if (i!=0)
	{
		Alarm[alarm_number].Al_min+=i;
		constrain((int8_t*)&Alarm[alarm_number].Al_min,0,59);
	}	Ind[0].blink=Ind[1].blink=0;	Ind[2].blink=Ind[3].blink=1;	if ((Time.am_pm)&&(Alarm[alarm_number].Al_hour>11))  //если 24, то меняем на 12
	{
		Ind[0].lamp=(Alarm[alarm_number].Al_hour-12)/10;
		Ind[1].lamp=(Alarm[alarm_number].Al_hour-12)%10;
	}
	else	{		Ind[0].lamp=Alarm[alarm_number].Al_hour/10;
		Ind[1].lamp=Alarm[alarm_number].Al_hour%10;	}	Ind[2].lamp=Alarm[alarm_number].Al_min/10;
	Ind[3].lamp=Alarm[alarm_number].Al_min%10;}/************************************************************************/
/*						Выбор режима подсветки                          */
/************************************************************************/void mode_led(void)  //режимы подсветки. 1-без одсветки, 2-однородная, 3-смешанная
{
	Status = STATUS_LED;
	no_points_global = 1;  //убрать точки с будильников
	DIVIDING_POINT_OFF;
	if (i|=0)
	{
 		if ((LED.mode != 1)&&(flag_start_led == 0))	led_color_run();	//если цвет был остановлен, то запустить
		LED.mode+=i;
		constrain((int8_t*)&LED.mode,1,3);
		led_config();	//сконфигурировать настройки
	}
	Ind[3].blink=1;
	Ind[0].lamp=1;
	Ind[1].lamp=10;
	Ind[2].lamp=10;
	Ind[3].lamp=LED.mode;
}
/************************************************************************/
/*				Настрока частоты обновления цвета                       */
/************************************************************************/
void speed_led(void)  //период полной смены цветов
{
	Status = STATUS_SETTINGS;
	if (i|=0)
	{
		LED.speed+=i;
		constrain((int8_t*)&LED.speed,1,4);
		led_config();	//сконфигурировать настройки
	}
	Ind[0].lamp=2;
	Ind[1].lamp=10;
	Ind[2].lamp=10;
	Ind[3].lamp=LED.speed;	
}
/************************************************************************/
/*					Настройка яркости подсветки                         */
/************************************************************************/
void bright_led(void)
{
	if (i|=0)
	{
		LED.bright+=i;
		constrain((int8_t*)&LED.bright,1,4);
		led_config();	//сконфигурировать настройки
	}
	Ind[0].lamp=3;
	Ind[1].lamp=10;
	Ind[2].lamp=10;
	Ind[3].lamp=LED.bright;	
}




/************************************************************************/
/*	возвращает зациклинное значение параметра, ограниченное min и max   */
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