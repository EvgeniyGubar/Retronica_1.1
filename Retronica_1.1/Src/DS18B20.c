/*
 * DS18B20.c
 *
 * Created: 22.01.2022 17:31:35
 *  Author: Evgeniy
 */ 
#include "DS18B20.h"

/* ���������� ���� ��������� ������ */
#define THERM_PORT PORTD
#define THERM_DDR DDRD
#define THERM_PIN PIND
#define THERM_DQ PD4
/* ������� ��� "�������� ����" � ��������� ������ �����/������ */
#define THERM_INPUT_MODE() THERM_DDR&=~(1<<THERM_DQ)
#define THERM_OUTPUT_MODE() THERM_DDR|=(1<<THERM_DQ)
#define THERM_LOW() THERM_PORT&=~(1<<THERM_DQ)
#define THERM_HIGH() THERM_PORT|=(1<<THERM_DQ)

// ������� ���������� ��������
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

static uint8_t therm_reset(void);            // ����� �������
static void therm_write_bit(uint8_t bit);    // ������� �������� ����
static uint8_t therm_read_bit(void);         // ������ ����
static uint8_t therm_read_byte(void);        // ������ ����
static void therm_write_byte(uint8_t byte);  // ���������� ����

/************************************************************************/
/*				  ������� �� �������������� �����������				    */
/************************************************************************/
uint8_t therm_start_convert(void)
{	
	if (!therm_reset()) //���� ������ ������, �� �������� �����������
	{
		therm_write_byte(THERM_CMD_SKIPROM);  //���������� � ������� � ����� ROM
		therm_write_byte(THERM_CMD_CONVERTTEMP);  //������� �� ��������������
		return 1;
	}
	else	return 0;    //���� ������ �� ������, �� ������� 0
}
/************************************************************************/
/*				������ ����������� � �������������� ������		        */
/************************************************************************/
void therm_read_temperature(uint16_t *temperature)  //������ 1-wire ds18b20
{
	uint8_t LSB,MSB;
	
	therm_reset(); //����� ��� ����������� �������� ������
	therm_write_byte(THERM_CMD_SKIPROM);
	therm_write_byte(THERM_CMD_RSCRATCHPAD);  //������ ������
	LSB = therm_read_byte();
	MSB = therm_read_byte();
	therm_reset(); //����� ��� ����������� �������� ������

	if ((MSB & 0x80) != 0)  //���� ������������� ����. �� �������� ����
	{
		*temperature = 0;
		return;
	}				//+500 - ���������� � ������� �������
	*temperature = (((LSB & 0x0F)*THERM_DECIMAL_STEPS_12BIT+500)/1000) + ((((LSB & 0xF0)>>4)|(MSB<<4)) *10); //�������������� � ������� �������� 25,5 = 255  
	return;
}

// ����� �������
static uint8_t therm_reset(void)
{	 
	uint8_t i;
	cli(); //��������� ����������
	// �������� ���� ���� �� 480uS
	THERM_LOW();
	THERM_OUTPUT_MODE();
	_delay_us(480);
	// �������� ����� �� 60uS
	THERM_INPUT_MODE();
	_delay_us(60);
	// �������� �������� �� ����� � ������ 480uS
	i=(THERM_PIN & (1<<THERM_DQ));
	_delay_us(420); //- �� �������� 420
	// ���������� �������� (0=OK, 1=������ �� ������)
	sei();
	return i;
}

// ������� �������� ����
static void therm_write_bit(uint8_t bit){
	// �������� �� 1uS
	cli();  //��������� ����������
	THERM_LOW();
	THERM_OUTPUT_MODE();
	_delay_us(1);
	// ���� ����� ��������� 1, ��������� ����� (���� ���, ��������� ��� ����)
	if(bit) THERM_INPUT_MODE();
	// ���� 60uS � ��������� �����
	_delay_us(60);
	THERM_INPUT_MODE();
	sei();
}

// ������ ����
static uint8_t therm_read_bit(void){
	uint8_t bit=0;
	cli();  //��������� ����������
	// �������� �� 1uS
	THERM_LOW();
	THERM_OUTPUT_MODE();
	_delay_us(1);
	// ��������� �� 14uS
	THERM_INPUT_MODE();
	_delay_us(14);
	// ������ ���������
	if(THERM_PIN&(1<<THERM_DQ)) bit=1;
	// ���� 45 ��� � ���������� ��������
	_delay_us(45);
	sei();
	return bit;
}

// ������ ����
static uint8_t therm_read_byte(void){
	uint8_t i=8, n=0;
	while(i--){
		// �������� � ����� �� 1 � ��������� ��������� ��������
		n>>=1;
		n|=(therm_read_bit()<<7);
	}
	return n;
}

// ���������� ����
static void therm_write_byte(uint8_t byte){
	uint8_t i=8;
	while(i--){
		// ���������� ��� � �������� ������ �� 1
		therm_write_bit(byte&1);
		byte>>=1;
		_delay_us(1);
	}
}

