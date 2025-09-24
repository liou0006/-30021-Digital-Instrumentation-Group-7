#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "lcd.h"
#include "interrupt.h"
#include "timer.h"
#include "window.h"
#include "flash.h"
#include "adc.h"
#include "math.h"

#define VREFINT_CAL *((uint16_t*) ((uint32_t) 0x1FFFF7BA)) //calibrated at 3.3V@ 30


static uint8_t lcdBuffer[LCD_BUFF_SIZE];

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	initJoystick();
	initLed();
	iniEXTIA4();
	initTimer();

	lcd_init_and_print();

	ADC_setup_PA();
	ADC_Setup_VREFEN();

	while(1) {
		static uint8_t lastDeci = 255;
		uint8_t curDeci = timeData.hundredths / 10;
		if (curDeci != lastDeci) {
			lastDeci = curDeci;

			uint16_t pa0 = ADC_measure_PA(1);
			uint16_t pa1 = ADC_measure_PA(2);
			uint16_t VREF = ADC_measure_VREF();

			printf("VREFint = %d\n",VREF);

			float V_DDA = 3.3 * VREFINT_CAL / VREF;
			printf("V_DDA = %f\n", V_DDA);

			float Vch0 = (V_DDA *pa0)/ (pow(2,12)-1);
			float Vch1 = (V_DDA *pa1)/ (pow(2,12)-1);

			float Abs = sqrt(pow(Vch0,2)+pow(Vch1,2));

			char line0[24], line1[24], line2[24], line3[24];
			sprintf(line0, "VRef: %4u", VREF);
			sprintf(line1, "VDDA: %.2fV", V_DDA);
			sprintf(line2, "PA0: %4u, PA1: %4u", pa0,pa1);
			sprintf(line3, "ABS value: %.2fV", Abs);


			lcd_write_string((uint8_t *)line0, lcdBuffer, 0, 0);
			lcd_write_string((uint8_t *)line1, lcdBuffer, 0, 1);
			lcd_write_string((uint8_t *)line2, lcdBuffer, 0, 2);
			lcd_write_string((uint8_t *)line3, lcdBuffer, 0, 3);
			lcd_push_buffer(lcdBuffer);

			for(uint32_t i = 0; i<1000;i++);

		}
	}
}
