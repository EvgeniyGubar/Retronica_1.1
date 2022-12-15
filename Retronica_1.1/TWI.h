/*
 * TWI.h
 *
 * Created: 22.01.2022 13:09:02
 *  Author: Evgeniy
 */ 

#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>

void i2c_Init(void);					//�������������
void i2c_start_cond(void);				//������������ ��������� ����� �� ����
void i2c_send_byte (unsigned char data);//�������� ����� ������
void i2c_stop_cond(void);				//������������ ��������� ���� �� ����
uint8_t i2c_get_byte(uint8_t ack);				//������ ����� ������   //0-� ��������������, 1-��� ����   uint8_t ack

#endif /* TWI_H_ */