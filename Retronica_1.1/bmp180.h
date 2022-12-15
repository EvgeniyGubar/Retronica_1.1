
#ifndef BMP180_H_
#define BMP180_H_

#include <avr/io.h>

void BMP180_init(void); 

uint8_t BMP180_get_temp_press(uint8_t param, uint32_t* result);

#endif /* BMP180_H_ */