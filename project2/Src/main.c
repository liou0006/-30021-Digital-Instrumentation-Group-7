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


		readSPI2(0x0F,GPIO_Pin_5);

		for (int i= 0 ; i<= 10000;i++);

//		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
//		spi2_transfer(0x80 | 0x0F);
//		spi2_transfer(0x00);
//		int8_t val = SPI_ReceiveData8(SPI2); // dummy write to clock in data
//		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
//		printf("AG WHO_AM_I = %d , %x, % u \n", val, val, val);

	}
}
