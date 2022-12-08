/*
 * Alarm_setting.c
 *
 * Created: 24.01.2022 19:53:31
 *  Author: Evgeniy
 */ 

#include "Buzzer.h"
/************************************************************************/
/*                        Запуск пищалки                                */
/************************************************************************/
void start_buzzer(void)
{
	static uint8_t buzz_count;
	volatile static uint16_t delay_buzz;
	if (GetTick() - delay_buzz >= 3)
	{
		delay_buzz = GetTick();
		buzz_count++;
		BUZZ_ON;
		if (buzz_count>20){BUZZ_OFF;}
		if (buzz_count>40){BUZZ_ON;}
		if (buzz_count>60){BUZZ_OFF;}
		if (buzz_count>80){BUZZ_ON;}
		if (buzz_count>100){BUZZ_OFF;}
		if (buzz_count>120){BUZZ_ON;}
		if (buzz_count>140){BUZZ_OFF;}
		if (buzz_count==255){buzz_count=0;}
	}
}
/************************************************************************/
/*                       Остановка пищалки                              */
/************************************************************************/
uint8_t stop_buzzer(void)
{
	BUZZ_OFF;
	return 0;
}