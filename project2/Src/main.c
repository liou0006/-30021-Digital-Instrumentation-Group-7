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
		printf("%d\n", tempVal);
	}

		init_page_flash(PG31_BASE);
		FLASH_Unlock();
		write_word_flash(PG31_BASE, 0, 0x01010101);
		write_word_flash(PG31_BASE, 1, 0x02020202);
		write_word_flash(PG31_BASE, 2, 0x03030303);
		write_word_flash(PG31_BASE, 3, 0x04040404);
		FLASH_Lock();

	int32_t value = 0;
	for (int i = 0; i <= 3;i++){
		value = read_word_flash(PG31_BASE,i);
		printf("%x\n",value);
	}


	while(1) {

	}
}
