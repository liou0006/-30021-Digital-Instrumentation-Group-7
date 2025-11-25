#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "lcd.h"
#include "lsm9ds1.h"
#include "spiSlave.h"

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	initSlaveSPI();
	iniPB12();

	printf("Slave Polling Started.\n");

	uint8_t rxBufferSize = 20;
	uint8_t rxBuffer[rxBufferSize];
	int16_t dataArray[rxBufferSize/2];

	while(1) {

		// checks if CS pin is low
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_SET);

		//throws away bad signal
		while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == SET) {
			SPI_ReceiveData8(SPI3);
		}

		for (int i = 0; i < rxBufferSize; i++) {
			while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
			rxBuffer[i] = SPI_ReceiveData8(SPI3);
		}

		//checks if CS pin is high
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_RESET);


		for (int i = 0; i < rxBufferSize / 2; i++) {

			uint8_t highByte = rxBuffer[i * 2];
			uint8_t lowByte  = rxBuffer[i * 2 + 1];

			dataArray[i] = (int16_t)((highByte << 8) | lowByte);
		}

		printf("Rx: ");
		for (int i = 0; i < rxBufferSize / 2; i++) {
			printf("%d	", dataArray[i]);
		}
		printf("\n");

	}
}

