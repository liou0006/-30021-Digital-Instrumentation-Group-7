#ifndef _LCD_GRAPHICS_H_
#define _LCD_GRAPHICS_H_

/***********************************************************
 * Includes
 ***********************************************************/
#include "stm32f30x_conf.h"
#include "lcd.h"

/***********************************************************
 * Defines
 ***********************************************************/
#define LCD_HEIGHT 32
#define LCD_SLICE_SIZE 8	// Row height (4 rows each with 128 slices)
//#define LCD_VIRTUAL_SIZE	// Virtual buffer width to enable horizontal scrolling
#define LCD_ROWS 4
#define VIRTUAL_WIDTH_SIZE 256
//#define VIRTUAL_BUFF_SIZE (LCD_ROWS * VIRTUAL_WIDTH_SIZE)

/***********************************************************
 * Functions
 ***********************************************************/
void lcd_clear_buffer(uint8_t *buffer);
void lcd_draw_pixel(uint8_t *buffer, uint8_t x, uint8_t y);
void lcd_draw_vertical_line(uint8_t *buffer, uint8_t x, uint8_t y_start, uint8_t y_end);
void lcd_draw_horizontal_line(uint8_t *buffer, uint8_t x_start, uint8_t x_end, uint8_t y);
void lcd_draw_horizontal_line_v2(uint8_t *buffer, uint16_t buf_width, uint16_t x_start, uint16_t x_end, uint16_t y);
void lcd_draw_char3x5(uint8_t *buffer, uint8_t x, uint8_t y, char c);

void lcd_draw_fft_window(uint8_t *buffer);
void lcd_draw_axis(uint8_t *buffer);
void lcd_draw_fft(uint8_t *buffer);
void lcd_draw_histogram(uint8_t *buffer);
void lcd_draw_bar(uint8_t *buffer, uint8_t x, uint8_t height, uint8_t max_height);

#endif /*! _LCD_GRAPHICS_H_ */
