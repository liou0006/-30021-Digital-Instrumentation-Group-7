#include "menu.h"

static menu_state_t currentMenu = MENU_MAIN;
static sensor_t currentSensor;
static axis_t currentAxis;
//static uint8_t joystickState = 0;
static uint8_t state = 0;
static FFTmode = 0;

void menu_init(void) {
	draw_menu_main();
}

void menu_set_joystick_state(uint8_t state) {
//	joystickState = state;
}

menu_state_t menu_get_current_state(void) {
//	return currentMenu;
}

void menu_update(void) {
	state = readJoystickState();
	uint8_t sel = 0;

	if (state == UP && sel > 0) sel--;
	else if (state == DOWN && sel < 2) sel++;

	switch (currentMenu) {
	case MENU_MAIN:
		draw_menu_main();
		if (state == UP) currentMenu = MENU_FFT;
		else if (state == DOWN) currentMenu = MENU_HIST;
		break;

	case MENU_FFT:
		draw_menu_fft(sel);
		if (state == LEFT) currentMenu = MENU_MAIN;
		break;

	case MENU_HIST:
		draw_menu_histogram(sel);
		if (state == LEFT) currentMenu = MENU_MAIN;
		break;

	case MENU_PLOT:
		lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);

		if (FFTmode == 0) {}
		else if (FFTmode == 1) {}

		if (state == LEFT) currentMenu = MENU_MAIN;
		break;
	}

	state = 0;	// reset state after processing
}

void draw_menu_main(void) {
	lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
	lcd_write_string((uint8_t *)"Main Menu:", lcdBuffer, 0, 0);
	lcd_write_string((uint8_t *)"  FFT", lcdBuffer, 0, 1);
	lcd_write_string((uint8_t *)"  Histogram", lcdBuffer, 0, 2);
}

void draw_menu_fft(uint8_t sel) {
	lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);

	lcd_write_string((uint8_t *)"FFT Plot:", lcdBuffer, 0, 0);
	lcd_write_string((uint8_t *)(sel == 0 ? ">Accelerometer" : " Accelerometer"), lcdBuffer, 0, 1);
	lcd_write_string((uint8_t *)(sel == 1 ? ">Gyroscope" : " Gyroscope"), lcdBuffer, 0, 2);
	lcd_write_string((uint8_t *)(sel == 2 ? ">Magnetometer" : " Magnetometer"), lcdBuffer, 0, 3);
}

void draw_menu_histogram(uint8_t sel) {
	lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);

	lcd_write_string((uint8_t *)"Histogram:", lcdBuffer, 0, 0);
	lcd_write_string((uint8_t *)(sel == 0 ? ">Accelerometer" : " Accelerometer"), lcdBuffer, 0, 1);
	lcd_write_string((uint8_t *)(sel == 1 ? ">Gyroscope" : " Gyroscope"), lcdBuffer, 0, 2);
	lcd_write_string((uint8_t *)(sel == 2 ? ">Magnetometer" : " Magnetometer"), lcdBuffer, 0, 3);
}
