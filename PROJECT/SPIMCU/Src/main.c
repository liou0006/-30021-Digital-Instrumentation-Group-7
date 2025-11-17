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
#include "spiMaster.h"

#define SPISLAVE_BUFFER_SIZE 10

// master main

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud


	//	init_SPI_CS();
	initMasterSPI();
	//	initAG();
	//	initMag();

	//	int16_t txSize[SPISLAVE_BUFFER_SIZE];
	int sizeOfDataArray = 3, sizeOfTxArray = sizeOfDataArray * 2;
	int16_t dataArray[sizeOfDataArray];
	int8_t txSize[sizeOfTxArray];


	while(1) {
		//				while(readAG(0x0F) != 0x68 || readM(0x0F) != 0x3d){
		//		//			printf("Waiting to find WHO AM I REGISTER values\n");
		//
		//					printf("AG = %x , M = %x \n", readAG(0x0F),readM(0x0F));
		//
		//				};

		//		int16_t gyroX = readOutputAG(0x18);
		//		int16_t gyroY = readOutputAG(0x1A);
		//		int16_t gyroZ = readOutputAG(0x1C);
		//
		//		dataArray[0] = gyroX;
		//		dataArray[1] = gyroY;
		//		dataArray[2] = gyroZ;

		dataArray[0] = 123;
		dataArray[1] = 345;
		dataArray[2] = 567;

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


		printf("[0] = %d	 [1] = %d	 [2] = %d \n", dataArray[0],dataArray[1],dataArray[2]);

		for (int i = 0; i < sizeOfDataArray; i++){
			txSize[i * 2] 		= (uint8_t)(dataArray[i] >> 8);
			txSize[i * 2 + 1 ] 	= (uint8_t)(dataArray[i] & 0xFF);
		}

		// sending data via SPI to oter MCU

		GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_RESET);
		//			for(uint32_t i =0; i < 500000;i++);
		printf("PB3 is low \n");
		for (int i = 0; i < sizeOfTxArray; i++){
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET);
			SPI_SendData8(SPI2, txSize[i]);
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) != SET);
			(void)SPI_ReceiveData8(SPI2);
		}
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_SET);
		for(uint32_t i =0; i < 500000;i++);


	}
}
