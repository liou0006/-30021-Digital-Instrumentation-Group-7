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
	MENU_HISTOGRAM
} MenuState;

void menu_init(void);
void menu_update(void);
void menu_set_joystick_state(uint8_t state);
MenuState menu_get_current_state(void);

void draw_menu_main(void);
void draw_menu_fft(void);
void draw_menu_histogram(void);

#endif /* MENU_H_ */
