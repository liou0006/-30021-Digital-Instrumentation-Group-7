#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "lcd.h"

void lcd_init_and_print(void) {
    static uint8_t lcdBuffer[LCD_BUFF_SIZE];   // Frame buffer for LCD

    init_spi_lcd();        // Initialize SPI + GPIOs and reset LCD
    memset(lcdBuffer, 0x00, LCD_BUFF_SIZE);  // Clear buffer

    // Write "Hello" at x=0, y=0
    lcd_write_string((uint8_t *)"Hello", lcdBuffer, 0, 0);

    // Push buffer content to LCD
    lcd_push_buffer(lcdBuffer);
}



int main(void)
{
	uart_init( 9600 ); // Initialize USB serial at 9600 baud
	initJoystick();
	//initLed();
    lcd_init_and_print();

	while(1){
	handleJoystick();

	}
}
