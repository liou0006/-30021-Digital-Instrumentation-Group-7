#include "menu.h"

static menu_state_t currentMenu = MENU_MAIN;
static uint8_t sel = 0;			// Selected line
static uint8_t joystick = 0;	// Joystick joystick (default is none)
static sensor_t currentSensor;
static axis_t currentAxis;
static uint8_t FFTmode = 0;		// 1 = FFT, 0 = Histogram

// Unimplemented functions
//void displayFFT(sensor_t sensor, axis_t axis);
//void displayHistogram(sensor_t sensor, axis_t axis);

void menu_init() {
	lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
	lcd_write_string((uint8_t *)"Main Menu:", lcdBuffer, 0, 0);
	lcd_write_string((uint8_t*)" FFT", lcdBuffer, 0, 1);
	lcd_write_string((uint8_t*)" Histogram", lcdBuffer, 0, 2);
}

void menu_update() {
	static uint8_t wait = 0;
	joystick = readJoystickState();

	if (wait) {
		if (joystick == 0) wait = 0;
		joystick = 0;
	}

	switch (currentMenu) {
	case MENU_MAIN:
		if (joystick == UP && sel > 0) sel--;
		else if (joystick == DOWN && sel < 1) sel++;
		else if (joystick == CENTER) {
			if (sel == 0) {
				currentMenu = MENU_FFT;
				FFTmode = 1;
			} else {
				currentMenu = MENU_HIST;
				FFTmode = 0;
			}
			sel = 0;
			wait = 1;
		}
		lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
		lcd_write_string((uint8_t *)"Main Menu:", lcdBuffer, 0, 0);
		lcd_write_string((uint8_t*)(sel == 0 ? ">FFT" : " FFT"), lcdBuffer, 0, 1);
		lcd_write_string((uint8_t*)(sel == 1 ? ">Histogram" : " Histogram"), lcdBuffer, 0, 2);
		break;

	case MENU_FFT:
	case MENU_HIST:
		if (joystick == LEFT) {
			currentMenu = MENU_MAIN;
			sel = 0;
			wait = 1;
		} else if (joystick == UP && sel > 0) {
			sel--;
			wait = 1;
		} else if (joystick == DOWN && sel < 2) {
			sel++;
			wait = 1;
		} else if (joystick == CENTER) {
			if (sel == 0) currentSensor = SENSOR_ACCEL;
			else if (sel == 1) currentSensor = SENSOR_GYRO;
			else currentSensor = SENSOR_MAGNET;
			currentMenu = MENU_AXIS;
			sel = 0;
			wait = 1;
		}
		lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
		lcd_write_string((uint8_t*)(FFTmode ? "FFT: Select Sensor" : "Hist.: Select Sensor"), lcdBuffer, 0, 0);
		lcd_write_string((uint8_t *)(sel == 0 ? ">Accelerometer" : " Accelerometer"), lcdBuffer, 0, 1);
		lcd_write_string((uint8_t *)(sel == 1 ? ">Gyroscope" : " Gyroscope"), lcdBuffer, 0, 2);
		lcd_write_string((uint8_t *)(sel == 2 ? ">Magnetometer" : " Magnetometer"), lcdBuffer, 0, 3);
		break;

	case MENU_AXIS:
		if (joystick == LEFT) {
			currentMenu = (FFTmode ? MENU_FFT : MENU_HIST);
			sel = 0;
			wait = 1;
		} else if (joystick == UP && sel > 0) {
			sel--;
			wait = 1;
		} else if (joystick == DOWN && sel < 2) {
			sel++;
			wait = 1;
		} else if (joystick == CENTER) {
			if (sel == 0) currentAxis = AXIS_X;
			else if (sel == 1) currentAxis = AXIS_Y;
			else currentAxis = AXIS_Z;
			currentMenu = MENU_PLOT;
			sel = 0;
			wait = 1;
		}
		lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
		lcd_write_string((uint8_t*)(FFTmode ? "FFT: Select Axis" : "Hist.: Select Axis"), lcdBuffer, 0, 0);
		lcd_write_string((uint8_t*)(sel == 0 ? ">X" : " X"), lcdBuffer, 0, 1);
		lcd_write_string((uint8_t*)(sel == 1 ? ">Y" : " Y"), lcdBuffer, 0, 2);
		lcd_write_string((uint8_t*)(sel == 2 ? ">Z" : " Z"), lcdBuffer, 0, 3);
		break;

	case MENU_PLOT:
		lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
		lcd_write_string((uint8_t*)"MENU_PLOT", lcdBuffer, 0, 0);
		if (joystick == LEFT) {
			currentMenu = MENU_AXIS;
			wait = 1;
		}
		lcd_clear_buffer(virtualBuffer, LCD_ROWS * VIRTUAL_WIDTH_SIZE);
		draw_graph_axis();
		// Copy visible window to physical LCD buffer
		update_lcdBuffer();
		break;
	}
}
