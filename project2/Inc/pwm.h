#ifndef PWM_H_
#define PWM_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>

/* Exported functions ------------------------------------------------------- */
void timer16_pwm_init();
void GPIO_set_AF1_PA12();

#endif /* PWM_H_ */
