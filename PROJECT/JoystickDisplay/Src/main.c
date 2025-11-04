#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "interrupts.h"
#include "timer.h"
#include "lcd.h"
#include "lcd_graphics.h"
#include "adc.h"

// Initialize LCD and virtual buffer
uint8_t lcdBuffer[LCD_BUFF_SIZE];
uint8_t virtualBuffer[VIRTUAL_WIDTH_SIZE * LCD_ROWS];

int main(void) {
	uart_init( 9600 );	// Initialize USB serial at 9600 baud

	// ================== Initialize ===================
	initJoystick();		// Enabling GPIO pins for joystick
	initLed();			// Enabling GPIO pins for LED
	initTimer();
	init_spi_lcd();		// Initialize SPI for LCD
	ADC_setup_PA();		// Enabling GPIO pins for ADC

	// Clear LCD buffer and virtual buffer
	lcd_clear_buffer(lcdBuffer, 0);
	lcd_clear_buffer(virtualBuffer, 1);

	// Give virtual buffer values
	lcd_draw_horizontal_line(virtualBuffer, VIRTUAL_WIDTH_SIZE, 0, 127, 31);
	lcd_draw_horizontal_line(virtualBuffer, VIRTUAL_WIDTH_SIZE, 128, 255, 0);

	while(1) {
		// Map potentiometer value to scroll offset
		uint16_t pot2_val = ADC_measure_PA(2);
		uint16_t max_scroll = VIRTUAL_WIDTH_SIZE - LCD_LINE_SIZE;
		uint16_t scroll_offset = (pot2_val * max_scroll) / 4095;

		// Copy visible window to physical LCD buffer
		update_lcdBuffer(scroll_offset);

		// Push LCD buffer
		lcd_push_buffer(lcdBuffer);
	}
}
