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

static uint8_t lcdBuffer[LCD_BUFF_SIZE];

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	initJoystick();
	initLed();
	iniEXTIA4();
	initTimer();

	lcd_init_and_print();
	ADC_setup_PA();

	while(1) {
	    static uint8_t lastDeci = 255;
	    uint8_t curDeci = timeData.hundredths / 10;
	    if (curDeci != lastDeci) {
	        lastDeci = curDeci;

	        uint16_t pa0 = ADC_measure_PA(1);
	        uint16_t pa1 = ADC_measure_PA(2);

	        char line0[24], line1[24];
	        sprintf(line0, "PA0: %4u", pa0);
	        sprintf(line1, "PA1: %4u", pa1);
	        lcd_write_string((uint8_t *)line0, lcdBuffer, 0, 1);
	        lcd_write_string((uint8_t *)line1, lcdBuffer, 0, 2);
	        lcd_push_buffer(lcdBuffer);
	    }
	}
}
