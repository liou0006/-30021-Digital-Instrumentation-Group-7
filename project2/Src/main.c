#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "lcd.h"
#include "interrupt.h"
#include "timer.h"
#include "window.h"
#include "flash.h"
#include "lsm9ds1.h"

#define WHO_AM_I     0x0F


int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	//	initJoystick();
	//	initLed();
	//	iniEXTIA4();
	initTimer();
	//	lcd_init_and_print();

	init_SPI_CS();

	//enable Gyroscope
	writeAG(0x10,0b01100000);

	//enable Accelerometer
	writeAG(0x19,0b01111000);
	writeAG(0x20,0b01000000);


	while(1) {

//		printGyroXYZ();
		printAccelXYZ();

		for (int i= 0 ; i<= 10000;i++); // hold to make the signal not osciliate very fast

	}
}
