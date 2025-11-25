#ifndef TIMER_H_
#define TIMER_H_


/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>
#include "joystick.h"
#include "window.h"

typedef struct {
	volatile uint8_t hours, minutes, seconds, hundredths;
} timestruct_t;



/* Exported functions ------------------------------------------------------- */
void initTimer();
void TIM2_IRQHandler(void);
void stopWatch(uint8_t *flag);
void printTime();
void stopWatchFunction();

#endif /* TIMER_H_ */
