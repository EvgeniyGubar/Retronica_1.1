/*
 * led_setting.c
 *
 * Created: 23.01.2022 20:00:09
 *  Author: Evgeniy
 */ 

#include "LED.h"

static uint16_t delay_for_leds;	//������������� �������� ����� ������
static uint8_t value;			//������� ������� � �������������� HSV to RGB �� 0 �� 255

static void hsvToRgb(uint16_t h, uint8_t *result); //�������� ����������� HSV � RGB   


/************************************************************************/
/*       ������������� �������� ����� ���������� ������ �����           */
/************************************************************************/
void led_delay(void)

{
	volatile static uint16_t current_delay; //������� ������� ��������
	
	if (GetTick() - current_delay >= delay_for_leds)
	{
		current_delay = GetTick();
		led_processing();
	}
}
/************************************************************************/
/*			    ������� ������ ����� �� ����������			            */
/************************************************************************/void led_processing(void){	static uint8_t RGB[12];	switch(LED.mode)
	{
		case 1:{//����� 1 - ��� ���������
					cli();
					for (uint8_t i=0; i<12; i++)
					{
						ws2812_send_byte(0);
					}
					sei();
				};break;
		case 2:{//����� 2 - ����������� ���������
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
		case 3:{//����� 3 - ��������� ������
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
/*				     ������������ ���������                             */
/************************************************************************/void led_config(void){	switch(LED.speed)   //��, �� ������� 360 �������� �����
	{
		case 1:{delay_for_leds = 28;};break;// 10 ������   
		case 2:{delay_for_leds = 83;};break;// 30 ������	
		case 3:{delay_for_leds = 167;};break;// 1 ������	
		case 4:{delay_for_leds = 1667;};break;// 10 �����	
		case 5:{delay_for_leds = 5;};break;// 1,8 ������� (������ ��� ������ �����)
	}	switch(LED.bright)  //������ ������� �� 0 �� 255
	{
		case 1:{value=255;};break;  //100% �������
		case 2:{value=128;};break;	//75% �������
		case 3:{value=51;};break;	//50% �������
		case 4:{value=24;};break;	//25% �������
	}	if (LED.mode==1)	flag_start_led = 0;  //��������� ���������	else				flag_start_led = 1;  //���������� ���� �� ������ ���������	led_processing();	//����� ��������� ��������� ������� �� ����������}/************************************************************************/
/*         ���������� ���������� ����� � ������ � ��� ��������          */
/************************************************************************/void led_color_stop(void)   //  �����������/��������� �������� ����� ������{
	flag_start_led = 0;
	eeprom_write_word(& color_eeprom, color_angle);
}/************************************************************************/
/*           ������ ����� ����� � �������� �� eeprom                    */
/************************************************************************/void led_color_run(void){	flag_start_led = 1;//������ �������� � ������� ������	eeprom_write_word(& color_eeprom, 0);}/************************************************************************/
/*              �������� ����������� HSV � RGB                          */
/************************************************************************/// Converts a color from HSV to RGB.
// h is hue, as a number between 0 and 360.
// s is the saturation, as a number between 0 and 255.
// v is the value, as a number between 0 and 255.
static void hsvToRgb(uint16_t h, uint8_t *result)
{
	uint8_t red, green, blue;  //���������� ��� ������� ����� ������������ ���������
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