#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "led.h"

#define UP		(0x01)
#define DOWN	(0x02)
#define LEFT	(0x04)
#define RIGHT	(0x08)
#define CENTER	(0x10)

void initJoystick();
int8_t readJoystickState();
void handleJoystick();

#endif /* JOYSTICK_H_ */
