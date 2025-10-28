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
#include "spiMaster.h"

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	initMasterSPI();

	initAG();
	initMag();

	while(1) {

		readTempteratureC();
		//		printGyroXYZ();
		//		printAccelXYZ();
		//		printMagnetXYZ();

	}
}
