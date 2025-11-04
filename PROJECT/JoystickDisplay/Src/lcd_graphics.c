#include "lcd_graphics.h"

/***********************************************************
 * Constants (local)
 ***********************************************************/
// 3x5 fonts for digits '0'-'9'
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

const uint16_t max_scroll = VIRTUAL_WIDTH_SIZE - LCD_LINE_SIZE;

/***********************************************************
 * Functions
 ********************Y***************************************/
void lcd_clear_buffer(uint8_t *buffer, uint8_t buff_type) {
	if (buff_type == 0) memset(buffer, 0x00, LCD_BUFF_SIZE);
	else if (buff_type == 1) memset(buffer, 0x00, LCD_ROWS * VIRTUAL_WIDTH_SIZE);
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
void lcd_draw_vertical_line(uint8_t *buffer, uint16_t buff_width, uint16_t x, uint16_t y_start, uint16_t y_end) {
//	// Check bounds
//	if (x >= buff_width) x = buff_width - 1;	// Clamp to right edge
//	if (y_start > y_end) {
//		uint16_t temp = y_start;
//		y_start = y_end;
//		y_end = temp;
//	}
//	if (y_end >= LCD_HEIGHT) return;
//
//	for (uint16_t y = y_start; y <= y_end; y++) {
//		uint16_t row = y / LCD_SLICE_SIZE;	// Find row
//		uint16_t bit  = y % LCD_SLICE_SIZE;	// Find bit
//		uint16_t base = row * buff_width;
//
//		buffer[base + x] |= (1 << bit);
//	}
}

/*
 * Draws a horizontal line at y which goes from x_start to x_end.
 * Specify the buffer width and the function works for both the
 * LCD buffer and the virtual buffer.
 */
void lcd_draw_horizontal_line(uint8_t *buffer, uint16_t buff_width, uint16_t x_start, uint16_t x_end, uint16_t y) {
	// Check bounds
	if (y >= LCD_HEIGHT) return;
	if (x_start > x_end) {
		uint16_t temp = x_start;
		x_start = x_end;
		x_end = temp;
	}
	if (x_start >= buff_width) return;
	if (x_end >= buff_width) x_end = buff_width - 1;	// Clamp to right edge

	uint16_t row = y / LCD_SLICE_SIZE;	// Find row
	uint16_t bit = y % LCD_SLICE_SIZE;	// Find bit
	uint16_t base = row * buff_width;

	for (uint16_t x = x_start; x <= x_end; x++) {
		buffer[base + x] |= (1 << bit);
	}
}

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

/*
 * Copies the visible window of the virtual buffer to the
 * physical LCD buffer based on the calculated scroll
 * offset.
 */
void update_lcdBuffer() {
	// Map potentiometer value to scroll offset
	uint16_t pot2_val = ADC_measure_PA(2);
	uint16_t scroll_offset = (pot2_val * max_scroll) / 4095;

	// Copy visible window to physical LCD buffer
	for (uint8_t row = 0; row < LCD_ROWS; row++) {
		uint16_t v_offset = row * VIRTUAL_WIDTH_SIZE;	// Start of virtual row
		uint16_t l_offset = row * LCD_LINE_SIZE;		// Start of LCD row
		for (uint8_t col = 0; col < LCD_LINE_SIZE; col++) {
			lcdBuffer[l_offset + col] = virtualBuffer[v_offset + scroll_offset + col];
		}
	}
}






void lcd_draw_fft(uint8_t *buffer) {
	// Needs more inputs

	// Inputs: buffer, x, y_start, y_end, magnitude_scale??
}








void lcd_draw_fft_window(uint8_t *buffer) {
	//
}

void lcd_draw_axis(uint8_t *buffer) {
	// Insert check bounds code

//	lcd_draw_horizontal_line(buffer, 9, LCD_LINE_SIZE-1, 24);
//	lcd_draw_vertical_line(buffer, 9, 0, 24);
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
