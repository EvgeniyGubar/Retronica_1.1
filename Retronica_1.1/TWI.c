/*
 * TWI.c
 *
 * Created: 22.01.2022 13:08:37
 *  Author: Evgeniy
 */ 

#include "TWI.h"

#define F_CPU		16000000UL //������� ������������ ����������
#define F_I2C		100000UL                  //������� ������ ����
#define TWBR_VALUE	(((F_CPU)/(F_I2C)-16)/2)

#if ((TWBR_VALUE > 255) || (TWBR_VALUE == 0))
#error "TWBR value is not correct"
#endif

/************************************************************************/
/*							�������������                               */
/************************************************************************/
void i2c_Init(void)
{
	TWBR = TWBR_VALUE;
	TWSR = 0;
}
/************************************************************************/
/*				������������ ��������� ����� �� ����                    */
/************************************************************************/
void i2c_start_cond(void)
{
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);   //��������� ��������� �����
	while(!(TWCR&(1<<TWINT)));              //����, ����� ����������� ���� �������� �������� TWINT
}
/************************************************************************/
/*						�������� ����� ������                           */
/************************************************************************/
void i2c_send_byte (unsigned char data)
{
	TWDR=data;                              //������ � ������� ������ ����� ����������
	TWCR=(1<<TWINT)|(1<<TWEN);              //��������
	while(!(TWCR&(1<<TWINT)));              //����, ����� ����������� ���� �������� �������� TWINT
}
/************************************************************************/
/*				������������ ��������� ���� �� ����                     */
/************************************************************************/
void i2c_stop_cond(void)
{
	TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);   //��������� ��������� ����
}
/************************************************************************/
/*						������ ����� ������                             */
/************************************************************************/
uint8_t i2c_get_byte(uint8_t ack)    //0-� ��������������, 1-��� ����   uint8_t ack
{
	uint8_t data=0;
	
	if (ack)  //������ � �������������
	{
	 	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	 	while(!(TWCR & (1<<TWINT)));
	 	data = TWDR;
	}
	else  //������ ��� �������������	
	{
		TWCR = (1<<TWINT)|(1<<TWEN);
		while(!(TWCR & (1<<TWINT)));
		data = TWDR;
	};
	return data;
}
