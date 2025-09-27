#ifndef PWM_H_
#define PWM_H_

#include "stm32f30x_conf.h"
#include <stdint.h>
#include "adc.h"

// init two independent servo outputs:
//  - TIM16_CH1 on PA6 (AF1)
//  - TIM17_CH1 on PB9 (AF10)
void timer16_pwm_init(void);
void timer17_pwm_init(void);

// set pulse width in microseconds (clamped to 1000..2000 by default)
void setServoPulse_TIM16(uint16_t us);
void setServoPulse_TIM17(uint16_t us);

// read two pots and update both servos
void update_servos(void);

#endif /* PWM_H_ */
