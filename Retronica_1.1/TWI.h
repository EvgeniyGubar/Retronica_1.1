/*
 * TWI.h
 *
 * Created: 22.01.2022 13:09:02
 *  Author: Evgeniy
 */ 

#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>

void i2c_Init(void);					//ÈÍÈÖÈÀËÈÇÀÖÈß
void i2c_start_cond(void);				//ÔÎĞÌÈĞÎÂÀÍÈÅ ÑÎÑÒÎßÍÈß ÑÒÀĞÒ ÍÀ ØÈÍÅ
void i2c_send_byte (unsigned char data);//ÎÒÏĞÀÂÊÀ ÁÀÉÒÀ ÄÀÍÍÛÕ
void i2c_stop_cond(void);				//ÔÎĞÌÈĞÎÂÀÍÈÅ ÑÎÑÒÎßÍÈß ÑÒÎÏ ÍÀ ØÈÍÅ
uint8_t i2c_get_byte(uint8_t ack);				//×ÒÅÍÈÅ ÁÀÉÒÀ ÄÀÍÍÛÕ   //0-ñ ïîäòâåğæäåíèåì, 1-áåç íåãî   uint8_t ack

#endif /* TWI_H_ */