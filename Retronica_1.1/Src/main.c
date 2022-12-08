/*
 *
 *
 */ 

#include "main.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/void get_mcucsr(void)
{
	mcusr_mirror = MCUCSR;
	MCUCSR = 0;
	wdt_disable();
}						
/************************************************************************/
/*                                                                      */
/************************************************************************/
int main(void)		
{			
	get_mcucsr();		//получить данные о источнике сброса
	ws2812_init();
 	ports_init();		
 	SysTick_init();		//инициализация таймера, отсчитывающего задержки
 	indicators_init();	//инициализация таймера динамической индикации
 	i2c_Init();			
 	RTC_init();			//если часы не запущены, то запустит
	sei();
		
 	if (!(mcusr_mirror&(1<<WDRF)))    //если сброс был не от WDT то с анимацией включения
 	{
		volatile uint16_t delay_animation = 0;
	 	uint8_t i=0;
		 
		//тестовый прогон подсветки
		LED.mode = 2;   //монохром
		LED.speed = 5;  //особая скорость - полный оборот 1,8 секунды
		LED.bright = 1; //полная яркость
		led_config();   //запуск подсветки
		
		//тестовый прогон индикаторов
		while(GetTick()<1800)
		{
			Ind[0].lamp = Ind[1].lamp = Ind[2].lamp = Ind[3].lamp = i;
			led_delay();	//запуск подсветки
			if (GetTick() - delay_animation >= 180)
			{
				delay_animation = GetTick();
				i++;
			}			
		}
	}
	else  //если от таймера, то сохранить количество ошибок
	{
		uint16_t error;
		error = eeprom_read_word((uint16_t *)50);//записать в лог, количество сбросов по таймеру watchdog
		error++;
		eeprom_write_word((uint16_t *)50, error);
	}
	read_eeprom();		//читаем настройки из EEPROM, если EEPROM чистая, т.е. это первое включение, то запишет стандартные настройки в EEPROM
	
	while(GetTick()<1801){};	//таймаут для сброса светодиодов
 	
	if (color_angle!=0)   //если режим захвата цвета был установлен, то отключаем анимацию и устанавливаем цвет для
 	{		 
		led_config();	
		flag_start_led = 0;       
 	}
	else led_config();
	
	start_program_flag = 1;
	Menu_Navigate(&Main_menu);	//запуск работы MicroMenu
		
	wdt_enable(WDTO_2S);	//сторожевой таймер на 2 секунды на всякий случай
	
while (1)
{	
	asm("WDR");   //сброс таймера 		
	
	static uint8_t input_rtc_flag;		//флаг новой секунды, сигнал о которой приходит от ds1307	

	if ((!RTC_INPUT)&&(!input_rtc_flag))	
	{
		input_rtc_flag = 1;  //сделано, чтобы обработка прерывания от ds13007 выполнялась только один раз на весь низкий уровень
		
		total_us += (uint32_t)correct_us; //подсчет набежавшей коррекции
		if (total_us >= 10000000 ) start_correct_flag = 1;  //выкладки в excel файле
	}
	if (RTC_INPUT&&input_rtc_flag)	
	{
		input_rtc_flag = 0;
		if ((Status == STATUS_MAIN) && (!single_animation_flag))	time_manager();	//обработка сигнала от ds1307 		 
	}  	 	if (flag_settings_delay)	stop_delay();		//задержка выхода из настроек (5 секунд) 	if (flag_start_led)			led_delay();		//запускает обработку смены цвета
	if (buzzer_flag)			start_buzzer();		//включение будильника
	if (single_animation_flag)	single_animation();	//анимация переключения цифр против отравления катодов
	if (therm_ready_flag)							//если запрос на преобразование отправлен, то ждем 750 мс и считываем данные о температуре
	{
		if (GetTick() - therm_ready_delay > 750)
		{
			therm_ready_flag = 0;
			therm_read_temperature(&temp_data);
		}
	}
	button_processing();
  }
}




/************************************************************************/
/*                Установка флага на запуск анимации                    */
/************************************************************************/
static void single_animation_start(void)
{
	delay_animation = GetTick();
	single_animation_flag = 1;
}
/************************************************************************/
/*			     Анимация против отравления катодов                     */
/************************************************************************/
static void single_animation(void)
{
	uint8_t arr[10] = {3,8,9,4,0,5,7,2,6,1};
	static uint8_t num, index;
	
	Ind[num].lamp = arr[index];
	
	if (GetTick() - delay_animation > 50)
	{
		delay_animation = GetTick();
		index++;
		if (index == 10)
		{
			Ind[num].lamp = Ind[num].clone;
			num++;
			index = 0;
			if (num == 4)
			{
				num = 0;
				single_animation_flag = 0;
			}
		}
	}
}
/************************************************************************/
/*      Запуск задержки в 5 секунд для выхода из режима настроек       */
/************************************************************************/
static void start_delay(void)  {	flag_settings_delay = 1;	delay_for_exit = GetTick();}
/************************************************************************/
/*					Задержка выхода из настроек                         */
/************************************************************************/
static void stop_delay(void)
{	
	if (GetTick() - delay_for_exit > 5000) 
	{		Menu_Navigate(&Main_menu);
	}
}
/************************************************************************/
/*					 Опрос кнопок и навигация                           */
/************************************************************************/
static void button_processing(void)
{
	enum ButtonValues state = GetButtonPress();
	
	if (buzzer_flag&&(state != BUTTON_NONE))	
		{
			buzzer_flag = stop_buzzer();
			return;
		}
	if (state != BUTTON_NONE)
	{
		start_delay();
		switch (state)
		{
			case BUTTON_PLUS:
			{
				i = 1;
				if (Status != STATUS_MAIN)  Menu_Navigate(MENU_PREVIOUS); // если не в основном режиме, то кнопка плюс
				if ((Status == STATUS_MAIN)&&LED.mode!=1) //иначе остановка/запуск подсветки
				{
						if ((LED.mode != 1)&&(flag_start_led == 1))	led_color_stop();   //остановка если режим не первый и мы еще не сохранили цвет
						else	led_color_run();						
				}  
			}break;	
			case BUTTON_PLUS_PUSH:
			{
				i = 0;
				if (Status == STATUS_MAIN) {single_animation_flag = 0; Menu_Navigate(&mode_light);}   //если в основном режиме - заходим в меню подсветки
			}break;	
			case BUTTON_MINUS:
			{
				i = -1;
				if (Status != STATUS_MAIN)Menu_Navigate(MENU_NEXT);   // если не в основном режиме, то кнопка минус
			}break;	
			case BUTTON_MINUS_PUSH:
			{
				i = 0;
				if (Status == STATUS_MAIN) {single_animation_flag = 0; Menu_Navigate(&num_alarm);}
			}break;		
			case BUTTON_ENTER:
			{
				i = 0;
				if (Status != STATUS_MAIN)
				{
					//быстрый выход из настроек, если функции не активированы
					if(((Status == STATUS_ALARM)&&Alarm[alarm_number].Al_status==0)||((Status == STATUS_LED)&&LED.mode==1))  Menu_Navigate(&Main_menu);
					else Menu_Navigate(MENU_CHILD);
				}
			}break;	
			case BUTTON_ENTER_PUSH:
			{
				i = 0;
				if (Status == STATUS_MAIN) {single_animation_flag = 0; Menu_Navigate(&am_pm);}
			}break;			default:break;		}
	}
}
/************************************************************************
   Функция отображения времени/даты/температуры. Следит за будильниками
     и выставляет флаг на включение. Включает флаги на анимацию против
							отравления катодов                                                                 
************************************************************************/void time_manager(void){	Time.Sec=RTC_get(RTC_SEC_ADR);    //получение данных от микросхемы часов RTC    Time.Min=RTC_get(RTC_MIN_ADR);    Time.Hour=RTC_get(RTC_HOUR_ADR);		
	Time.Hour=((Time.Hour>>4)*10)+(Time.Hour&0x0f); //перевод в десятичные
	Time.Min=(((Time.Min>>4)*10)+(Time.Min&0x0f));  //перевод в десятичные
		
/************************************************************************/
/*			   Отключить будильник спустя минуту работы                 */
/************************************************************************/
	if (buzzer_flag&&Time.Sec==0x59)	buzzer_flag = stop_buzzer();
	
/************************************************************************/
/*					Проверка будильников и их включение				    */
/************************************************************************/
	if (Time.Sec==0)
	{
		for (uint8_t j=0;j<4;j++)
		{
			if ((Alarm[j].Al_status==1) && (Alarm[j].Al_hour==Time.Hour) && (Alarm[j].Al_min==Time.Min))
			{
				buzzer_flag = 1;			// функция включения звука			
			}
		}
	}
/************************************************************************/
/*						Корректировка времени                           */
/************************************************************************/
	if ((start_correct_flag)&&(Time.Sec==0x25)) //делать перевод на 25-й секунде (25 представлена в 16-тиричной системе)
	{											//вычитание и сложение в 10-тичной единицы даст верную запись в 16-ти
		start_correct_flag = 0;
		total_us -= 10000000;	//обнулить счетчик корректировочных микросекунд
		if (Time.Correct_sec > 0)Time.Sec += 1;
		if (Time.Correct_sec < 0)Time.Sec -= 1;
		RTC_SetValue(RTC_SEC_ADR, ((Time.Sec>>4)*10) + (Time.Sec&0x0F));	//преобразовать секунды в десятичный формат и записать
	}
/************************************************************************/
/*						Условие на показ даты			                */
/************************************************************************/
	if ((Time.data_on) && (((Time.Sec>=0x05)&&(Time.Sec<0x10))||((Time.Sec>=0x35)&&(Time.Sec<0x40))))
	{
		no_points_global = 1;  //убрать точки с будильников
		Time.Day=RTC_get(RTC_DAY_ADR);
		Time.Month=RTC_get(RTC_MONTH_ADR);
		DIVIDING_POINT_ON;
		Ind[0].lamp=Time.Day>>4;		Ind[1].lamp=Time.Day&0x0f;
		Ind[2].lamp=Time.Month>>4;
		Ind[3].lamp=Time.Month&0x0f;
		return;
	}	
/************************************************************************/
/*					Условие на показ температуры                        */
/************************************************************************/	
    if((Time.temp_on)&&(((Time.Sec>=0x19)&&(Time.Sec<0x24))||((Time.Sec>=0x49)&&(Time.Sec<0x54))))  //Запуск преобразования за секунду до показа
    {
	  	therm_ready_flag = therm_start_convert();
		therm_ready_delay = GetTick();
		if (!therm_ready_flag)	temp_data = 999; //если датчик не найден, то выдать ошибку 99,9
    }
 	if ((Time.temp_on) && (((Time.Sec>=0x20)&&(Time.Sec<0x25))||((Time.Sec>=0x50)&&(Time.Sec<0x55))))  //Показ температуры
 	{	
		static uint16_t filt_temp_data;
		const uint8_t k = 3, A = 7;  //[k=2 A=3], [k=3 A=7], [k=4 A=15], [k=5 A=31], чем больше k тем плавнее фильтр
		
		if (abs(temp_data - filt_temp_data) > 20) filt_temp_data = temp_data; //если отличаются на 2 градуса, то фильтру присвоить новое значение
		else	filt_temp_data = (A*filt_temp_data + temp_data) >> k;  //фильтр
		
		if (filt_temp_data > 999) filt_temp_data = 999;  //максимумальная отоброжаемая температура
	
		no_points_global = 1;  //убрать точки с будильников
		DIVIDING_POINT_ON;
				
 		Ind[0].lamp = filt_temp_data/100;
 		Ind[1].lamp = (filt_temp_data%100)/10;
 		Ind[2].lamp = filt_temp_data%10;
		Ind[3].lamp = 10;	
		return;
	}	
/************************************************************************/
/*			     Условие на пробег цифр каждые 5 минут					*/
/************************************************************************/
  		no_points_global = 0;  //показать точки будильников
  		if ((Time.am_pm)&&(Time.Hour>11))  Time.Hour-=12;				
  		
  		if ( ((Time.Sec==0x00)&&((Time.Min%5)==0)) || start_program_flag )   //Анимация переключения против отравления катодов каждые 5 минут
  		{
  			start_program_flag = 0;
  			single_animation_start();
  			Ind[0].clone = Time.Hour/10;
  			Ind[1].clone = Time.Hour%10;
  			Ind[2].clone = Time.Min/10;
  			Ind[3].clone = Time.Min%10;
  		}		
/************************************************************************/
/*			Если никакие условия не проходят, то показать время         */
/************************************************************************/
		else
  		{
	  		Ind[0].lamp=Time.Hour/10;
	  		Ind[1].lamp=Time.Hour%10;
	  		Ind[2].lamp=Time.Min/10;
	  		Ind[3].lamp=Time.Min%10;
  		}
  		PORTC^=(1<<PC0); //инверсия точки
}/************************************************************************/
/*                                                                      */
/************************************************************************/
static void ports_init(void)
{
	//на выход
	DDRB|=(1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3); //аноды ламп 1-4
	DDRC|=(1<<PC0); //анод ИНС-1
	DDRD|=(1<<PD0)|(1<<PD1)|(1<<PD2)|(1<<PD3)    |(1<<PD5)   |(1<<PD6); //выходы на катоды через микросхему дешифратор, зумер, точку ламп
	
	//на вход с подтягиванием к +5
	PORTC|=(1<<PC1)|(1<<PC2)|(1<<PC3); //кнопки
	PORTB|=(1<<PB5); // внешнее прерывание от часов
}





/************************************************************************/
/*                                                                      */
/************************************************************************/
/*
static void full_animation_start(void)
{
	delay_animation = GetTick();
	full_animation_flag = 1;
}
/ ************************************************************************ /
/ *                                                                      * /
/ ************************************************************************ /
static void full_animation(void)
{
	uint8_t arr[10] = {3,8,9,4,0,5,7,2,6,1};
	static uint8_t index;

	Ind[0].lamp = Ind[1].lamp = Ind[2].lamp = Ind[3].lamp = arr[index];
	if (GetTick() - delay_animation > 50)
	{
		delay_animation = GetTick();
		index++;
		if (index == 10)
		{
			index = 0;
			full_animation_flag = 0;
			Ind[0].lamp = Ind[0].clone;
			Ind[1].lamp = Ind[1].clone;
			Ind[2].lamp = Ind[2].clone;
			Ind[3].lamp = Ind[3].clone;
		}
	}
}
*/
