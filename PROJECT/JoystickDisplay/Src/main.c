#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "interrupts.h"
#include "timer.h"
#include "lcd.h"
#include "lcd_graphics.h"
#include "adc.h"

static uint8_t lcdBuffer[LCD_BUFF_SIZE];		// 4 rows x 128 slices per row
//static uint8_t graphBuffer[LCD_BUFF_SIZE * 2];	// 4 x (4 rows x 128 slices per row)

//#define LCD_ROWS 4
//
//void update_LCD_window(int offset) {
//	// Offset is in range 0 to LCD_BUFF_SIZE - LCD_LINE_SIZE
//	//					  0 to 384
//	for (int row = 0; row < LCD_ROWS; row++) {
//		memcpy(&lcdBuffer[row * LCD_LINE_SIZE],
//			   &graphBuffer[row * LCD_BUFF_SIZE + offset],
//			   LCD_LINE_SIZE);
//	}
//}

int main(void) {
	uart_init( 9600 );	// Initialize USB serial at 9600 baud

	// ================== Initialize ===================
	initJoystick();		// Enabling GPIO pins for joystick
	initLed();			// Enabling GPIO pins for LED
	initTimer();
	init_spi_lcd();		// Initialize SPI for LCD
	ADC_setup_PA();		// Enabling GPIO pins for ADC

	// Is Ex2-6 calibration necessary when just reading raw ADC value?

	// Clear LCD buffer and virtual/graph buffer
	lcd_clear_buffer(lcdBuffer);
//	lcd_clear_buffer(graphBuffer);

//	lcd_draw_horizontal_line(graphBuffer, 0, 127, 25);
//	lcd_draw_horizontal_line(graphBuffer, 128, 255, 20);

	lcd_draw_horizontal_line(lcdBuffer, 0, LCD_LINE_SIZE - 1, 31);

	// Push buffer to LCD
	lcd_push_buffer(lcdBuffer);




	while(1) {
//		static float pa0_f = 0, pa1_f = 0;
//		const float alpha = 0.1;	// Smoothing factor, smaller means smoother
//
//		static uint8_t last_deci = 255;
//		uint8_t current_deci = timeData.hundredths / 10;	// Update every 0.1 s
//		if (current_deci != last_deci) {
//			last_deci = current_deci;
//
//			pa0_f = pa0_f * (1 - alpha) + alpha * ADC_measure_PA(1);
//			pa1_f = pa1_f * (1 - alpha) + alpha * ADC_measure_PA(2);
//
//			// Convert read ADC value to string
//			char line0[24], line1[24];
//			sprintf(line0, "PA0: %4u", (uint16_t)pa0_f);
//			sprintf(line1, "PA1: %4u", (uint16_t)pa1_f);
//
//			// Write strings to LCD buffer
//			lcd_write_string((uint8_t *)line0, lcdBuffer, 0, 1);
//			lcd_write_string((uint8_t *)line1, lcdBuffer, 0, 2);
//
//			// Push buffer to LCD
//			lcd_push_buffer(lcdBuffer);
//		}
	}






	// ================== LCD GRAPHICS =================

//	// Initialize SPI for LCD
//	init_spi_lcd();
//
//	// Initialize and clear buffer
//	static uint8_t buffer[LCD_BUFF_SIZE];	// 512 bytes
//	lcd_clear_buffer(buffer);

//	lcd_test_cosine_wave();

//	lcd_draw_horizontal_line(buffer, 2, 29, 10);
//	lcd_draw_axis(buffer);

//	lcd_draw_char3x5(buffer, 8, 27, '1');
//	lcd_draw_char3x5(buffer, 12, 27, '0');
//
//	lcd_draw_char3x5(buffer, 16, 25, '1');
//	lcd_draw_char3x5(buffer, 20, 25, '0');
//
//	lcd_draw_char3x5(buffer, 24, 22, '1');
//	lcd_draw_char3x5(buffer, 28, 22, '0');

//	lcd_draw_char3x5(buffer, 0, 27, '0');
//	lcd_draw_char3x5(buffer, 4, 27, '1');
//	lcd_draw_char3x5(buffer, 8, 27, '2');
//	lcd_draw_char3x5(buffer, 12, 27, '3');
//	lcd_draw_char3x5(buffer, 16, 27, '4');
//	lcd_draw_char3x5(buffer, 20, 27, '5');
//	lcd_draw_char3x5(buffer, 24, 27, '6');
//	lcd_draw_char3x5(buffer, 28, 27, '7');
//	lcd_draw_char3x5(buffer, 32, 27, '8');
//	lcd_draw_char3x5(buffer, 36, 27, '9');
//
//	lcd_push_buffer(buffer);

//	while(1);
}
