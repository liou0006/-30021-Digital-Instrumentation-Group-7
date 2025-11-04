#ifndef TIMER_H_
#define TIMER_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>
#include "joystick.h"

typedef struct {
	volatile uint8_t hours, minutes, seconds, hundredths;
} timestruct_t;

extern timestruct_t timeData;

/* Exported functions ------------------------------------------------------- */
void initTimer();
void TIM2_IRQHandler(void);

#endif /* TIMER_H_ */
