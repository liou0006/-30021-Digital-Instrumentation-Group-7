#ifndef _LSM9DS1_H_
#define _LSM9DS1_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>
#include "30010_io.h"
#include "lcd.h"

void init_SPI_CS();
uint8_t spi2_transfer(uint8_t data);

uint8_t readAG(uint8_t reg);
void writeAG(uint8_t reg, uint8_t data);

int16_t readOutput(uint8_t lowReg);
void printGyroXYZ();
void printAccelXYZ();

uint8_t readM(uint8_t reg);


#endif
