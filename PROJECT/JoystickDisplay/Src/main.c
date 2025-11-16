#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
//#include "led.h"
//#include "interrupts.h"
//#include "lcd_graphics.h"
//#include "fft.h"
//#include "histogram.h"

// Includes with functions called from main
#include "joystick.h"
#include "adc.h"
#include "lcd_graphics.h"
#include "lcd.h"
#include "menu.h"

// Define buffers (allocate memory)
uint8_t lcdBuffer[LCD_BUFF_SIZE];
uint8_t virtualBuffer[VIRTUAL_WIDTH_SIZE * LCD_ROWS];

int main(void) {
	uart_init( 9600 );	// Initialize USB serial at 9600 baud

	// Initialize
	initJoystick();		// Enabling GPIO pins for joystick
	init_spi_lcd();		// Initialize SPI for LCD
	ADC_setup_PA();		// Enabling GPIO pins for ADC
	menu_init();		// Initialize main menu

	while(1) {
		menu_update();
		lcd_push_buffer(lcdBuffer);
	}
}
