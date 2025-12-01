#ifndef _LCD_GRAPHICS_H_
#define _LCD_GRAPHICS_H_

// ---------- Includes ----------
#include "stm32f30x_conf.h"
#include <stdint.h>
#include <string.h>
#include "lcd.h"

// ---------- Defines ----------
// Additional LCD dimensions not defined in lcd.h
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

// ---------- Extern LCD buffers ----------
extern uint8_t lcdBuffer[LCD_BUFF_SIZE];
extern uint8_t virtualBuffer[VIRTUAL_WIDTH_SIZE * LCD_ROWS];

// ---------- Functions ----------
void lcd_clear_buffer(uint8_t *buffer, uint16_t buff_size);

// Helper function to convert from graph y-axis (increase from bottom to top)
// to LCD y-axis (increase top to bottom)
uint16_t graph_y_to_lcd_y(uint16_t graph_y);

// Draw default axis lines or draw default axis with x offset
void draw_graph_axis();
void draw_new_axis(uint16_t x_offset);

// Various draw functions
void lcd_draw_pixel(uint8_t *buffer, uint16_t buff_width, uint16_t x, uint16_t y);
void lcd_draw_vertical_line(uint8_t *buffer, uint16_t buff_width, uint16_t x, uint16_t y_start, uint16_t y_end);
void lcd_draw_horizontal_line(uint8_t *buffer, uint16_t buff_width, uint16_t x_start, uint16_t x_end, uint16_t y);
void lcd_draw_char3x5(uint8_t *buffer, uint16_t buff_width, uint16_t x, uint16_t y, char c);

// Convert int/float to 3x5 char (used for x-/y-axis label drawing)
void lcd_convert_int_to_char3x5_y_axis(uint8_t *buffer, uint16_t buff_width, int max_num_digits, int val, uint16_t x, uint16_t y);
void lcd_convert_int_to_char3x5_x_axis(uint8_t *buffer, uint16_t buff_width, int val, uint16_t x, uint16_t y);
void lcd_convert_float_to_char3x5_y_axis(uint8_t *buffer, uint16_t buff_width, int max_num_digits, float val, uint16_t x, uint16_t y, int decimals);
void lcd_convert_float_to_char3x5_x_axis(uint8_t *buffer, uint16_t buff_width, float val, uint16_t x, uint16_t y, int decimals);

// Copy section of virtualBuffer that corresponds to pot2 value and display that
void update_lcdBuffer();

#endif /*! _LCD_GRAPHICS_H_ */
