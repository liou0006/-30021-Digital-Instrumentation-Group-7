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
#define GRAPH_HEIGHT 24		// pixels
#define GRAPH_WIDTH 251		// pixels
#define GRAPH_X_OFFSET 5
#define GRAPH_MARGIN_TOP 3
#define GRAPH_MARGIN_BOTTOM 6

/***********************************************************
 * Global ...
 ***********************************************************/
extern uint8_t lcdBuffer[LCD_BUFF_SIZE];
extern uint8_t virtualBuffer[VIRTUAL_WIDTH_SIZE * LCD_ROWS];

//typedef struct {
//	uint8_t *buffer;	// Pointer to start of virtual buffer
//	uint16_t width;		// Graph width in pixels
//	uint16_t height;	// graph height in pixels
//	uint16_t x_start;	// x offset into virtual buffer (start position in virtual buffer)
//	uint16_t x_end;		// x end position in virtual buffer
//	uint16_t y_end;		// y limit (since graph y-axis is opposite of LCD y-axis)
//	uint16_t y_start;	// y offset into virtual buffer
//} graph_area_t;

//typedef struct {
//	uint16_t N;				// Number of data samples
//	const uint16_t *data;	// Constant pointer to sample data
//	uint16_t lcd_width;		// Graph width in pixels
//	uint16_t lcd_height;	// Graph height in pixels
//	uint16_t x_origin;		// X position (left edge)
//	uint16_t y_origin;		// Y position (bottom edge -- opposite of LCD y origin)
//	uint16_t x_max;			// Max. value for X axis
//	uint16_t y_max;			// Max. value for Y axis
//	uint16_t x_min;			// Min. value for X axis
//	uint16_t y_min;			// Min. value for Y axis
//	uint8_t x_lab_spacing;	// Spacing (samples) between X axis labels
//	uint8_t y_lab_spacing;	// Spacing (ticks) between Y axis labels
//	char x_lab[16];			// X axis label
//	char y_lab[16];			// Y axis label
//} fft_graph_t;

/***********************************************************
 * Functions
 ***********************************************************/
void lcd_clear_buffer(uint8_t *buffer, uint16_t buff_size);
uint16_t graph_y_to_lcd_y(uint16_t graph_y);
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
