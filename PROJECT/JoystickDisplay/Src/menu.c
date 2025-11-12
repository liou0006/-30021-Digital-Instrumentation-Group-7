#include "menu.h"

static MenuState currentMenu = MENU_MAIN;
static uint8_t joystickState = 0;

void menu_init(void) {
	lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
	lcd_write_string((uint8_t *)"Main Menu:", lcdBuffer, 0, 0);
	lcd_write_string((uint8_t *)" FFT", lcdBuffer, 0, 1);
	lcd_write_string((uint8_t *)" Histogram", lcdBuffer, 0, 2);
}

void menu_set_joystick_state(uint8_t state) {
	joystickState = state;
}

MenuState menu_get_current_state(void) {
	return currentMenu;
}

void menu_update(void) {
	switch (currentMenu) {
	case MENU_MAIN:
		draw_menu_main();
		if (joystickState == UP) currentMenu = MENU_FFT;
		else if (joystickState == DOWN) currentMenu = MENU_HISTOGRAM;
		break;

	case MENU_FFT:
		draw_menu_fft();
		if (joystickState == LEFT) currentMenu = MENU_MAIN;
		break;

	case MENU_HISTOGRAM:
		draw_menu_histogram();
		if (joystickState == LEFT) currentMenu = MENU_MAIN;
		break;
	}

	joystickState = 0;	// reset state after processing
}

void draw_menu_main(void) {
	lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
	lcd_write_string((uint8_t *)"Main Menu:", lcdBuffer, 0, 0);
	lcd_write_string((uint8_t *)" FFT", lcdBuffer, 0, 1);
	lcd_write_string((uint8_t *)" Histogram", lcdBuffer, 0, 2);
}

void draw_menu_fft(void) {
	lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
	lcd_write_string((uint8_t *)"FFT Menu:", lcdBuffer, 0, 0);
	lcd_write_string((uint8_t *)" accel", lcdBuffer, 0, 1);
	lcd_write_string((uint8_t *)" gyro", lcdBuffer, 0, 2);
	lcd_write_string((uint8_t *)" magnet", lcdBuffer, 0, 3);
}

void draw_menu_histogram(void) {
	lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
	lcd_write_string((uint8_t *)"Histogram Menu:", lcdBuffer, 0, 0);
	lcd_write_string((uint8_t *)" accel", lcdBuffer, 0, 1);
	lcd_write_string((uint8_t *)" gyro", lcdBuffer, 0, 2);
	lcd_write_string((uint8_t *)" magnet", lcdBuffer, 0, 3);
}
