#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "lcd.h"
#include "interrupt.h"
#include "timer.h"
#include "window.h"
#include "flash.h"
#include "lsm9ds1.h"

#define WHO_AM_I     0x0F
static uint8_t lcdbuffer[LCD_LINE_BUFF_SIZE];

uint8_t readWAI(uint8_t reg) {
	// CS low (use your actual AG CS pin)
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);


	int8_t dataline = 0x80 | 0X0F;   // send address with READ bit
	spi2_transmit(dataline);
	//	SPI_SendData8(SPI2, dataline);

//	spi2_transmit(0x00);

	//	SPI_SendData8(SPI2, 0x00);

	int8_t val = spi2_transmit(0x00); // dummy write to clock in data
	// CS high
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);

	return val;


}

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	//	initJoystick();
	//	initLed();
	//	iniEXTIA4();
	//	initTimer();
	//	lcd_init_and_print();
	//	enableCSPins();
	//	initSPI();

	init_spi_gyro_accel();



	while(1) {

		//		char line1[24];

		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
		spi2_transfer(0x80 | 0x0F);
		int8_t val = spi2_transfer(0x00); // dummy write to clock in data
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
//		printf("AG WHO_AM_I = %x\n", val);

	}
}
