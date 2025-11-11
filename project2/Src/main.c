#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "lcd.h"
#include "interrupt.h"
//#include "timer.h"
#include "window.h"
#include "flash.h"
#include "lsm9ds1.h"


int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	//	initJoystick();
	//	initLed();
	//	iniEXTIA4();
//	initTimer();
	//	lcd_init_and_print();

	init_SPI_CS();


	initAG();
	initMag();

	while(1) {

		while(readAG(0x0F) != 0x68 || readM(0x0F) != 0x3d){
			printf("Waiting to find WHO AM I REGISTER values\n");

		};
		printf("%x\n",readAG(0x0F));
//		printf("AG = %x M = %x \n",readAG(0x0F),readM(0x0F));
//		printf("AG = %x \n",readAG(0x0F));
////
//		printf("M = %x \n",readM(0x0F));
//
//		for ( int i = 0 ; i<10000; i++);
//
//		printf("M = %x \n ", readM(0x0F));

//		readTempteratureC();
		//		printGyroXYZ();
		//		printAccelXYZ();
		//		printMagnetXYZ();

	}
}
