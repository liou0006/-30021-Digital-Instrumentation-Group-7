#ifndef _AFG2021_H_
#define _AFG2021_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>
#include "30010_io.h"
#include "joystick.h"
#include "led.h"

void TimeICInit();
void GPIO_set_AF1_PA0();
void TIM2_IRQHandler(void);
void joystickIC();

#endif
