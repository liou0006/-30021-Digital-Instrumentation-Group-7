#include "lcd_graphics.h"

#include "adc.h"

/***********************************************************
 * Constants (local)
 ***********************************************************/
// 3x5 fonts for digits '0'-'9'
const uint8_t font3x5[][3] = {
		{0x1F, 0x11, 0x1F},	// 0
		{0x00, 0x1F, 0x00},	// 1
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

// Define graph area (i.e. position in virtual buffer)
const uint16_t graph_x_start = GRAPH_X_OFFSET;
const uint16_t graph_x_end = VIRTUAL_WIDTH_SIZE - 2 - GRAPH_MARGIN_RIGHT;
const uint16_t graph_y_bottom = LCD_HEIGHT - 1 - GRAPH_MARGIN_BOTTOM;
const uint16_t graph_y_top = GRAPH_MARGIN_TOP;

/***********************************************************
 * Functions
 ********************Y***************************************/
void lcd_clear_buffer(uint8_t *buffer, uint16_t buff_size) {
	memset(buffer, 0x00, buff_size);
}

uint16_t graph_y_to_lcd_y(uint16_t graph_y) {
	return LCD_HEIGHT - 1 - GRAPH_MARGIN_BOTTOM - graph_y;
}

void draw_graph_axis() {
	lcd_draw_vertical_line(virtualBuffer, VIRTUAL_WIDTH_SIZE, graph_x_start, graph_y_top, graph_y_bottom);
	lcd_draw_horizontal_line(virtualBuffer, VIRTUAL_WIDTH_SIZE, graph_x_start, graph_x_end, graph_y_bottom);
}

//void draw_hist_bar(uint16_t x_start, uint16_t bar_width, y_start, y_end) {
//	//
//}


void draw_axis_number(uint16_t num) {
//	char low = num & 0xFF;
//	char high = num >> 8;

//	lcd_draw_char3x5(buffer, buff_width, x_tick - 5, graph_y_to_lcd_y(0) + 2, '1');
//	lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 4, graph_y_to_lcd_y(0) + 2, '0');
//	lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 8, graph_y_to_lcd_y(0) + 2, '0');
}


/*
 * The function turns on a single pixel at coordinates (x, y)
 * in the frame buffer for the entire LCD display.
 */
void lcd_draw_pixel(uint8_t *buffer, uint16_t buff_width, uint16_t x, uint16_t y) {
	if (x >= buff_width || y >= LCD_HEIGHT) return;	// Checks bounds
	uint16_t row = y / LCD_SLICE_SIZE;		// Finds slice row
	uint16_t bit = y % LCD_SLICE_SIZE;		// Finds vertical offset within slice
	uint16_t base = row * buff_width;
	buffer[base + x] |= (1 << bit);	// Computes index in buffer and turns on specific pixel
}

/*
 * Draws a vertical line at x which goes from y_start to y_end
 */
void lcd_draw_vertical_line(uint8_t *buffer, uint16_t buff_width, uint16_t x, uint16_t y_start, uint16_t y_end) {
	// Check bounds
	if (x >= buff_width) x = buff_width - 1;	// Clamp to right edge
	if (y_start > y_end) {
		uint16_t temp = y_start;
		y_start = y_end;
		y_end = temp;
	}
	if (y_end >= LCD_HEIGHT) y_end = LCD_HEIGHT - 1;

	for (uint16_t y = y_start; y <= y_end; y++) {
		uint16_t row = y / LCD_SLICE_SIZE;	// Find row
		uint16_t bit  = y % LCD_SLICE_SIZE;	// Find bit
		uint16_t base = row * buff_width;

		buffer[base + x] |= (1 << bit);
	}
}

void lcd_draw_fft_mag(uint8_t *buffer, uint16_t buff_width, uint16_t x, uint16_t mag) {
//	uint16_t x_min = 5, x_max = buff_width - 1, y_length_max = 26;
//
//	if (x >= buff_width) return;
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
void lcd_draw_char3x5(uint8_t *buffer, uint16_t buff_width, uint16_t x, uint16_t y, char c) {
	if (c < '0' || c > '9') return;

	uint16_t index = c - '0';
	for (uint16_t col = 0; col < 3; col++) {
		uint16_t bits = font3x5[index][col];
		for (uint16_t row = 0; row < 5; row++) {
			if (bits & (1 << row)) {
				lcd_draw_pixel(buffer, buff_width, x + col, y + row);
			}
		}
	}
}

void lcd_convert_int_to_char3x5_y_axis(uint8_t *buffer, uint16_t buff_width, int val, uint16_t x, uint16_t y) {
	char str[12];		// Enough for 32-bit int
	int idx = 0;

	if (val == 0) {
		lcd_draw_char3x5(buffer, buff_width, x + 4, y + 1, '0');
		return;
	}

	// Extract digits in reverse order
	while (val > 0) {
		int digit = val % 10;
		str[idx++] = '0' + digit;	// Convert to ASCII
		val /= 10;
	}

	if (idx < 2) {
		lcd_draw_char3x5(buffer, buff_width, x + 4, y, str[idx - 1]);
		return;
	}

	// Digits are reversed, so drawing from left to right
	for (int i = idx - 1; i >= 0; i--) {
		lcd_draw_char3x5(buffer, buff_width, x, y, str[i]);
		x += 4;		// Move right by width of char + 1 px
	}
}

void lcd_convert_int_to_char3x5_x_axis(uint8_t *buffer, uint16_t buff_width, int val, uint16_t x, uint16_t y) {
	char str[12];		// Enough for 32-bit int
	int idx = 0;

	if (val == 0) {
		lcd_draw_char3x5(buffer, buff_width, x, y, '0');
		return;
	}

	// Extract digits in reverse order
	while (val > 0) {
		int digit = val % 10;
		str[idx++] = '0' + digit;	// Convert to ASCII
		val /= 10;
	}

	// Digits are reversed, so drawing from left to right
	for (int i = idx - 1; i >= 0; i--) {
		lcd_draw_char3x5(buffer, buff_width, x, y, str[i]);
		x += 4;		// Move right by width of char + 1 px
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
	uint16_t scroll_offset = (pot2_val * max_scroll) / 4080;	// Should be 4095 but our pots never reach this

	if (scroll_offset > max_scroll) scroll_offset = max_scroll;

	// Copy visible window to physical LCD buffer
	for (uint8_t row = 0; row < LCD_ROWS; row++) {
		uint16_t v_offset = row * VIRTUAL_WIDTH_SIZE;	// Start of virtual row
		uint16_t l_offset = row * LCD_LINE_SIZE;		// Start of LCD row
		for (uint8_t col = 0; col < LCD_LINE_SIZE; col++) {
			lcdBuffer[l_offset + col] = virtualBuffer[v_offset + scroll_offset + col];
		}
	}
}

// Will only update virtualBuffer
//void lcd_draw_fft(uint16_t mag)




//
//
//void lcd_draw_fft(uint8_t *buffer) {
//	// Needs more inputs
//
//	// Inputs: buffer, x, y_start, y_end, magnitude_scale??
//}
//
//
//
//void lcd_draw_fft_window(uint8_t *buffer) {
//	//
//}
//
//void lcd_draw_axis(uint8_t *buffer) {
//	// Insert check bounds code
//
////	lcd_draw_horizontal_line(buffer, 9, LCD_LINE_SIZE-1, 24);
////	lcd_draw_vertical_line(buffer, 9, 0, 24);
//}
//
//void lcd_draw_histogram(uint8_t *buffer) {
//	// Not implemented yet
//}
//
//
//void lcd_draw_bar(uint8_t *buffer, uint8_t x, uint8_t height, uint8_t max_height) {
//	if (height > max_height) height = max_height;
//	uint8_t y_end = 31;		// bottom of the screen
//	uint8_t y_start = y_end - height + 1;
////	lcd_draw_vertical_line(buffer, x, y_start, y_end);
//}
