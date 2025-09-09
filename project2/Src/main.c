#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "timer.h"
#include "window.h"

int main(void)
{
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	// --------------- TIMER ---------------
	initStopWatchWindow();
	printf("\033[3;1H");	// moves cursor to row 3 column 1
	printf("Time since start:  \n");
	printf("Split time 1:      \n");
	printf("Split time 2:      ");

	initTimer();
	initJoystick();

	uint8_t flag = 0;

	TIM2_IRQHandler();

	while(1) {
		stopWatch(&flag);

		printf("\033[3;20H");	// moves cursor to row 3 column 20
		printTime();
	}

	/*
	initTimer();
	initJoystick();

	uint8_t flag = 0;

	TIM2_IRQHandler();

//	uint8_t prev_sec = timeData.seconds;

	while(1) {
		stopWatch(&flag);
//		if (prev_sec != timeData.seconds) {
//			printTime();
//			prev_sec = timeData.seconds;
//		}
		printTime();
	}
	// -------------------------------------
	 */
}
