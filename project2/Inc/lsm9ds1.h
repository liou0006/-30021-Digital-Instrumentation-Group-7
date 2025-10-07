#ifndef _LSM9DS1_H_
#define _LSM9DS1_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>
#include "30010_io.h"
#include "lcd.h"

void initSPI();
void init_spi_gyro_accel();
uint8_t spi2_transfer(uint8_t data);
void enableCSPins();

#endif
