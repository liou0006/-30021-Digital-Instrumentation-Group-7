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


int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud


	GPIO_set_AF1_PA0();
	while(1) {
//		printf("TIM_GetCapture1 value = %ld\n", TIM_GetCapture1(TIM2));
//		printf("Meas. freq.: %f\n", measuredFreq);
//		printf("Meas. duty: %f\n\n", measuredDuty);
	}
}
