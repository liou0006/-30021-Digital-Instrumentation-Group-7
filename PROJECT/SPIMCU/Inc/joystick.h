/*
 * joystick.h
 *
 *  Created on: Sep 6, 2025
 *      Author: liou-
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course


void initJoystick();
int8_t readJoystick();
void handleJoystick();


#endif /* JOYSTICK_H_ */
