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

//	initMasterSPI();

	init_SPI_CS();
	initAG();
	initMag();

	//	int16_t txSize[SPISLAVE_BUFFER_SIZE];
	int sizeTemp = 3;
	uint8_t txSize[sizeTemp*2];
	uint16_t dataArray[sizeTemp];


	while(1) {
		while(readAG(0x0F) != 0x68 || readM(0x0F) != 0x3d){
			printf("Waiting to find WHO AM I REGISTER values\n");

		};



//		printf("%d\n",dataArray[0]);


//for (int i = 0; i < 10000; i++);


//				for (int i = 0; i < sizeTemp; i++){
//					txSize[i * 2] 		= (uint8_t)(dataArray[i] >> 8);
//					txSize[i * 2 + 1 ] 	= (uint8_t)(dataArray[i] & 0xFF);
//				}

		// sending data to SPI
		//		write to other MCUs SPI


//		for (int i = 0; i < sizeTemp * 2; i++){
//			GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_RESET);
//			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET);
//			SPI_SendData8(SPI2, txSize[i]);
//			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) != SET);
//			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
//			GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_SET);
//		}
//
//		memset(dataArray, 0, sizeof(dataArray));

		printf("AG = %x | M = %x \n", readAG(0x0F));
//		printf("AG = %x | M = %x \n", readAG(0x0F), readM(0x0F));
//				readTempteratureC();
//				printGyroXYZ();
//				printAccelXYZ();
		//		printMagnetXYZ();




	}
}
