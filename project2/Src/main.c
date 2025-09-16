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


#include "interrupt.h"
#include "timer.h"
//#include "window.h"

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	initJoystick();
	initLED();
	iniEXTIA4();
	initTimer();


	int val = 10;
	printf("Value = %02ld\n", val);

	uint8_t a = 10;
	char str[7];
	sprintf(str, "a = %2d", a);

	printf("%s\n",str);

	float floatVal = 1.234;
	printf("%6f\n", floatVal);

	//	int8_t prev_state = readJoystick();
	//	int8_t current_state;
	//initLed();
    lcd_init_and_print();

	while(1) {
		handleJoystick();



		//		current_state = readJoystick();
		//		if (prev_state != current_state) {
		//			if (current_state == 1) {
		////				setLED('r');
		//				printf("UP (red)\n");
		//			} else if (current_state == 2) {
		////				setLED('g');
		//				printf("DOWN (green)\n");
		//			} else if (current_state == 4) {
		////				setLED('b');
		//				printf("LEFT (blue)\n");
		//			} else if (current_state == 8) {
		////				setLED('c');
		//				printf("RIGHT (cyan)\n");
		//			} else if (current_state == 16) {
		////				setLED('m');
		//				printf("CENTER (magenta)\n");
		//			} else {
		//				setLED('d');
		//			}
		//			prev_state = current_state;
		//		}
	}
}
