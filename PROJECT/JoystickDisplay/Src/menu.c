#include "menu.h"

static menu_state_t currentMenu = MENU_MAIN;
static uint8_t sel = 0;			// Selected line
static uint8_t joystick = 0;	// Joystick joystick (default is none)
static sensor_t currentSensor;
static axis_t currentAxis;
static uint8_t FFTmode = 0;		// 1 = FFT, 0 = Histogram
static menu_state_t lastMenu = MENU_MAIN;
static uint8_t lastSel = 255;

// Unimplemented functions
//void displayFFT(sensor_t sensor, axis_t axis);
//void displayHistogram(sensor_t sensor, axis_t axis);

void menu_init(void) {
	lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
	lcd_write_string((uint8_t *)"Main Menu:", lcdBuffer, 0, 0);
	lcd_write_string((uint8_t*)" FFT", lcdBuffer, 0, 1);
	lcd_write_string((uint8_t*)" Histogram", lcdBuffer, 0, 2);
}

menu_state_t menu_get_current_joystick(void) {
	return currentMenu;
}

void menu_update(void) {
	joystick = readJoystickState();

	switch (currentMenu) {
	// ---------- MAIN MENU ----------
	case MENU_MAIN:
		if (joystick == UP && sel > 0) sel--;
		else if (joystick == DOWN && sel < 1) sel++;
		else if (joystick == CENTER) {
			if (sel == 0) {
//				currentMenu = MENU_FFT;
				FFTmode = 1;
				sel = 0;
			} else {
//				currentMenu = MENU_HIST;
				FFTmode = 0;
				sel = 0;
			}
			currentMenu = MENU_SENSOR;
		}
		lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
		lcd_write_string((uint8_t *)"Main Menu:", lcdBuffer, 0, 0);
		lcd_write_string((uint8_t*)(sel == 0 ? ">FFT" : " FFT"), lcdBuffer, 0, 1);
		lcd_write_string((uint8_t*)(sel == 1 ? ">Histogram" : " Histogram"), lcdBuffer, 0, 2);
		break;

	// ---------- SENSOR SELECT ----------
//	case MENU_FFT:
	case MENU_SENSOR:
		if (joystick == UP && sel > 0) sel--;
		else if (joystick == DOWN && sel < 2) sel++;

		if (joystick == LEFT) {
			currentMenu = MENU_MAIN;
			sel = 0;
		} else if (joystick == CENTER) {
			if (sel == 0) {
				currentSensor = SENSOR_ACCEL;
			} else if (sel == 1) {
				currentSensor = SENSOR_GYRO;
			} else {
				currentSensor = SENSOR_MAGNET;
			}
			currentMenu = MENU_AXIS;
			sel = 0;
		}
		lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
		lcd_write_string((uint8_t*)(FFTmode ? "FFT: Select Sensor" : "Hist.: Select Sensor"), lcdBuffer, 0, 0);
		lcd_write_string((uint8_t *)(sel == 0 ? ">Accelerometer" : " Accelerometer"), lcdBuffer, 0, 1);
		lcd_write_string((uint8_t *)(sel == 1 ? ">Gyroscope" : " Gyroscope"), lcdBuffer, 0, 2);
		lcd_write_string((uint8_t *)(sel == 2 ? ">Magnetometer" : " Magnetometer"), lcdBuffer, 0, 3);
		break;

	// ---------- PLOT ----------
//	case MENU_PLOT:
//		lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
//
//		if (FFTmode == 0) {}
//		else if (FFTmode == 1) {}
//
//		if (joystick == LEFT) currentMenu = MENU_MAIN;
//		break;
	}

	joystick = 0;	// reset joystick after processing
}
