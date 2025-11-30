#ifndef _LCD_GRAPHICS_H_
#define _LCD_GRAPHICS_H_

/***********************************************************
 * Includes
 ***********************************************************/
#include "stm32f30x_conf.h"
#include <stdint.h>
#include <string.h>
#include "lcd.h"

/***********************************************************
 * Defines
 ***********************************************************/
#define LCD_HEIGHT 32
#define LCD_SLICE_SIZE 8
#define LCD_ROWS 4
#define VIRTUAL_WIDTH_SIZE 256

// Graph window
#define GRAPH_X_OFFSET 9
#define GRAPH_MARGIN_RIGHT 5
#define GRAPH_MARGIN_TOP 3
#define GRAPH_MARGIN_BOTTOM 6

// Char displaying
#define DIGIT_WIDTH 3
#define DOT_WIDTH 1
#define CHAR_SPACING 1

/***********************************************************
 * Global
 ***********************************************************/
extern uint8_t lcdBuffer[LCD_BUFF_SIZE];
extern uint8_t virtualBuffer[VIRTUAL_WIDTH_SIZE * LCD_ROWS];

/***********************************************************
 * Functions
 ***********************************************************/
void lcd_clear_buffer(uint8_t *buffer, uint16_t buff_size);
uint16_t graph_y_to_lcd_y(uint16_t graph_y);
void draw_graph_axis();
void draw_new_axis(uint16_t x_offset);
void lcd_draw_pixel(uint8_t *buffer, uint16_t buff_width, uint16_t x, uint16_t y);
void lcd_draw_vertical_line(uint8_t *buffer, uint16_t buff_width, uint16_t x, uint16_t y_start, uint16_t y_end);
void lcd_draw_horizontal_line(uint8_t *buffer, uint16_t buff_width, uint16_t x_start, uint16_t x_end, uint16_t y);
void lcd_draw_char3x5(uint8_t *buffer, uint16_t buff_width, uint16_t x, uint16_t y, char c);
void lcd_convert_int_to_char3x5_y_axis(uint8_t *buffer, uint16_t buff_width, int max_num_digits, int val, uint16_t x, uint16_t y);
void lcd_convert_int_to_char3x5_x_axis(uint8_t *buffer, uint16_t buff_width, int val, uint16_t x, uint16_t y);
void lcd_convert_float_to_char3x5_y_axis(uint8_t *buffer, uint16_t buff_width, int max_num_digits, float val, uint16_t x, uint16_t y, int decimals);
void lcd_convert_float_to_char3x5_x_axis(uint8_t *buffer, uint16_t buff_width,
		float val, uint16_t x, uint16_t y, int decimals);
void update_lcdBuffer();

#endif /*! _LCD_GRAPHICS_H_ */
