#ifndef _LCD_GRAPHICS_H_
#define _LCD_GRAPHICS_H_

/***********************************************************
 * Includes
 ***********************************************************/
#include "stm32f30x_conf.h"
#include <stdint.h>
#include "lcd.h"
#include "adc.h"

/***********************************************************
 * Defines
 ***********************************************************/
#define LCD_HEIGHT 32
#define LCD_SLICE_SIZE 8
#define LCD_ROWS 4
#define VIRTUAL_WIDTH_SIZE 256
#define NUM_SAMPLES 32
// Graph window
#define GRAPH_HEIGHT 23		// pixels
#define GRAPH_WIDTH 25		// pixels
#define GRAPH_X_OFFSET 8
#define GRAPH_MARGIN_TOP 2
#define GRAPH_MARGIN_BOTTOM 6

/***********************************************************
 * Global ...
 ***********************************************************/
extern uint8_t lcdBuffer[LCD_BUFF_SIZE];
extern uint8_t virtualBuffer[VIRTUAL_WIDTH_SIZE * LCD_ROWS];

/***********************************************************
 * Functions
 ***********************************************************/
void lcd_clear_buffer(uint8_t *buffer, uint16_t buff_size);
uint16_t graph_y_to_lcd_y(uint16_t graph_y);
void draw_graph_axis();
//void lcd_init_graph(graph_area_t *graph);

void lcd_draw_pixel(uint8_t *buffer, uint16_t buff_width, uint16_t x, uint16_t y);
void lcd_draw_fft_mag(uint8_t *buffer, uint16_t buff_width, uint16_t x, uint16_t mag);
void lcd_draw_vertical_line(uint8_t *buffer, uint16_t buff_width, uint16_t x, uint16_t y_start, uint16_t y_end);
void lcd_draw_horizontal_line(uint8_t *buffer, uint16_t buff_width, uint16_t x_start, uint16_t x_end, uint16_t y);
void lcd_draw_char3x5(uint8_t *buffer, uint16_t buff_width, uint16_t x, uint16_t y, char c);
void update_lcdBuffer();

//void lcd_draw_fft_window(uint8_t *buffer);
//void lcd_draw_axis(uint8_t *buffer);
//void lcd_draw_fft(uint8_t *buffer);
//void lcd_draw_histogram(uint8_t *buffer);
//void lcd_draw_bar(uint8_t *buffer, uint8_t x, uint8_t height, uint8_t max_height);

#endif /*! _LCD_GRAPHICS_H_ */
