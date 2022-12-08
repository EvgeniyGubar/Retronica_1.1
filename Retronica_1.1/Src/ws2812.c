/*
 * ws2812.c
 *
 * Created: 22.01.2022 17:43:20
 *  Author: Evgeniy
 */ 

#include "ws2812.h"

#define set   PORTD|=(1<<PD7)
#define reset PORTD&=~(1<<PD7)

 void set_zero(void);						 //установка "0"
 void set_one(void);							 //установка "1"

void ws2812_init(void)
{
	DDRD|=(1<<PD7);//светодиод
	
	for (uint8_t i=0; i<12; i++)//выключение подсветки (от помех после включения могут гореть произвольным цветом)
	{
		ws2812_send_byte(0);
	}
}

//установка "0"
 void set_zero(void)
{
	set;
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	reset;
}

//установка "1"
 void set_one(void)
{
	set;
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	reset;
}

/*********ОТПРАВКА БАЙТА ИНФОРМАЦИИ*************/
void ws2812_send_byte(unsigned char c)
{
	unsigned char i=0;
	for(i=0;i<8;i++)
	{
		if(c & 0x80) set_one();
		else	    set_zero();
		c <<= 1;
	}
}