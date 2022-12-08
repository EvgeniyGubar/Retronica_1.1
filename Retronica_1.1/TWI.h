/*
 * TWI.h
 *
 * Created: 22.01.2022 13:09:02
 *  Author: Evgeniy
 */ 

#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>

void i2c_Init(void);					//хмхжхюкхгюжхъ
void i2c_start_cond(void);				//тнплхпнбюмхе янярнъмхъ ярюпр мю ьхме
void i2c_send_byte (unsigned char data);//нропюбйю аюирю дюммшу
void i2c_stop_cond(void);				//тнплхпнбюмхе янярнъмхъ ярно мю ьхме
uint8_t i2c_get_byte(void);				//времхе аюирю дюммшу   //0-Я ОНДРБЕПФДЕМХЕЛ, 1-АЕГ МЕЦН   uint8_t ack

#endif /* TWI_H_ */