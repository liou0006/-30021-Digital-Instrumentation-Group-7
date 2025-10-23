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


	extern volatile uint32_t capture1;
	extern volatile uint32_t capture2;
	extern volatile float freq;
	extern volatile float duty;
	extern volatile float high_us;
	extern volatile uint32_t tim2_hits;
	extern volatile uint8_t flag;

	int main(void) {
		uart_init( 9600 ); // Initialize USB serial at 9600 baud

		TimeICInit();

		while(1) {
			if (flag == 1){
				flag = 0;

	//			printf("CC1: %.0f | CC2: %.0f | freq: %f | duty %f\n", TIM_GetCapture1(TIM2)*1.0f, TIM_GetCapture2(TIM2)*1.0f, freq, duty);
				printf("cap1: %u | cap2: %u | freq: %f | duty: %f | high_us: %f\n", capture1, capture2,freq, duty, high_us);
			}
		}
	}
