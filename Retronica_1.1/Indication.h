/*
 * Indication.h
 *
 * Created: 22.01.2022 18:05:45
 *  Author: Evgeniy
 */ 


#ifndef INDICATION_H_
#define INDICATION_H_

#include <avr/io.h>

struct
{
	uint8_t lamp;	//содержат числа от 0 до 9 для непосредственного отображения на лампах
	uint8_t blink;	//флаги для мигания ламп при настройках
	uint8_t clone;
} Ind[4];			//информация об индикаторах

void indicators_init(void);   //запуск таймеров при старте

#endif /* INDICATION_H_ */