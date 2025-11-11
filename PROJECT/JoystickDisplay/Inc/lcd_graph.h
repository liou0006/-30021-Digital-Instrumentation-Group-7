//#ifndef _LCD_GRAPH_H_
//#define _LCD_GRAPH_H_
//
///***********************************************************
// * Includes
// ***********************************************************/
//#include "stm32f30x_conf.h"
//#include <stdint.h>
//#include "lcd.h"
//#include "lcd_draw.h"
//
///***********************************************************
// * Defines
// ***********************************************************/
//#define NUM_SAMPLES 32
//
///***********************************************************
// * Global ...
// ***********************************************************/
//extern uint8_t lcdBuffer[LCD_BUFF_SIZE];
//extern uint8_t virtualBuffer[VIRTUAL_WIDTH_SIZE * LCD_ROWS];
//
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
////	char x_lab[16];			// X axis label
////	char y_lab[16];			// Y axis label
//} fft_graph_t;
//
//// Not updated (could potentially be the same as fft_graph_t)
////typedef struct {
////	uint16_t N;				// Number of data samples
////	const uint16_t *data;	// Constant pointer to sample data
////	uint16_t lcd_width;		// Graph width in pixels
////	uint16_t lcd_height;	// Graph height in pixels
////	uint16_t x_origin;		// X position (left edge)
////	uint16_t y_origin;		// Y position (bottom edge -- opposite of LCD y origin)
////	uint16_t x_max;			// Max. value for X axis
////	uint16_t y_max;			// Max. value for Y axis
////	uint16_t x_min;			// Min. value for X axis
////	uint16_t y_min;			// Min. value for Y axis
////	uint8_t x_lab_spacing;	// Spacing (samples) between X axis labels
////	uint8_t y_lab_spacing;	// Spacing (ticks) between Y axis labels
////	char x_lab[16];			// X axis label
////	char y_lab[16];			// Y axis label
////} hist_graph_t;
//
///***********************************************************
// * Functions
// ***********************************************************/
//void init_fft_graph(fft_graph_t *g, uint16_t *mags);
//
//#endif /*! _LCD_GRAPH_H_ */
