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
extern volatile uint32_t tim2_hits;
extern volatile uint8_t flag;

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud


	GPIO_set_AF1_PA0();
	while(1) {

		if (flag == 1){
			float measFreq = freq;
			float measDuty = duty;
			flag = 0;

			printf("freq: %f | duty: %f\n",measFreq,measDuty);
		}
		//printf("irq: %u\n",tim2_hits);

		//		printf("TIM_GetCapture1 value = %ld\n", TIM_GetCapture1(TIM2));
		//		printf("Meas. freq.: %f\n", measuredFreq);
		//		printf("Meas. duty: %f\n\n", measuredDuty);
	}
}
