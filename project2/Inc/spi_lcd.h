#ifndef SPI_LCD_H
#define SPI_LCD_H

#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "lcd.h"


void initLCD(void);

void transmit_bytes(uint8_t *data, uint16_t length);

void push_lcd_buffer(uint8_t *buffer);

void reset_lcd_screen(void);

#endif // SPI_LCD_H
