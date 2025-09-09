#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"

int main(void)
{
	uart_init( 9600 ); // Initialize USB serial at 9600 baud
	initJoystick();
	initLed();

	while(1){
	handleJoystick();

	}
}
