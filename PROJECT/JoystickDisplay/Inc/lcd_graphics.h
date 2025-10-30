#ifndef _LCD_GRAPHICS_H_
#define _LCD_GRAPHICS_H_

#include "stm32f30x_conf.h"
#include "lcd.h"

/* Defined values for the lcd buffer in lcd.h (repeated for convenience):
 *
 * CHAR_WIDTH 6
 * LCD_BUFF_SIZE 512
 * LCD_LINE_BUFF_SIZE 256
 * LCD_LINE_SIZE 128
 */
#define LCD_HEIGHT 32
#define LCD_SLICE_SIZE 8	// Row height (4 rows each with 128 slices)
#define LCD_VIRTUAL_WIDTH	// Virtual buffer width to enable horizontal scrolling

//void lcd_init_and_print(void);
void lcd_clear_buffer(uint8_t *buffer);
void lcd_draw_fft_window(uint8_t *buffer);
void lcd_draw_fft(uint8_t *buffer);
void lcd_draw_histogram(uint8_t *buffer);
void lcd_draw_pixel(uint8_t *buffer, uint8_t x, uint8_t y);
void lcd_draw_vertical_line(uint8_t *buffer, uint8_t x, uint8_t y_start, uint8_t y_end);
void lcd_draw_horizontal_line(uint8_t *buffer, uint8_t x_start, uint8_t x_end, uint8_t y);
void lcd_draw_bar(uint8_t *buffer, uint8_t x, uint8_t height, uint8_t max_height);

void lcd_draw_number(uint8_t *buffer, int num, uint8_t x, uint8_t y);
//void lcd_draw_x_axis(uint8_t *buffer, uint8_t x_start, uint8_t x_end);
void lcd_draw_axis(uint8_t *buffer);
void lcd_draw_char3x5(uint8_t *buffer, uint8_t x, uint8_t y, char c);

#endif /*! _LCD_GRAPHICS_H_ */
