/*
 * led_setting.c
 *
 * Created: 23.01.2022 20:00:09
 *  Author: Evgeniy
 */ 

#include "LED.h"

static uint16_t delay_for_leds;	//установленная задержка смены цветов
static uint8_t value;			//уровень яркости в преобразовании HSV to RGB от 0 до 255

static void hsvToRgb(uint16_t h, uint8_t *result); //Функуция конвертации HSV В RGB   


/************************************************************************/
/*       Неблокирующая задержка между процессами вывода цвета           */
/************************************************************************/
void led_delay(void)

{
	volatile static uint16_t current_delay; //счетчик текущей задержки
	
	if (GetTick() - current_delay >= delay_for_leds)
	{
		current_delay = GetTick();
		led_processing();
	}
}
/************************************************************************/
/*			    Процесс вывода цвета на светодиоды			            */
/************************************************************************/void led_processing(void){	static uint8_t RGB[12];	switch(LED.mode)
	{
		case 1:{//Режим 1 - без подсветки
					cli();
					for (uint8_t i=0; i<12; i++)
					{
						ws2812_send_byte(0);
					}
					sei();
				};break;
		case 2:{//Режим 2 - монохромная подсветка
					hsvToRgb(color_angle, RGB);
					cli();
					for (uint8_t l=0;l<12;l++)
					{
						for ( uint8_t i=0; i<3; i++)
						{
							ws2812_send_byte(RGB[i]);
						}
					}
					sei();
					color_angle++;
					if (color_angle==360)color_angle=0;
				};break;
		case 3:{//Режим 3 - подсветка радуга
					hsvToRgb(color_angle, RGB);
					hsvToRgb(color_angle+90, &RGB[3]);
					hsvToRgb(color_angle+180, &RGB[6]);
					hsvToRgb(color_angle+270, &RGB[9]);					
					cli();
					for (uint8_t i = 0; i<12; i++)
					{
						ws2812_send_byte(RGB[i]);
					}		
					sei();			
					color_angle++;
					if (color_angle==360) color_angle=0;
				};break;
	}}/************************************************************************/
/*				     Конфигурация подсветки                             */
/************************************************************************/void led_config(void){	switch(LED.speed)   //мс, из расчета 360 вариаций цвета
	{
		case 1:{delay_for_leds = 28;};break;// 10 секунд   
		case 2:{delay_for_leds = 83;};break;// 30 секунд	
		case 3:{delay_for_leds = 167;};break;// 1 минута	
		case 4:{delay_for_leds = 1667;};break;// 10 минут	
		case 5:{delay_for_leds = 5;};break;// 1,8 секунды (только при старте часов)
	}	switch(LED.bright)  //уровни яркости от 0 до 255
	{
		case 1:{value=255;};break;  //100% яркость
		case 2:{value=128;};break;	//75% яркость
		case 3:{value=51;};break;	//50% яркость
		case 4:{value=24;};break;	//25% яркость
	}	if (LED.mode==1)	flag_start_led = 0;  //остановка подсветки	else				flag_start_led = 1;  //установить флаг на запуск подсветки	led_processing();	//сразу запустить одиночный процесс на светодиоды}/************************************************************************/
/*         Сохранение выбранного цвета в память и его фиксация          */
/************************************************************************/void led_color_stop(void)   //  оставливает/запускает анимацию смены цветов{
	flag_start_led = 0;
	eeprom_write_word(& color_eeprom, color_angle);
}/************************************************************************/
/*           Запуск смены цвета и удаление из eeprom                    */
/************************************************************************/void led_color_run(void){	flag_start_led = 1;//запуск анимации и очистка памяти	eeprom_write_word(& color_eeprom, 0);}/************************************************************************/
/*              Функуция конвертации HSV В RGB                          */
/************************************************************************/// Converts a color from HSV to RGB.
// h is hue, as a number between 0 and 360.
// s is the saturation, as a number between 0 and 255.
// v is the value, as a number between 0 and 255.
static void hsvToRgb(uint16_t h, uint8_t *result)
{
	uint8_t red, green, blue;  //переменные для задания цвета светодиодной подсветки
	uint8_t f = (h % 60) * 255 / 60;
	uint8_t p = 0;
	uint8_t q = (255 - f ) * value / 255;
	uint8_t t = (255 - (255 - f)) * value / 255;
	
	switch((h / 60) % 6)
	{
		case 0: red = value; green = t; blue = p; break;
		case 1: red = q; green = value; blue = p; break;
		case 2: red = p; green = value; blue = t; break;
		case 3: red = p; green = q; blue = value; break;
		case 4: red = t; green = p; blue = value; break;
		case 5: red = value; green = p; blue = q; break;
	}	
	result[0] = green;
	result[1] = red;
	result[2] = blue;
}