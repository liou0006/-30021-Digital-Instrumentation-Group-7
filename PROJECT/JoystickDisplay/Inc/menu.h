#ifndef MENU_H_
#define MENU_H_

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course

#include <stdint.h>
#include "plot.h"

typedef enum {
	MENU_MAIN,
	MENU_COLLECT,
	MENU_FFT,
	MENU_HIST,
	MENU_PLOT,
	MENU_AXIS
} menu_state_t;

void menu_init();
void menu_update();

#endif /* MENU_H_ */
