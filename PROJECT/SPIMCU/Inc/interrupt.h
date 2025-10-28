/*
 * interrupt.h
 *
 *  Created on: Sep 9, 2025
 *      Author: liou-
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h"
#include "led.h"

void iniEXTIA4();
void EXTI4_IRQHandler();

#endif /* INTERRUPT_H_ */
