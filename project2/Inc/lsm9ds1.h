#ifndef _LSM9DS1_H_
#define _LSM9DS1_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>
#include "lcd.h"

void init_spi_gyro_accel();
void spi_transmit(uint8_t data);


#endif
