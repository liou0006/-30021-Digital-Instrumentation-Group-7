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
#include "spiSlave.h"

#define SPISLAVE_BUFFER_SIZE 10

// Slave main

int main(void) {
	uart_init( 115200 ); // Initialize USB serial at 9600 baud

	initSlaveSPI();
	initAG();
	initMag();

	//	int16_t txSize[SPISLAVE_BUFFER_SIZE];
	int sizeTemp = 3;
	uint8_t txSize[sizeTemp*2];

	uint16_t dataArray[sizeTemp];


	while(1) {

		uint16_t gyroX = readOutputAG(0x18);
		uint16_t gyroY = readOutputAG(0x1A);
		uint16_t gyroZ = readOutputAG(0x1C);

//		int16_t accelX = readOutputAG(0x28);
//		int16_t accelY = readOutputAG(0x2A);
//		int16_t accelZ = readOutputAG(0x2C);
//
//		int16_t tempVal = readOutputAG(0x15);
//		float tempC = 25.0f + (tempVal /16.0f);
//
//		int16_t magnetX = readOutputM(0x28);
//		int16_t magnetY = readOutputM(0x2A);
//		int16_t magnetZ = readOutputM(0x2C);

		dataArray[0] = gyroX;
		dataArray[1] = gyroY;
		dataArray[2] = gyroZ;

//		printf("%d\n",dataArray[0]);


//for (int i = 0; i < 10000; i++);


				for (int i = 0; i < sizeTemp; i++){
					txSize[i * 2] 		= (uint8_t)(dataArray[i] >> 8);
					txSize[i * 2 + 1 ] 	= (uint8_t)(dataArray[i] & 0xFF);

				}

		// sending data to SPI
		//		write to other MCUs SPI
		for (int i = 0; i < sizeTemp * 2; i++){
			GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_RESET);
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET);
			SPI_SendData8(SPI2, txSize[i]);
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) != SET);
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
			GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_SET);
		}

		memset(dataArray, 0, sizeof(dataArray));


		//		readTempteratureC();
		//		printGyroXYZ();
		//		printAccelXYZ();
		//		printMagnetXYZ();




	}
}
