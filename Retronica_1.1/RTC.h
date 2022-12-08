/*
 * RTC.h
 *
 * Created: 22.01.2022 11:36:03
 *  Author: Evgeniy
 */ 


#ifndef RTC_H_
#define RTC_H_

#include <avr/io.h>
#include "TWI.h"

/************************************************************************/
#define RTC_INPUT		(!(PINB&(1<<PB5)))  //ñèãíàë îò ds1307 êàæäóþ ñåêóíäó

/*àäðåñà ðåãèñòðîâ*/
#define RTC_SEC_ADR     0x00
#define RTC_MIN_ADR     0x01
#define RTC_HOUR_ADR    0x02
#define RTC_DAY_ADR     0x04
#define RTC_MONTH_ADR   0x05
#define RTC_YEAR_ADR    0x06
#define RTC_SQWE_ADR    0x07
// #define RTC_RAM_1		0x08
// #define RTC_RAM_2		0x09
// #define RTC_RAM_3		0x0A
// #define RTC_RAM_4		0x0B
// #define RTC_RAM_5		0x0C
// #define RTC_RAM_6		0x0D
// #define RTC_RAM_7		0x0F
// #define RTC_RAM_8		0x10
// #define RTC_RAM_9		0x11
// #define RTC_RAM_10		0x12
// #define RTC_RAM_11		0x13
// #define RTC_RAM_12		0x14
// #define RTC_RAM_13		0x15
// #define RTC_RAM_14		0x16
/************************************************************************/

void RTC_init(void);							//Èíèöèàëèçàöèÿ
void RTC_SetValue(uint8_t adr, uint8_t data);	//ÇÀÏÈÑÜ ÁÀÉÒÀ ÄÀÍÍÛÕ ÏÎ ÓÊÀÇÀÍÍÎÌÓ ÀÄÐÅÑÓ
uint8_t RTC_get(uint8_t address);				//×òåíèå ÎÄÍÎÃÎ áàéòà ïî óêàçàííîìó àäðåñó


#endif /* RTC_H_ */