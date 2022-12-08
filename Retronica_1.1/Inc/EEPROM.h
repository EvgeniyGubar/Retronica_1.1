/*
 * EEPROM.h
 *
 * Created: 23.01.2022 0:02:56
 *  Author: Evgeniy
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "MenuSettings.h"
#include <avr/io.h>
#include <avr/eeprom.h>

void read_eeprom(void);		//Чтение настроек
void save_to_eeprom(void);  //Сохранение изменненых настроек в EEPROM

#endif /* EEPROM_H_ */