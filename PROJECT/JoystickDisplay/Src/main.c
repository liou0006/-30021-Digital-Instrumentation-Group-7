#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
//#include "joystick.h"
//#include "led.h"
//#include "interrupts.h"
#include "lcd.h"
//#include <math.h>

//#define SAMPLE_RATE		8000.0f	// Hz
//#define FFT_SIZE		256
//#define DISPLAY_WIDTH	128
//#define DISPLAY_HEIGHT	32

void lcd_draw_vertical_line(uint8_t *lcdBuffer, uint8_t x, uint8_t y_start, uint8_t y_end)
{
    if (x >= 128) return;  // limit check
    if (y_start > y_end) { uint8_t tmp = y_start; y_start = y_end; y_end = tmp; }

    for (uint8_t y = y_start; y <= y_end; y++) {
        uint8_t page = y / 8;        // 0–3
        uint8_t bit  = y % 8;        // 0–7
        lcdBuffer[page * 128 + x] |= (1 << bit);
    }
}

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

	// === Full height vertical line every 8 pixels
	static uint8_t lcdBuffer[512];	// 128 x 32 = 512 bytes
	init_spi_lcd();
	memset(lcdBuffer, 0x00, sizeof(lcdBuffer));

	// Draw some vertical lines
	for (int i = 0; i < 128; i += 8) {	// every 8 pixels
		lcd_draw_vertical_line(lcdBuffer, i, 0, 31);	// full height line
	}

	// Push buffer to LCD
	lcd_push_buffer(lcdBuffer);

	while(1);
}
