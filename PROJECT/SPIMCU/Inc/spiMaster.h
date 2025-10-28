/*
 * spiMaster.h
 *
 *  Created on: Oct 28, 2025
 *      Author: liou-
 */

#ifndef SPIMASTER_H_
#define SPIMASTER_H_


/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>
#include "30010_io.h"
#include "lcd.h"

void init_SPI_CS();
uint8_t spi2_transfer(uint8_t data);

int16_t readOutput(uint8_t lowReg);

uint8_t readAG(uint8_t reg);
void writeAG(uint8_t reg, uint8_t data);

void printGyroXYZ();
void printAccelXYZ();
void readTempteratureC();

uint8_t readM(uint8_t reg);
void writeM(uint8_t reg, uint8_t data);

void printMagnetXYZ();

void initAG();
void initMag();





#endif /* SPIMASTER_H_ */
