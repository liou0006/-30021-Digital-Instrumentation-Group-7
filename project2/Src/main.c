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
	initStopWatchWindow();
	printf("\033[3;1H"); // moves cursor to row 3 column 1
	printf("Time since start: \n");
	printf("Split time 1: \n");
	printf("Split time 2: \n");
	TIM2_IRQHandler();
	uint8_t flag = 0;


	int val = 10;
	printf("Value = %02ld\n", val);

	uint8_t a = 10;
	char str[7];
	sprintf(str, "a = %2d", a);

	printf("%s\n",str);

	float floatVal = 1.234;
	printf("%6f\n", floatVal);

	while(1) {
		handleJoystick();
		stopWatch(&flag);
		printf("\033[3;20H"); // moves cursor to row 3 column 20
		printTime();



	}
}
