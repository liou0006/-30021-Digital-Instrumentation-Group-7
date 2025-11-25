#ifndef _LSM9DS1_H_
#define _LSM9DS1_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>

void init_SPI_CS();

uint8_t spi2_transfer(uint8_t data) ;

int16_t readOutputAG(uint8_t lowReg);
int16_t readOutputM(uint8_t lowReg);

uint8_t readAG(uint8_t reg);
uint8_t readM(uint8_t reg);

void writeAG(uint8_t reg, uint8_t data);
void writeM(uint8_t reg, uint8_t data);

void printTempteratureC();
void printGyroXYZ();
void printAccelXYZ();
void printMagnetXYZ();

void initAG();
void initMag();

#endif
