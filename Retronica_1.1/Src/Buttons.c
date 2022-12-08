/*
 * Buttons.c
 *
 * Created: 23.01.2022 20:15:25
 *  Author: Evgeniy
 */ 


#include "Buttons.h"

#define ENTER              (!(PINC&(1<<PC2)))  //������
#define MINUS              (!(PINC&(1<<PC1)))  //������
#define PLUS               (!(PINC&(1<<PC3)))   //������
#define BUTTON_PRESS		(PINC&0x0E)		//���� ������ ���� �� ���� ������			

/************************************************************************/
/*			������� ������ ������, ���������� �� �������� �����         */
/************************************************************************/
enum ButtonValues GetButtonPress(void){
	
 	volatile static uint16_t time = 0;           //����� ��������� ������� �������� ������
	volatile static uint16_t push_count=0;       //������� ��������� ������, ���� ���������
	static uint8_t push_flag = 0;
 	static uint8_t button_count = 0;
	static uint8_t button_state = 0; 
	
 	if(GetTick() != time)
 	{
 		if(BUTTON_PRESS != 0x0E )
 		{
 			if (button_count < 25) button_count++;	//������� ��������� ��� ������� � ��
 			if (button_count == 25)
 			{
				button_state = BUTTON_PRESS;		//������������� ���������� ������
  				if (push_count < 500) push_count++;
 				if (push_count == 499)		//������������ ��� ����������
 				{
 					push_flag = 1;
 					switch(button_state)
 					{
	 					case 0x0A: return BUTTON_ENTER_PUSH; break;
	 					case 0x0C: return BUTTON_MINUS_PUSH; break;
	 					case 0x06: return BUTTON_PLUS_PUSH; break;
	 					default: break;
 					}
 				}
			}
 		}
 		else
 		{
 			if (button_count > 0)	//������� ��������� ��� �������
			{
				 button_count--; 
				 push_count = 0;
				 if (button_count == 0)	//�������� ������������ ��� ��������� ������
				 {
					 if (push_flag == 1)	push_flag = 0;		//���� ���� ������������ ��� ��������� ������, �� ������� �� �������
					 else	//����� - ���������� �������� �������
					 {
						 switch(button_state)	//���� ��������� ��������������� ��� �������, �� ������ ��������� ��� �������
						 {
							 case 0x0A: return BUTTON_ENTER; break;
							 case 0x0C: return BUTTON_MINUS; break;
							 case 0x06: return BUTTON_PLUS; break;
							 default: break;
						 }
						 button_state = 0;
					 }
				 }
			}
		}
		time = GetTick();
 	}	
	return BUTTON_NONE;
}