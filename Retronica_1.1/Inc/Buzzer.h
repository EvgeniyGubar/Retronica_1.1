/*
 * alarm_setting.h
 *
 * Created: 24.01.2022 19:53:59
 *  Author: Evgeniy
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_

#define BUZZ_ON            PORTD|=(1<<PD5)     //������
#define BUZZ_OFF           PORTD&=~(1<<PD5)

#include <avr/io.h>
#include <stdlib.h>
#include "SysTick.h"

void start_buzzer(void);//������ �������
uint8_t stop_buzzer(void);	//��������� ������� - ��������� 0

#endif /* BUZZER_H_ */