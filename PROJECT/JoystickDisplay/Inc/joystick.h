#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course

#include "led.h"

void initJoystick();
int8_t readJoystickState();
void handleJoystick();

#endif /* JOYSTICK_H_ */
