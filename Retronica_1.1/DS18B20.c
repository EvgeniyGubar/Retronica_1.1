/*
 * DS18B20.c
 *
 * Created: 22.01.2022 17:31:35
 *  Author: Evgeniy
 */ 
#include "DS18B20.h"

/* Определяем куда подключен датчик */
#define THERM_PORT PORTD
#define THERM_DDR DDRD
#define THERM_PIN PIND
#define THERM_DQ PD4
/* Макросы для "дергания ноги" и изменения режима ввода/вывода */
#define THERM_INPUT_MODE() THERM_DDR&=~(1<<THERM_DQ)
#define THERM_OUTPUT_MODE() THERM_DDR|=(1<<THERM_DQ)
#define THERM_LOW() THERM_PORT&=~(1<<THERM_DQ)
#define THERM_HIGH() THERM_PORT|=(1<<THERM_DQ)

// команды управления датчиком
#define THERM_CMD_CONVERTTEMP 0x44
#define THERM_CMD_RSCRATCHPAD 0xbe
#define THERM_CMD_WSCRATCHPAD 0x4e
#define THERM_CMD_CPYSCRATCHPAD 0x48
#define THERM_CMD_RECEEPROM 0xb8
#define THERM_CMD_RPWRSUPPLY 0xb4
#define THERM_CMD_SEARCHROM 0xf0
#define THERM_CMD_READROM 0x33
#define THERM_CMD_MATCHROM 0x55
#define THERM_CMD_SKIPROM 0xcc
#define THERM_CMD_ALARMSEARCH 0xec

#define THERM_DECIMAL_STEPS_12BIT 625

static uint8_t therm_reset(void);            // сброс датчика
static void therm_write_bit(uint8_t bit);    // функция отправки бита
static uint8_t therm_read_bit(void);         // чтение бита
static uint8_t therm_read_byte(void);        // читаем байт
static void therm_write_byte(uint8_t byte);  // отправляем байт

/************************************************************************/
/*				  Команда на преобразование температуры				    */
/************************************************************************/
uint8_t therm_start_convert(void)
{	
	if (!therm_reset()) //если датчик найден, то показать температуру
	{
		therm_write_byte(THERM_CMD_SKIPROM);  //обращаемся к датчику с любым ROM
		therm_write_byte(THERM_CMD_CONVERTTEMP);  //команда на преобразование
		return 1;
	}
	else	return 0;    //если датчик не найдет, то вернуть 0
}
/************************************************************************/
/*				Чтение температуры и форматирование данных		        */
/************************************************************************/
void therm_read_temperature(uint16_t *temperature)  //датчик 1-wire ds18b20
{
	uint8_t LSB,MSB;
	
	therm_reset(); //сброс для прекращения передачи данных
	therm_write_byte(THERM_CMD_SKIPROM);
	therm_write_byte(THERM_CMD_RSCRATCHPAD);  //чтение памяти
	LSB = therm_read_byte();
	MSB = therm_read_byte();
	therm_reset(); //сброс для прекращения передачи данных

	if ((MSB & 0x80) != 0)  //если отрицательная темп. то показать нули
	{
		*temperature = 0;
		return;
	}				//+500 - округление в большую сторону
	*temperature = (((LSB & 0x0F)*THERM_DECIMAL_STEPS_12BIT+500)/1000) + ((((LSB & 0xF0)>>4)|(MSB<<4)) *10); //преобразование в десятые градусов 25,5 = 255  
	return;
}

// сброс датчика
static uint8_t therm_reset(void)
{	 
	uint8_t i;
	cli(); //запретить прерывания
	// опускаем ногу вниз на 480uS
	THERM_LOW();
	THERM_OUTPUT_MODE();
	_delay_us(480);
	// подымаем линию на 60uS
	THERM_INPUT_MODE();
	_delay_us(60);
	// получаем значение на линии в период 480uS
	i=(THERM_PIN & (1<<THERM_DQ));
	_delay_us(420); //- по даташиту 420
	// возвращаем значение (0=OK, 1=датчик не найден)
	sei();
	return i;
}

// функция отправки бита
static void therm_write_bit(uint8_t bit){
	// опускаем на 1uS
	cli();  //запретить прерывания
	THERM_LOW();
	THERM_OUTPUT_MODE();
	_delay_us(1);
	// если хотим отправить 1, поднимаем линию (если нет, оставляем как есть)
	if(bit) THERM_INPUT_MODE();
	// ждем 60uS и поднимаем линию
	_delay_us(60);
	THERM_INPUT_MODE();
	sei();
}

// чтение бита
static uint8_t therm_read_bit(void){
	uint8_t bit=0;
	cli();  //запретить прерывания
	// опускаем на 1uS
	THERM_LOW();
	THERM_OUTPUT_MODE();
	_delay_us(1);
	// поднимаем на 14uS
	THERM_INPUT_MODE();
	_delay_us(14);
	// читаем состояние
	if(THERM_PIN&(1<<THERM_DQ)) bit=1;
	// ждем 45 мкс и возвращаем значение
	_delay_us(45);
	sei();
	return bit;
}

// читаем байт
static uint8_t therm_read_byte(void){
	uint8_t i=8, n=0;
	while(i--){
		// сдвигаем в право на 1 и сохраняем следующее значение
		n>>=1;
		n|=(therm_read_bit()<<7);
	}
	return n;
}

// отправляем байт
static void therm_write_byte(uint8_t byte){
	uint8_t i=8;
	while(i--){
		// отправляем бит и сдвигаем вправо на 1
		therm_write_bit(byte&1);
		byte>>=1;
		_delay_us(1);
	}
}

