#ifndef PWM_H_
#define PWM_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>
#include "adc.h"

/* Exported functions ------------------------------------------------------- */
void timer16_pwm_init();
void GPIO_set_AF1_PA12();
void GPIO_set_AF1_PA6();
void setDutyCycle(int dutycycle);
void regulate_voltage(float desired_voltage)

#endif /* PWM_H_ */
