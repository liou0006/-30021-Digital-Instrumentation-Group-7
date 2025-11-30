#include "lcd_graphics.h"

#include "adc.h"
#include <stdio.h>
#include <math.h>

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

void draw_new_axis(uint16_t x_offset) {
	lcd_draw_vertical_line(virtualBuffer, VIRTUAL_WIDTH_SIZE, graph_x_start + x_offset, graph_y_top, graph_y_bottom);
	lcd_draw_horizontal_line(virtualBuffer, VIRTUAL_WIDTH_SIZE, graph_x_start + x_offset, graph_x_end, graph_y_bottom);
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
	if (c == '-') {
		for (uint16_t col = 0; col < 3; col++) {
			uint16_t bits = 0x04;
			for (uint16_t row = 0; row < 5; row++) {
				if (bits & (1 << row)) {
					lcd_draw_pixel(buffer, buff_width, x + col, y + row);
				}
			}
		}
	} else if (c == '.') {
		uint16_t bits = 0x10;
		for (uint16_t row = 0; row < 5; row++) {
			if (bits & (1 << row)) {
				lcd_draw_pixel(buffer, buff_width, x, y + row);
			}
		}
	}

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

void lcd_convert_int_to_char3x5_y_axis(uint8_t *buffer, uint16_t buff_width,
		int max_num_digits, int val, uint16_t x, uint16_t y) {
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

	// Digits are reversed, so drawing from left to right
	if (max_num_digits == 1) {
		lcd_draw_char3x5(buffer, buff_width, x + 4, y, str[idx - 1]);
		return;
	} else if (max_num_digits == 2) {
		if (idx < 2) {
			lcd_draw_char3x5(buffer, buff_width, x + 4, y, str[idx - 1]);
			return;
		}
		for (int i = idx - 1; i >= 0; i--) {
			lcd_draw_char3x5(buffer, buff_width, x, y, str[i]);
			x += 4;		// Move right by width of char + 1 px
		}
	} else if (max_num_digits == 3) {
		if (idx < 2) {
			lcd_draw_char3x5(buffer, buff_width, x + 8, y, str[idx - 1]);
			return;
		} else if (idx < 3) {
			x += 4;
			for (int i = idx - 1; i >= 0; i--) {
				lcd_draw_char3x5(buffer, buff_width, x, y, str[i]);
				x += 4;		// Move right by width of char + 1 px
			}
			return;
		}
		for (int i = idx - 1; i >= 0; i--) {
			lcd_draw_char3x5(buffer, buff_width, x, y, str[i]);
			x += 4;		// Move right by width of char + 1 px
		}
	}
}

void lcd_convert_int_to_char3x5_x_axis(uint8_t *buffer, uint16_t buff_width, int val, uint16_t x, uint16_t y) {
	char str[12];		// Enough for 32-bit int
	int idx = 0;
	int isNegative = (val < 0);

	// Handle the special case for 0
	if (val == 0) {
		lcd_draw_char3x5(buffer, buff_width, x-3, y, '0');
		return;
	}

	// Find absolute value
	if (isNegative) val = -val;

	// Extract digits in reverse order
	while (val > 0) {
		int digit = val % 10;
		str[idx++] = '0' + digit;	// Convert to ASCII
		val /= 10;
	}

	// Draw negative sign
	if (isNegative) str[idx++] = '-';

	// Center value around middle pixel
	uint16_t digit_offset = 0;
	if (idx == 6) digit_offset = 11;
	else if (idx == 5) digit_offset = 9;
	else if (idx == 4) digit_offset = 7;
	else if (idx == 3) digit_offset = 5;
	else if (idx == 2) digit_offset = 3;
	else if (idx == 1) digit_offset = 1;

	// Digits are reversed, so drawing from left to right
	for (int i = idx - 1; i >= 0; i--) {
		lcd_draw_char3x5(buffer, buff_width, x - digit_offset, y, str[i]);
		x += 4;		// Move right by width of char + 1 px
	}
}

void lcd_convert_float_to_char3x5_y_axis(uint8_t *buffer, uint16_t buff_width,
		int max_num_digits, float val, uint16_t x, uint16_t y, int decimals) {
	// Sanity constraint
	if (max_num_digits < 1) max_num_digits = 1;
	if (decimals < 0) decimals = 0;
	if (!buffer) return;

	// Format float into string using given decimals
	// Rounding provided by snprintf
	char str[12];
	if (decimals == 0) {
		snprintf(str, sizeof(str), "%.0f", val);
	} else {
		char fmt[8];
		snprintf(fmt, sizeof(fmt), "%%.%df", decimals);
		snprintf(str, sizeof(str), fmt, val);
	}

	// Trim leading spaces if they exists
	char *s = str;
	while (*s == ' ') s++;

	// Compute px width of formatted label
	int label_px_width = 0;
	size_t len = strlen(s);
	for (size_t i = 0; i < len; ++i) {
		char c = s[i];
		if (c == '.') label_px_width += DOT_WIDTH;
		else label_px_width += DIGIT_WIDTH;
		if (i + 1 < len) label_px_width += CHAR_SPACING;
	}

	int reserved_px = (max_num_digits * DIGIT_WIDTH) + 1;
//	if (max_num_digits > 1) reserved_px += (max_num_digits - 1) * CHAR_SPACING;
//
	int start_x = x + reserved_px - label_px_width;
//	if (start_x < (int)x) start_x = (int)x;

	int cur_x = start_x;
	for (size_t i = 0; i < len; ++i) {
		char c = s[i];
		/* Draw char (lcd_draw_char3x5 expects an ASCII char) */
		lcd_draw_char3x5(buffer, buff_width, (uint16_t)cur_x, y, c);

		/* Advance cur_x by this char's pixel width + spacing (if not last char) */
		if (c == '.') cur_x += DOT_WIDTH;
		else cur_x += DIGIT_WIDTH;

		if (i + 1 < len) cur_x += CHAR_SPACING;
	}
}



void lcd_convert_float_to_char3x5_x_axis(uint8_t *buffer, uint16_t buff_width,
		float val, uint16_t x, uint16_t y, int decimals) {
	// Sanity constraint
	if (decimals < 0) decimals = 0;
	if (!buffer) return;

	// Format float into string using given decimals
	// Rounding provided by snprintf (also handles negative values)
	char str[12];
	if (decimals == 0) {
		snprintf(str, sizeof(str), "%.0f", val);
	} else {
		char fmt[8];
		snprintf(fmt, sizeof(fmt), "%%.%df", decimals);
		snprintf(str, sizeof(str), fmt, val);
	}

	// Trim leading spaces if they exists
	char *s = str;
	while (*s == ' ') s++;

	// Compute px width of formatted label
	int label_px_width = 0;
	size_t len = strlen(s);
	for (size_t i = 0; i < len; ++i) {
		char c = s[i];
		if (c == '.') {
			// Character has 1 px width
			label_px_width += DOT_WIDTH;
		} else {
			// Standard characters have 3 px width
			label_px_width += DIGIT_WIDTH;
		}

		// Add spacing if not last character
		if (i + 1 < len) label_px_width += CHAR_SPACING;
	}

	// ---------- Center value around middle pixel ----------
	// Input x is the center point for the label
	int half_width = label_px_width / 2;
	int x_start = (int)x - half_width;

	// Ensuring drawing label is within graph limit (only for left side)
	if (x_start < 0) x_start = 0;

	// ---------- Draw characters ----------
	int cur_x = x_start;
	for (size_t i = 0; i < len; ++i) {
		char c = s[i];

		// Draw char
		lcd_draw_char3x5(buffer, buff_width, (uint16_t)cur_x, y, c);

		// Advance cur_x by the char's px width
		if (c == '.') cur_x += DOT_WIDTH;
		else cur_x += DIGIT_WIDTH;

		// Add spacing if not last character
		if (i + 1 < len) cur_x += CHAR_SPACING;
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
