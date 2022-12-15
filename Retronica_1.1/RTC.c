/*
 * RTC.c
 *
 * Created: 22.01.2022 17:31:35
 *  Author: Evgeniy
 */ 

#include "RTC.h"

#define RTC_SET_POINTER	0xff //счетчик указателя адреса
#define DS1307_ADR_W	0xD0
#define DS1307_ADR_R	0xD1

//uint8_t RTC_GetValue(uint8_t ack);

/************************************************************************/
/*                                                                      */
/************************************************************************/
void RTC_init(void)
{
	RTC_SetValue(RTC_SQWE_ADR, 10);	//разрешение мендра с частотой 1Гц
	if ((RTC_get(RTC_SEC_ADR)&0x80)!=0)	//если часы не идут (7 бит=1) - первый запуск
	{
		RTC_SetValue(RTC_HOUR_ADR, 12);
		RTC_SetValue(RTC_MIN_ADR, 30);
		RTC_SetValue(RTC_SEC_ADR, 00);
		RTC_SetValue(RTC_MONTH_ADR, 12);
		RTC_SetValue(RTC_DAY_ADR, 31);
		RTC_SetValue(RTC_YEAR_ADR, 22);
	}
}
/************************************************************************/
/*            ЗАПИСЬ БАЙТА ДАННЫХ ПО УКАЗАННОМУ АДРЕСУ                  */
/************************************************************************/
void RTC_SetValue(uint8_t adr, uint8_t data)
{
	/*формируем состояние СТАРТ*/
	i2c_start_cond();
	/*выдаемна шину пакет SLA-W*/
	i2c_send_byte(DS1307_ADR_W);
	/*передаем адрес регистра ds1307*/
	i2c_send_byte(adr);
	/*передаем данные или пропускаем*/
	if (data != RTC_SET_POINTER)
	{
		/*это чтобы привести данные к BCD формату*/
		data = ((data/10)<<4) + data%10;
		i2c_send_byte(data);
	}
}

/************************************************************************/
/*            Чтение ОДНОГО байта по указанному адресу                  */
/************************************************************************/
uint8_t RTC_get(uint8_t address)
{
	uint8_t data=0;
	RTC_SetValue(address, RTC_SET_POINTER); //отправляем адрес регистра по которому хотим читать данные
	/*формируем состояние СТАРТ*/
	i2c_start_cond();
	/*выдаем на шину пакет SLA-R*/
	i2c_send_byte(DS1307_ADR_R);  
	/*чтение с подтверждением*/
	data=i2c_get_byte(0);  //1
	i2c_stop_cond();
	return data;
	
}

// uint8_t RTC_GetValue(uint8_t ack)
// {
// 	uint8_t data=0;
// 	
// 	if (ack==0)
// 	{
// 		RTC_SetValue(0, RTC_RESET_POINTER); //сброс указателя адреса
// 		/*формируем состояние СТАРТ*/
// 		i2c_start_cond();
// 		/*выдаем на шину пакет SLA-R*/
// 		i2c_send_byte(DS1307_ADR_R);
// 		/*чтение с подтверждением*/
// 		data=i2c_get_byte(0);
// 	}
// 	if (ack==1){ 
// 		/*чтение с подтверждением*/
// 		data=i2c_get_byte(0);
// 	}
// 	if (ack==2){
// 		/*чтение без подтверждения*/
// 		data=i2c_get_byte(1);
// 		/*формируем состояние СТОП*/
// 		i2c_stop_cond();
// 	};
// 	
// 	return data;
// }





