#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "interrupt.h"
#include "timer.h"
#include "window.h"

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	initJoystick();
	initLED();
	iniEXTIA4();
	initTimer();

	// --------------- TIMER ---------------
stopWatchFunction();
uint8_t stopWatchFlag = 0;

	while(1) {
		stopWatch(&stopWatchFlag);
		printf("\033[3;20H"); // moves cursor to row 3 column 20
		printTime();
	}
}
