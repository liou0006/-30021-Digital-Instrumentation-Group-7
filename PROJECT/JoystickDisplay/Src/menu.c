#include "menu.h"

#include "joystick.h"
#include "lcd.h"
#include "lcd_graphics.h"
#include "sensors.h"

// Current menu
static menu_state_t currentMenu = MENU_MAIN;
static uint8_t sel = 0;			// Selected line
static uint8_t joystick = 0;	// Joystick state (default is none)

// Remember cursor position for each menu
static uint8_t sel_main = 0;
static uint8_t sel_sensor = 0;
static uint8_t sel_axis = 0;

// Remember menu info
static sensor_t currentSensor;
static axis_t currentAxis;
static uint8_t FFTmode = 0;		// 1 = FFT, 0 = Histogram

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
		sel_main = 0;
		sel_sensor = 0;
		sel_axis = 0;

		if (joystick == UP && sel > 0) sel--;
		else if (joystick == DOWN && sel < 1) sel++;
		else if (joystick == CENTER) {
			sel_main = sel;		// Save main menu cursor

			if (sel == 0) {
				currentMenu = MENU_FFT;
				FFTmode = 1;
			} else {
				currentMenu = MENU_HIST;
				FFTmode = 0;
			}

			sel = sel_sensor;	// Restore previous cursor in sensor menu
			wait = 1;
		}
		lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
		lcd_write_string((uint8_t *)"Main Menu:", lcdBuffer, 0, 0);
		lcd_write_string((uint8_t*)(sel == 0 ? ">FFT" : " FFT"), lcdBuffer, 0, 1);
		lcd_write_string((uint8_t*)(sel == 1 ? ">Histogram" : " Histogram"), lcdBuffer, 0, 2);
		break;

	case MENU_FFT:
	case MENU_HIST:
		sel_axis = 0;

		if (joystick == LEFT) {
			currentMenu = MENU_MAIN;
			sel = sel_main;		// Restore main menu cursor
			wait = 1;
		} else if (joystick == UP && sel > 0) {
			sel--;
			wait = 1;
		} else if (joystick == DOWN && sel < 2) {
			sel++;
			wait = 1;
		} else if (joystick == CENTER) {
			sel_sensor = sel;	// Save sensor selection cursor

			if (sel == 0) currentSensor = SENSOR_ACCEL;
			else if (sel == 1) currentSensor = SENSOR_GYRO;
			else currentSensor = SENSOR_MAGNET;

			currentMenu = MENU_AXIS;
			sel = sel_axis;		// Restore previous axis cursor
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
			sel = sel_sensor;	// Restore sensor cursor
			wait = 1;
		} else if (joystick == UP && sel > 0) {
			sel--;
			wait = 1;
		} else if (joystick == DOWN && sel < 2) {
			sel++;
			wait = 1;
		} else if (joystick == CENTER) {
			sel_axis = sel;	// Save axis cursor

			if (sel == 0) currentAxis = AXIS_X;
			else if (sel == 1) currentAxis = AXIS_Y;
			else currentAxis = AXIS_Z;

			currentMenu = MENU_PLOT;
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

		if (joystick == LEFT) {
			currentMenu = MENU_AXIS;
			sel = sel_axis;		// Restore axis cursor
			wait = 1;
		}

		lcd_clear_buffer(virtualBuffer, LCD_ROWS * VIRTUAL_WIDTH_SIZE);
		draw_graph_axis();

		if (FFTmode) {
			plot_fft(currentSensor, currentAxis);
		} else {
			plot_histogram(currentSensor, currentAxis);
		}

		update_lcdBuffer();		// Copy visible window to physical LCD buffer
		break;
	}
}

