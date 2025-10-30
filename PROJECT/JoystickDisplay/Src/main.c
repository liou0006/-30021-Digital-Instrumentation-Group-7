#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
//#include "joystick.h"
//#include "led.h"
//#include "interrupts.h"
#include "lcd.h"
#include "lcd_graphics.h"
#include <math.h>

// From CHATGPT to test if printing works
//void lcd_test_cosine_wave(void) {
//	static uint8_t buffer[LCD_BUFF_SIZE];
//	lcd_clear_buffer(buffer);
//
//	const int num_points = LCD_LINE_SIZE;	// one bar per column
//	const float amplitude = 16.0f;			// half the LCD height (32)
//	const float offset = 16.0f;				// center vertically
//	const float freq = 2.0f;				// no. of cosine periods across screen
//
//	float magnitude[num_points];
//
//    // Generate cosine wave magnitudes
//    for (int i = 0; i < num_points; i++) {
//        float angle = 2.0f * M_PI * freq * i / num_points;
//        float value = cosf(angle);
//
//        // Map to vertical coordinate (centered on screen)
//        float y_float = offset + amplitude * value;
//        uint8_t y = (uint8_t)(LCD_HEIGHT_SIZE - 1 - y_float); // invert Y (0 = top)
//        lcd_draw_pixel(buffer, i, y);
//    }
//
//    // Push to LCD
//    lcd_push_buffer(buffer);
//}

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	// ===== BASIC JOYSTICK FUNCTIONALITY WITH LED =====
//	initLed();
//	initJoystick();
//
//	setLed('d');
//
//	while(1) {
//		handleJoystick();
//	}

	// ================== LCD DISPLAY ==================
//	static uint8_t lcdBuffer [ LCD_BUFF_SIZE ];	// Frame buffer for LCD
//
//	// Initialize SPI + GPIOs and reset LCD
//	init_spi_lcd();
//
//	// Clear buffer
//	memset(lcdBuffer, 0x00, LCD_BUFF_SIZE);
//
//	// Write "Hello" at x=0, y=0 on LCD
//	lcd_write_string((uint8_t *)"Hello", lcdBuffer, 0, 0);
//
//	// Push buffer content to LCD
//	lcd_push_buffer(lcdBuffer);
//
//	while(1);


	// ================== LCD GRAPHICS =================

	// Initialize SPI for LCD
	init_spi_lcd();

	// Initialize and clear buffer
	static uint8_t buffer[LCD_BUFF_SIZE];	// 512 bytes
	lcd_clear_buffer(buffer);

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

	lcd_draw_char3x5(buffer, 0, 27, '0');
	lcd_draw_char3x5(buffer, 4, 27, '1');
	lcd_draw_char3x5(buffer, 8, 27, '2');
	lcd_draw_char3x5(buffer, 12, 27, '3');
	lcd_draw_char3x5(buffer, 16, 27, '4');
	lcd_draw_char3x5(buffer, 20, 27, '5');
	lcd_draw_char3x5(buffer, 24, 27, '6');
	lcd_draw_char3x5(buffer, 28, 27, '7');
	lcd_draw_char3x5(buffer, 32, 27, '8');
	lcd_draw_char3x5(buffer, 36, 27, '9');

	lcd_push_buffer(buffer);

	while(1);
}
