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
	uint8_t lamp;	//�������� ����� �� 0 �� 9 ��� ����������������� ����������� �� ������
	uint8_t blink;	//����� ��� ������� ���� ��� ����������
	uint8_t clone;
} Ind[4];			//���������� �� �����������

void indicators_init(void);   //������ �������� ��� ������

#endif /* INDICATION_H_ */