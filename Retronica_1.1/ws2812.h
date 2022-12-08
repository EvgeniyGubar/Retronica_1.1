/*
 * ws2812.h
 *
 * Created: 22.01.2022 17:43:03
 *  Author: Evgeniy
 */ 


#ifndef WS2812_H_
#define WS2812_H_

#include <avr/io.h>
									
void ws2812_init(void);						 //инициализация
void ws2812_send_byte(unsigned char c);  //отправка байта информации

#endif /* WS2812_H_ */