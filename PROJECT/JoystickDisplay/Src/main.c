#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "interrupts.h"

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	initLed();
	initJoystick();

	setLed('d');

//	printf("Hello\n");

//	int8_t prev_state = readJoystickState();
//	int8_t current_state;

	while(1) {
//		printf("Hello again\n");
		handleJoystick();


//		current_state = readJoystickState();
//		if (prev_state != current_state) {
////			printf("Joystick state: %u\n", current_state);
//			printJoystickState(current_state);
//			prev_state = current_state;
//		}
	}
}
