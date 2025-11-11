#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "interrupts.h"
//#include "timer.h"
#include "lcd.h"
#include "lcd_graphics.h"
//#include "lcd_draw.h"
//#include "lcd_graph.h"
#include "adc.h"
#include "fft.h"

// Initialize LCD and virtual buffer
uint8_t lcdBuffer[LCD_BUFF_SIZE];
uint8_t virtualBuffer[VIRTUAL_WIDTH_SIZE * LCD_ROWS];

int main(void) {
	uart_init( 9600 );	// Initialize USB serial at 9600 baud

	// ================== Initialize ===================
	initJoystick();		// Enabling GPIO pins for joystick
	initLed();			// Enabling GPIO pins for LED
//	initTimer();
	init_spi_lcd();		// Initialize SPI for LCD
	ADC_setup_PA();		// Enabling GPIO pins for ADC

	// Clear LCD buffer and virtual buffer
	lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
	lcd_clear_buffer(virtualBuffer, LCD_ROWS * VIRTUAL_WIDTH_SIZE);

	// Define graph area (i.e. position in virtual buffer)
	uint16_t graph_x_start = GRAPH_X_OFFSET;
	uint16_t graph_x_end = VIRTUAL_WIDTH_SIZE - 1;
	uint16_t graph_y_bottom = LCD_HEIGHT - 1 - GRAPH_MARGIN_BOTTOM;
	uint16_t graph_y_top = GRAPH_MARGIN_TOP;

	// Draw axis
	lcd_draw_vertical_line(virtualBuffer, VIRTUAL_WIDTH_SIZE, graph_x_start, 0, graph_y_bottom);
	lcd_draw_horizontal_line(virtualBuffer, VIRTUAL_WIDTH_SIZE, graph_x_start, graph_x_end, graph_y_bottom);

	compute_fft(virtualBuffer, VIRTUAL_WIDTH_SIZE);

	while(1) {
		// Copy visible window to physical LCD buffer
		update_lcdBuffer();

		// Push LCD buffer
		lcd_push_buffer(lcdBuffer);
	}
}
