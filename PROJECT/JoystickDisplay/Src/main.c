#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "lcd.h"
#include "lsm9ds1.h"
#include "spiSlave.h"
#include "sensors.h"

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	initSlaveSPI();
	iniPB12();

	printf("Slave Polling Started.\n");

	uint8_t rxBufferSize = 20;
	uint8_t rxBuffer[rxBufferSize];
	int16_t dataArray[rxBufferSize/2];
	uint8_t samples = 265;
	lsm9ds1_raw_data_t lsmdata;

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



		// update code


		for (int i = 0; i < rxBufferSize / 2; i++) {

			uint8_t highByte = rxBuffer[i * 2];
			uint8_t lowByte  = rxBuffer[i * 2 + 1];

			dataArray[i] = (int16_t)((highByte << 8) | lowByte);
		}




		for (int i = 0; i < samples; i++){

		lsmdata.gx = dataArray[0];
		lsmdata.gy = dataArray[1];
		lsmdata.gz = dataArray[2];
		lsmdata.ax = dataArray[3];
		lsmdata.ay = dataArray[4];
		lsmdata.az = dataArray[5];
		lsmdata.mx = dataArray[6];
		lsmdata.my = dataArray[7];
		lsmdata.mz = dataArray[8];


		}


		// stop while

		printf("Rx: ");
		for (int i = 0; i < rxBufferSize / 2; i++) {
			printf("%d	", dataArray[i]);
		}
		printf("\n");

	}
}

