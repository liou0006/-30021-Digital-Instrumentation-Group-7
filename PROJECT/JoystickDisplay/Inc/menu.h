#ifndef MENU_H_
#define MENU_H_

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "lcd.h"
#include "lcd_graphics.h"
#include "joystick.h"

typedef enum {
	MENU_MAIN,
	MENU_FFT,
	MENU_HIST,
	MENU_PLOT,
	MENU_SENSOR,
	MENU_AXIS
} menu_state_t;

typedef enum {
	SENSOR_ACCEL,
	SENSOR_GYRO,
	SENSOR_MAGNET
} sensor_t;

typedef enum {
	AXIS_X,
	AXIS_Y,
	AXIS_Z
} axis_t;

void menu_init();
void menu_update();

#endif /* MENU_H_ */
