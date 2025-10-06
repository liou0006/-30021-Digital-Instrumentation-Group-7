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

uint8_t lsm9_ag_read(uint8_t reg) {
	// CS low (use your actual AG CS pin)
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);

	int8_t dataline = 0x80 | reg;   // send address with READ bit
	SPI_SendData8(SPI2, dataline);

	SPI_SendData8(SPI2, 0x00);

	int8_t val = SPI_ReceiveData8(SPI2); // dummy write to clock in data
	// CS high
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);


	return val;


}

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud
	initJoystick();
	initLed();
	iniEXTIA4();
	initTimer();

	lcd_init_and_print();

	init_spi_gyro_accel();



	int8_t readWrite = 0x80;
	int8_t address = WHO_AM_I;
	int8_t data = 0xFF;

	//
	//	int16_t rwAddress = (readWrite | address) << 8;
	//	int16_t writeLine = rwAddress | data;
	//
	//	while (!(SPI2->SR & SPI_SR_TXE));
	//	SPI2->DR = rwAddress;
	//	while (!(SPI2->SR & SPI_SR_RXNE));
	//	uint16_t result = SPI2->DR;
	//	uint8_t who = result & 0xFF; // lower 8 bits are response
	//
	//	GPIO_WriteBit(GPIOB, GPIO_Pin_5, 0);
	//	spi_transmit(rwAddress);
	////	spi_transmit(writeLine);
	//	GPIO_WriteBit(GPIOB, GPIO_Pin_5, 1);
	////int temp = SPI_ReceiveData8(SPI2);
	//
	//printf("%u\n",who);


	while(1) {
	uint8_t who_ag= lsm9_ag_read(address);
		printf("AG WHO_AM_I = 0x%02X\n", who_ag);
	}
}
