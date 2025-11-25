#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "lsm9ds1.h"
#include "spiMaster.h"

#define DATASIZE 10

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	initMasterSPI();
	initAG();
	initMag();

	int sizeOfDataArray = DATASIZE, sizeOfTxArray = sizeOfDataArray * 2;
	int16_t dataArray[sizeOfDataArray];
	uint8_t txSize[sizeOfTxArray];


	while(readAG(0x0F) != 0x68 || readM(0x0F) != 0x3d){
		printf("AG = %x , M = %x \n", readAG(0x0F),readM(0x0F));

	};

	while(1) {

		int16_t gX = readOutputAG(0x18);
		int16_t gY = readOutputAG(0x1A);
		int16_t gZ = readOutputAG(0x1C);
		int16_t aX = readOutputAG(0x28);
		int16_t aY = readOutputAG(0x2A);
		int16_t aZ = readOutputAG(0x2C);
		int16_t tempVal = readOutputAG(0x15);
		int16_t mX = readOutputM(0x28);
		int16_t mY = readOutputM(0x2A);
		int16_t mZ = readOutputM(0x2C);
		float tempC = 25.0f + (tempVal /16.0f);

		dataArray[0] = gX;
		dataArray[1] = gY;
		dataArray[2] = gZ;
		dataArray[3] = aX;
		dataArray[4] = aY;
		dataArray[5] = aZ;
		dataArray[6] = mX;
		dataArray[7] = mY;
		dataArray[8] = mZ;
		dataArray[9] = tempC;


        printf("|%7d|%7d|%7d|%7d|%7d|%7d|%6d|%6d|%6d|%5d |\n",
                dataArray[0],
                dataArray[1],
                dataArray[2],
                dataArray[3],
                dataArray[4],
                dataArray[5],
                dataArray[6],
                dataArray[7],
                dataArray[8],
                dataArray[9]);


		for (int i = 0; i < sizeOfDataArray; i++){
			txSize[i * 2] 		= (uint8_t)(dataArray[i] >> 8);
			txSize[i * 2 + 1 ] 	= (uint8_t)(dataArray[i] & 0xFF);
		}

		GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_RESET);
		for(uint32_t i =0; i < 5000;i++);
		for (int i = 0; i < sizeOfTxArray; i++){
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET);
			SPI_SendData8(SPI2, txSize[i]);
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) != SET);
			(void)SPI_ReceiveData8(SPI2);
		}
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_SET);

	}
}
