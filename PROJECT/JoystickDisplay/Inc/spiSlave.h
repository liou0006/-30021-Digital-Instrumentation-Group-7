/*
 * spiSlave.h
 *
 *  Created on: Oct 28, 2025
 *      Author: liou-
 */

#ifndef SPISLAVE_H_
#define SPISLAVE_H_

#include "stm32f30x_conf.h"

void initSlaveSPI();
void initCSPins();
uint8_t spi3_transfer(uint8_t data);
uint8_t readAGSPI3(uint8_t reg);

void iniPB12();
#endif /* SPISLAVE_H_ */
