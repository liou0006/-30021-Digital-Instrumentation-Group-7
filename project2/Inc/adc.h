#ifndef ADC_H_
#define ADC_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>

/* Exported functions ------------------------------------------------------- */
void ADC_setup_PA(void);
uint16_t ADC_measure_PA(uint8_t ch);

void ADC_Setup_VREFEN();
uint16_t ADC_measure_VREF();

#endif /* ADC_H_ */
