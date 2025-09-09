#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "interrupt.h"

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	initJoystick();
	initLED();
	iniEXTIA4();

	int8_t prev_state = readJoystick();
	int8_t current_state;

	while(1) {
//		current_state = readJoystick();
//		if (prev_state != current_state) {
//			if (current_state == 1) {
////				setLED('r');
//				printf("UP (red)\n");
//			} else if (current_state == 2) {
////				setLED('g');
//				printf("DOWN (green)\n");
//			} else if (current_state == 4) {
////				setLED('b');
//				printf("LEFT (blue)\n");
//			} else if (current_state == 8) {
////				setLED('c');
//				printf("RIGHT (cyan)\n");
//			} else if (current_state == 16) {
////				setLED('m');
//				printf("CENTER (magenta)\n");
//			} else {
//				setLED('d');
//			}
//			prev_state = current_state;
//		}

	}
}
