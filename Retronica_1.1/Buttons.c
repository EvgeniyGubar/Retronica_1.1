/*
 * Buttons.c
 *
 * Created: 23.01.2022 20:15:25
 *  Author: Evgeniy
 */ 


#include "Buttons.h"

#define ENTER              (!(PINC&(1<<PC2)))  //кнопка
#define MINUS              (!(PINC&(1<<PC1)))  //кнопка
#define PLUS               (!(PINC&(1<<PC3)))   //кнопка
#define BUTTON_PRESS		(PINC&0x0E)		//если нажата хотя бы одна кнопка			

/************************************************************************/
/*			Функция опроса кнопок, вызывается из главного цикла         */
/************************************************************************/
enum ButtonValues GetButtonPress(void){
	
 	volatile static uint16_t time = 0;           //время последней выборки входного вывода
	volatile static uint16_t push_count=0;       //счетчик удержания кнопок, флаг удержания
	static uint8_t push_flag = 0;
 	static uint8_t button_count = 0;
	static uint8_t button_state = 0; 
	
 	if(GetTick() != time)
 	{
 		if(BUTTON_PRESS != 0x0E )
 		{
 			if (button_count < 25) button_count++;	//счетчик дребезгов при нажатии в мс
 			if (button_count == 25)
 			{
				button_state = BUTTON_PRESS;		//зафиксировали сосотояние кнопок
  				if (push_count < 500) push_count++;
 				if (push_count == 499)		//срабатывание при удержанием
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
 			if (button_count > 0)	//счетчик дребезгов при отжатии
			{
				 button_count--; 
				 push_count = 0;
				 if (button_count == 0)	//короткое срабатывание при спадающем фронте
				 {
					 if (push_flag == 1)	push_flag = 0;		//если было срабатывание при удержании кнопки, то выходим из функции
					 else	//иначе - обработать короткое нажатие
					 {
						 switch(button_state)	//если состояние зафиксировалось при нажатии, то кнопка сработает при отжатии
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