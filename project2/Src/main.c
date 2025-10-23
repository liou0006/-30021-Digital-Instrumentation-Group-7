#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "lcd.h"
#include "interrupt.h"
#include "timer.h"
#include "window.h"
#include "flash.h"
//#include "lsm9ds1.h"
#include "afg2021.h"

extern volatile float freq;
extern volatile float duty;
extern volatile uint8_t readCapture;

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	TimeICInit();
	initJoystick();

	while(1) {
		joystickIC();
		if (readCapture == 1){
			readCapture = 0;
			printf("period: %.1f | freq: %f | duty: %f \n", 1e6f/freq, freq, duty);
		}
		else{
			printf("Paused\n");
		}


	}
}
