#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "lcd.h"

void initLCD(void){
	uint8_t fbuffer[512];
	memset(fbuffer, 0xAA, 512);

}


void transmit_bytes(uint8_t *data, uint16_t length){

}

void push_lcd_buffer(uint8_t *buffer){
	
}

void reset_lcd_screen(void){

}

