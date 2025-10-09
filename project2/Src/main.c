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
	initAG();
	initMag();

	while(1) {

		//		printGyroXYZ();
		//		printAccelXYZ();

		printMagnetXYZ();

		//		uint8_t val = readM(0x0F);
		//
		//		printf("WHOAMI reg = %02x\n",val);

		for (int i= 0 ; i<= 10000;i++); // hold to make the signal not osciliate very fast

	}
}
