#ifndef SPI_LCD_H
#define SPI_LCD_H

#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "lcd.h"
#include <stdint.h>

// Core functions
void initLCD(void);
void transmit_bytes(uint8_t *data, uint16_t length);
void push_lcd_buffer(uint8_t *buffer);
void reset_lcd_screen(void);

// Write to LCD screen
void lcd_fill(uint8_t pattern);                            // fill framebuffer with a byte pattern
void lcd_draw_pixel(uint8_t x, uint8_t y, uint8_t on);     // set/clear a single pixel
void lcd_draw_char(uint8_t x, uint8_t y, char c);          // draw one 5x7 ASCII character
void lcd_draw_string(uint8_t x, uint8_t y, const char *s); // draw a string of characters

#endif // SPI_LCD_H
