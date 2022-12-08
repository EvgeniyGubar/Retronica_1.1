/*
 * ds18b20.h
 *
 * Created: 22.01.2022 17:33:48
 *  Author: Evgeniy
 */ 


#ifndef DS18B20_H_
#define DS18B20_H_

#define F_CPU 16000000UL //частота тактирования процессора

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t therm_start_convert(void); 
void therm_read_temperature(uint16_t *temperature);  //датчик 1-wire ds18b20


#endif /* DS18B20_H_ */