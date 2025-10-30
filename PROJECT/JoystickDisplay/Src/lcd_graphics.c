#include "lcd_graphics.h"

/*
 * Each function will update the contents of the LCD buffer.
 */

/*
 * 3x5 fonts for digits '0'-'9'
 */
const uint8_t font3x5[][3] = {
		{0x1F, 0x11, 0x1F},	// 0
		{0x00, 0x00, 0x1F},	// 1
		{0x1D, 0x15, 0x17},	// 2
		{0x15, 0x15, 0x1F},	// 3
		{0x07, 0x04, 0x1F},	// 4
		{0x17, 0x15, 0x1D},	// 5
		{0x1F, 0x15, 0x1D},	// 6
		{0x01, 0x01, 0x1F},	// 7
		{0x1F, 0x15, 0x1F},	// 8
		{0x17, 0x15, 0x1F}	// 9
};

/*
 * Draws a 5-row x 3-col number (num). The position (x, y)
 * indicates the top left corner of the number.
 */
void lcd_draw_char3x5(uint8_t *buffer, uint8_t x, uint8_t y, char c) {
	if (c < '0' || c > '9') return;

	uint8_t index = c - '0';
	for (int col = 0; col < 3; col++) {
		uint8_t bits = font3x5[index][col];
		for (int row = 0; row < 5; row++) {
			if (bits & (1 << row)) {
				lcd_draw_pixel(buffer, x + col, y + row);
			}
		}
	}
}

//void lcd_init_and_print(void) {
//	static uint8_t lcdBuffer[LCD_BUFF_SIZE];	// 512 bytes
//	init_spi_lcd();
//	memset(lcdBuffer, 0x00, sizeof(lcdBuffer));
//
//	// Draw some vertical lines
//	for (int i = 0; i < 128; i += 8) {	// every 8 pixels
//		lcd_draw_vertical_line(lcdBuffer, i, 0, 31);	// full height line
//	}
//
//	// Push buffer to LCD
//	lcd_push_buffer(lcdBuffer);
//}

void lcd_clear_buffer(uint8_t *buffer) {
	memset(buffer, 0x00, LCD_BUFF_SIZE);
}

void lcd_draw_fft_window(uint8_t *buffer) {
	//
}

/*
 * Draws a 5-row x 3-col number (num), note to start with
 * we assume we are drawing only 1 or 2 digit numbers.
 * The position (x, y) indicates the top left corner of the
 * number.
 */
void lcd_draw_number(uint8_t *buffer, int num, uint8_t x, uint8_t y) {
	if (num < 0 || num > 100) return;

	// We assume other bounds have been checked

	if (num == 0) {
		/*
		 * XXX
		 * X X
		 * X X
		 * X X
		 * XXX
		 */

//		// First row (from top)
//		uint8_t row = y / LCD_SLICE_SIZE;		// Finds slice row
//		uint8_t bit = y % LCD_SLICE_SIZE;		// Finds vertical offset within slice
//		buffer[row * LCD_LINE_SIZE + x] |= (1 << bit);
//		buffer[row * LCD_LINE_SIZE + x + 1] |= (1 << bit);
//		buffer[row * LCD_LINE_SIZE + x + 2] |= (1 << bit);
//
//		// 2nd row (from top)
//		uint8_t row = (y+1) / LCD_SLICE_SIZE;		// Finds slice row
//		uint8_t bit = (y+1) % LCD_SLICE_SIZE;		// Finds vertical offset within slice
//		buffer[row * LCD_LINE_SIZE + x] |= (1 << bit);
////		buffer[row * LCD_LINE_SIZE + x + 1] |= (1 << bit);
//		buffer[row * LCD_LINE_SIZE + x + 2] |= (1 << bit);
//
//		// 3rd row (from top)
//		uint8_t row = (y+2) / LCD_SLICE_SIZE;		// Finds slice row
//		uint8_t bit = (y+2) % LCD_SLICE_SIZE;		// Finds vertical offset within slice
//		buffer[row * LCD_LINE_SIZE + x] |= (1 << bit);
////		buffer[row * LCD_LINE_SIZE + x + 1] |= (1 << bit);
//		buffer[row * LCD_LINE_SIZE + x + 2] |= (1 << bit);
//
//		// 2nd row (from top)
//		uint8_t row = (y+1) / LCD_SLICE_SIZE;		// Finds slice row
//		uint8_t bit = (y+1) % LCD_SLICE_SIZE;		// Finds vertical offset within slice
//		buffer[row * LCD_LINE_SIZE + x] |= (1 << bit);
////		buffer[row * LCD_LINE_SIZE + x + 1] |= (1 << bit);
//		buffer[row * LCD_LINE_SIZE + x + 2] |= (1 << bit);
	}
}

//void lcd_draw_x_axis(uint8_t *buffer, uint8_t x_start, uint8_t x_end) {
//	// Insert check bounds code
//
//	lcd_draw_horizontal_line(buffer, 9, LCD_LINE_SIZE-1, 24);
//	lcd_draw_vertical_line(buffer, 9, 0, 24);
//}
void lcd_draw_axis(uint8_t *buffer) {
	// Insert check bounds code

	lcd_draw_horizontal_line(buffer, 9, LCD_LINE_SIZE-1, 24);
	lcd_draw_vertical_line(buffer, 9, 0, 24);
}

/*
 * The function turns on a single pixel at coordinates (x, y)
 * in the frame buffer for the entire LCD display.
 */
void lcd_draw_pixel(uint8_t *buffer, uint8_t x, uint8_t y) {
	if (x >= LCD_LINE_SIZE || y >= LCD_HEIGHT) return;	// Checks bounds
	uint8_t row = y / LCD_SLICE_SIZE;		// Finds slice row
	uint8_t bit = y % LCD_SLICE_SIZE;		// Finds vertical offset within slice
	buffer[row * 128 + x] |= (1 << bit);	// Computes index in buffer and turns on specific pixel
}

/*
 * Draws a vertical line at x which goes from y_start to y_end
 */
void lcd_draw_vertical_line(uint8_t *buffer, uint8_t x, uint8_t y_start, uint8_t y_end) {
    if (x >= LCD_LINE_SIZE) return;  // limit check
    if (y_start > y_end) { uint8_t tmp = y_start; y_start = y_end; y_end = tmp; }

    for (uint8_t y = y_start; y <= y_end; y++) {
        uint8_t page = y / 8;        // 0-3
        uint8_t bit  = y % 8;        // 0-7
        buffer[page * LCD_LINE_SIZE + x] |= (1 << bit);
    }
}

/*
 * Draws a horizontal line at y which goes from x_start to x_end
 */
void lcd_draw_horizontal_line(uint8_t *buffer, uint8_t x_start, uint8_t x_end, uint8_t y) {
	// Check bounds
	if (y <= 0 || y >= LCD_HEIGHT) return;
	if (x_start > x_end) {
		uint8_t temp = x_start;
		x_start = x_end;
		x_end = temp;
	}
	if (x_start >= LCD_LINE_SIZE) return;

	uint8_t row = y / LCD_SLICE_SIZE;	// Find row
	uint8_t bit = y % LCD_SLICE_SIZE;	// Find bit

	for (uint8_t x = x_start; x <= x_end; x++) {
		buffer[row * LCD_LINE_SIZE + x] |= (1 << bit);
	}
}



void lcd_draw_fft(uint8_t *buffer) {
	// Needs more inputs

	// Inputs: buffer, x, y_start, y_end, magnitude_scale??
}

void lcd_draw_histogram(uint8_t *buffer) {
	// Not implemented yet
}




void lcd_draw_bar(uint8_t *buffer, uint8_t x, uint8_t height, uint8_t max_height) {
	if (height > max_height) height = max_height;
	uint8_t y_end = 31;		// bottom of the screen
	uint8_t y_start = y_end - height + 1;
	lcd_draw_vertical_line(buffer, x, y_start, y_end);
}
