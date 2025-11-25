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


	while(1) {


		menu_update();
		lcd_push_buffer(lcdBuffer);


	}
}

//#include "stm32f30x.h"
//#include "ultrasonic_sensor.h"
//#include <stdint.h>
//#include "openlog_sd.h"
//#include "imu_stub.h"

//int main(void) {
//	SystemInit();
//	SystemCoreClockUpdate();
//	init_uart(115200);
//	delay(1000); // wait for openlog to boot

//	int count = 0;
//	uint8_t packet[IMU_PACKET_SIZE];

//	while (count < 1000) {
		// Get next IMU telemetry packet (18 bytes of raw binary)
//		imu_stub_next_packet(packet);

		// Write it directly to SD via OpenLog
//		openlog_writebytes(packet, IMU_PACKET_SIZE);
//		count++;
//		delay(1);   // a few ms between samples
//	}
//	reset();
//	while (1); // halt after done
//}
