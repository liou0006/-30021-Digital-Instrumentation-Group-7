#ifndef LED_H_
#define LED_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>

/* Exported functions ------------------------------------------------------- */
void initLed();
void setLed(char sel);

#endif /* LED_H_ */
