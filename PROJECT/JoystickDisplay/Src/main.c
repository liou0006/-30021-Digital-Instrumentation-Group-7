#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course

// Includes with functions called from main
#include "joystick.h"
#include "adc.h"
#include "lcd_graphics.h"
#include "lcd.h"
#include "menu.h"
#include "lsm9ds1.h"
#include "spiSlave.h"
#include "sensors.h"
#include "stm32f30x.h"
#include <stdint.h>
#include "openlog_sd.h"
#include "imu_stub.h"


// Define buffers (allocate memory)
uint8_t lcdBuffer[LCD_BUFF_SIZE];
uint8_t virtualBuffer[VIRTUAL_WIDTH_SIZE * LCD_ROWS];

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud
	init_uart(115200); // Initialize UART3 for writing to SD

	// Initialize Origin
	initJoystick();		// Enabling GPIO pins for joystick
	init_spi_lcd();		// Initialize SPI for LCD
	ADC_setup_PA();		// Enabling GPIO pins for ADC
	menu_init();		// Initialize main menu
	initSlaveSPI();
	iniPB12();
	// Initialize SD
	SystemInit();
	SystemCoreClockUpdate();
	delay(1000);

	uint8_t rxBufferSize = 20;
	uint8_t rxBuffer[rxBufferSize];
	int16_t dataArray[rxBufferSize/2];
	uint8_t sampleIndex = 0;
	uint16_t maxData = 50; // ændre det til 256
	lsm9ds1_raw_data_t lsmdata[maxData];
	uint8_t collect = 1;

	uint8_t packet[IMU_PACKET_SIZE];

	while(1) {


//		menu_update();


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



		if (sampleIndex != maxData){
			printf("Collected %d\n",sampleIndex);
			lsmdata[sampleIndex].gx = dataArray[0];
			lsmdata[sampleIndex].gy = dataArray[1];
			lsmdata[sampleIndex].gz = dataArray[2];
			lsmdata[sampleIndex].ax = dataArray[3];
			lsmdata[sampleIndex].ay = dataArray[4];
			lsmdata[sampleIndex].az = dataArray[5];
			lsmdata[sampleIndex].mx = dataArray[6];
			lsmdata[sampleIndex].my = dataArray[7];
			lsmdata[sampleIndex].mz = dataArray[8];
			lsmdata[sampleIndex].T = dataArray[9];
			sampleIndex++;
			//		imu_stub_next_packet(packet);

			// Write it directly to SD via OpenLog
			sensors_pack_raw(&lsmdata[sampleIndex], packet);

			// Write to SD
			openlog_writebytes(packet, IMU_PACKET_SIZE);
		}
		else if (sampleIndex == maxData){
			printf("Data sent\n");
			plot_histogram(lsmdata,SENSOR_ACCEL, AXIS_X);
			lcd_push_buffer(lcdBuffer);
			sampleIndex = maxData; // should be 0
			collect = 0;
		}
		else {
			printf("Error");
		}


		//		print on PuTTY
		//		printf("Rx: ");
		//		for (int i = 0; i < rxBufferSize / 2; i++) {
		//			printf("%d	", dataArray[i]);
		//		}
		//		printf("\n");

	}
}

//int main(void) {

//	while (count < 1000) {
		// Get next IMU telemetry packet (18 bytes of raw binary)

//	}
//	reset();
//	while (1); // halt after done
//}
