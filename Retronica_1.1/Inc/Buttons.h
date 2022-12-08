/*
 * Buttons.h
 *
 * Created: 23.01.2022 20:14:43
 *  Author: Evgeniy
 */ 

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <avr/io.h>
#include "SysTick.h"

//��������� ������
enum ButtonValues
{
	BUTTON_PLUS,
	BUTTON_MINUS,
	BUTTON_ENTER,
	BUTTON_PLUS_PUSH,
	BUTTON_MINUS_PUSH,
	BUTTON_ENTER_PUSH,
	BUTTON_NONE
};

enum ButtonValues GetButtonPress(void);//������� ������ ������, ���������� �� �������� �����

#endif /* BUTTON_H_ */