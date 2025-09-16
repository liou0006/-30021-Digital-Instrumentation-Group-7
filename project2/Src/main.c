#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "lcd.h"
#include "interrupt.h"
#include "timer.h"
#include "window.h"
#include "flash.h"

void lcd_init_and_print(void) {
	static uint8_t lcdBuffer[LCD_BUFF_SIZE];   // Frame buffer for LCD

	init_spi_lcd();        // Initialize SPI + GPIOs and reset LCD
	memset(lcdBuffer, 0x00, LCD_BUFF_SIZE);  // Clear buffer

	// Write "Hello" at x=0, y=0
	lcd_write_string((uint8_t *)"Hello", lcdBuffer, 0, 0);

	// Push buffer content to LCD
	lcd_push_buffer(lcdBuffer);
}



int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud
	initJoystick();
	initLed();
	iniEXTIA4();
	initTimer();

	lcd_init_and_print();

	uint32_t address = 0x0800F800;
	uint16_t tempVal;
	for ( int i = 0 ; i < 10 ; i++ ){
		tempVal = *(uint16_t *)(address + i * 2); // Read Command
		printf("%d ", tempVal);
	}

	float tempfloat = read_float_flash(PG31_BASE,0);


	/*
	init_page_flash(PG31_BASE);
	FLASH_Unlock();
	write_float_flash(PG31_BASE,0,(float)1.0);
	FLASH_Lock();
	tempfloat = read_float_flash(PG31_BASE,0);
	...
	tempval = read_word_flash(PG31_BASE,0);
	if(tempval!=(uint32_t)0xDEADBEEF){
	init_page_flash(PG31_BASE);
	FLASH_Unlock();
	write_word_flash(PG31_BASE,0,0xDEADBEEF);
	FLASH_Lock();
	}
	tempval = read_hword_flash(PG31_BASE,0);
	*/


	while(1) {

	}
}
