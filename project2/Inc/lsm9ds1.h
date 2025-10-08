#ifndef _LSM9DS1_H_
#define _LSM9DS1_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>
#include "30010_io.h"
#include "lcd.h"

void initSPI();
void init_spi_gyro_accel();
void spi2_transfer(uint8_t data);
void enableCSPins();
void readSPI2(uint8_t reg, uint16_t PIN);
void writeSPI2(uint8_t reg, uint16_t PIN);
uint8_t readSPI2_v2(uint8_t reg, uint16_t PIN);

#endif
