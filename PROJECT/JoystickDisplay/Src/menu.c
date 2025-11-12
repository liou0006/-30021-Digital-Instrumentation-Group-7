#include "menu.h"

static menu_state_t currentMenu = MENU_MAIN;
static sensor_t currentSensor;
static axis_t currentAxis;
static uint8_t joystickState = 0;
static FFTmode = 0;

void menu_init(void) {
	draw_menu_main();
}

void menu_set_joystick_state(uint8_t state) {
	joystickState = state;
}

menu_state_t menu_get_current_state(void) {
	return currentMenu;
}

void menu_update(void) {
	switch (currentMenu) {
	case MENU_MAIN:
		draw_menu_main();
		if (joystickState == UP) currentMenu = MENU_FFT;
		else if (joystickState == DOWN) currentMenu = MENU_HIST;
		break;

	case MENU_FFT:
		draw_menu_fft();
		if (joystickState == LEFT) currentMenu = MENU_MAIN;
		break;

	case MENU_HIST:
		draw_menu_histogram();
		if (joystickState == LEFT) currentMenu = MENU_MAIN;
		break;

	case MENU_PLOT:
		lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);

		if (FFTmode == 0) {}
		else if (FFTmode == 1) {}

		if (joystickState == LEFT) currentMenu = MENU_MAIN;
		break;
	}

	joystickState = 0;	// reset state after processing
}

void draw_menu_main(void) {
	lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
	lcd_write_string((uint8_t *)"Main Menu:", lcdBuffer, 0, 0);
	lcd_write_string((uint8_t *)"  FFT", lcdBuffer, 0, 1);
	lcd_write_string((uint8_t *)"  Histogram", lcdBuffer, 0, 2);
}

void draw_menu_fft(void) {
	lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
	lcd_write_string((uint8_t *)"FFT Plot:", lcdBuffer, 0, 0);
	lcd_write_string((uint8_t *)"  Accelerometer", lcdBuffer, 0, 1);
	lcd_write_string((uint8_t *)"  Gyroscope", lcdBuffer, 0, 2);
	lcd_write_string((uint8_t *)"  Magnetometer", lcdBuffer, 0, 3);
}

void draw_menu_histogram(void) {
	lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
	lcd_write_string((uint8_t *)"Histogram:", lcdBuffer, 0, 0);
	lcd_write_string((uint8_t *)"  Accelerometer", lcdBuffer, 0, 1);
	lcd_write_string((uint8_t *)"  Gyroscope", lcdBuffer, 0, 2);
	lcd_write_string((uint8_t *)"  Magnetometer", lcdBuffer, 0, 3);
}
