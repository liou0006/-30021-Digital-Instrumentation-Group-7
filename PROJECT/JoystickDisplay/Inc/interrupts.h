#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>

/* Exported functions ------------------------------------------------------- */
void initInterrupts();
void EXTI9_5_IRQHandler(void);

#endif /* INTERRUPTS_H_ */
