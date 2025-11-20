/************************************************
 * EXERCISE 1.1 + 1.2
 ************************************************/

#include "stm32f30x_conf.h"	// STM32 config
#include "30010_io.h"			// Input/output library for this course
#include "joystick.h"
#include "led.h"

int main(void) {
	uart_init( 9600 );			// Initialize USB serial at 9600 baud

	initJoystick();
	initLed();

	int8_t prev_state = readJoystick();
	int8_t current_state;

	while(1) {
		current_state = readJoystick();
		if (prev_state != current_state) {
			if (current_state == 1) {
				setLed('r');
				printf("UP (red)\n");
			} else if (current_state == 2) {
				setLed('g');
				printf("DOWN (green)\n");
			} else if (current_state == 4) {
				setLed('b');
				printf("LEFT (blue)\n");
			} else if (current_state == 8) {
				setLed('c');
				printf("RIGHT (cyan)\n");
			} else if (current_state == 16) {
				setLed('m');
				printf("CENTER (magenta)\n");
			} else {
				setLed('d');
			}
			prev_state = current_state;
		}
	}
}


/************************************************
 * EXERCISE 1.3
 ************************************************/

//#include "stm32f30x_conf.h"	// STM32 config
//#include "30010_io.h"			// Input/output library for this course
//#include "joystick.h"
//#include "led.h"
//#include "interrupt.h"
//
//int main(void) {
//	uart_init( 9600 );			// Initialize USB serial at 9600 baud
//
//	initJoystick();
//	initLed();
//	iniEXTIA4();
//
//	int8_t prev_state = readJoystick();
//	int8_t current_state;
//
//	while(1) {
//
//	}
//}


/************************************************
 * EXERCISE 1.4 + 1.5
 ************************************************/

//#include "stm32f30x_conf.h" // STM32 config
//#include "30010_io.h" 		// Input/output library for this course
//#include "joystick.h"
//#include "led.h"
//#include "timer.h"
//#include "window.h"

//int main(void)
//{
//	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	// --------------- TIMER ---------------
//	initStopWatchWindow();
//	printf("\033[3;1H");	// moves cursor to row 3 column 1
//	printf("Time since start:  \n");
//	printf("Split time 1:      \n");
//	printf("Split time 2:      ");

//	initTimer();
//	initJoystick();

//	uint8_t flag = 0;

//	TIM2_IRQHandler();

//	while(1) {
//		stopWatch(&flag);

//		printf("\033[3;20H");	// moves cursor to row 3 column 20
//		printTime();
//	}

//	/*
//	initTimer();
//	initJoystick();

//	uint8_t flag = 0;

//	TIM2_IRQHandler();

//	uint8_t prev_sec = timeData.seconds;

//	while(1) {
//		stopWatch(&flag);
//		if (prev_sec != timeData.seconds) {
//			printTime();
//			prev_sec = timeData.seconds;
//		}
//		printTime();
//	}
	// -------------------------------------
//	 */




//#include "stm32f30x_conf.h" // STM32 config
//#include "30010_io.h" 		// Input/output library for this course
//#include "joystick.h"
//#include "led.h"
//#include "interrupt.h"
//#include "timer.h"
//#include "window.h"
//
//int main(void) {
//	uart_init( 9600 ); // Initialize USB serial at 9600 baud
//
//	initJoystick();
//	initLed();
//	iniEXTIA4();
//	initTimer();
//
//
//	int val = 10;
//	printf("Value = %02ld\n", val);
//
//	uint8_t a = 10;
//	char str[7];
//	sprintf(str, "a = %2d", a);
//
//	printf("%s\n",str);
//
//	float floatVal = 1.234;
//	printf("%6f\n", floatVal);
//
//		int8_t prev_state = readJoystick();
//		int8_t current_state;
//
//	while(1) {
//		handleJoystick();
//		current_state = readJoystick();
//		if (prev_state != current_state) {
//			if (current_state == 1) {
//				printf("UP (red)\n");
//			} else if (current_state == 2) {
//				printf("DOWN (green)\n");
//			} else if (current_state == 4) {
//				printf("LEFT (blue)\n");
//			} else if (current_state == 8) {
//				printf("RIGHT (cyan)\n");
//			} else if (current_state == 16) {
//				printf("CENTER (magenta)\n");
//			} else {
//				setLed('d');
//			}
//			prev_state = current_state;
//		}
//	}
//}




/************************************************
 * EXERCISE 2.1
 ************************************************/
//
//#include "stm32f30x_conf.h" // STM32 config
//#include "30010_io.h"       // Input/output library for this course
//#include "lcd.h"
//
//void lcd_init_and_print(void) {
//    static uint8_t lcdBuffer[LCD_BUFF_SIZE];	// Frame buffer for LCD
//
//    init_spi_lcd();         // Initialize SPI + GPIOs and reset LCD
//    memset(lcdBuffer, 0x00, LCD_BUFF_SIZE);		// Clear buffer
//
//    // Write formatted text
//    char buffer[32];        // Adjust size for LCD width
//
//    int val1 = 42;
//    float val2 = 3.3;
//
//    sprintf(buffer, "Val1=%d, Val2=%.2f", val1, val2);  // Format string into buffer
//
//    lcd_write_string((uint8_t *)buffer, lcdBuffer, 0, 0);
//
//    // Push buffer content to LCD
//    lcd_push_buffer(lcdBuffer);
//}
//
//int main(void) {
//	uart_init( 9600 ); // Initialize USB serial at 9600 baud
//
//	lcd_init_and_print();
//
//	while(1) {}
//}



/************************************************
 * EXERCISE 2.2
 ************************************************/

//#include "stm32f30x_conf.h" // STM32 config
//#include "30010_io.h"       // Input/output library for this course
//#include "lcd.h"
//
//void lcd_init_and_print(void) {
//    static uint8_t lcdBuffer[LCD_BUFF_SIZE];	// Frame buffer for LCD
//
//    init_spi_lcd();         // Initialize SPI + GPIOs and reset LCD
//    memset(lcdBuffer, 0x00, LCD_BUFF_SIZE);		// Clear buffer
//
//    // Write formatted text
//    char buffer[32];        // Adjust size for LCD width
//
//    int val1 = 42;
//    float val2 = 3.3;
//
//    sprintf(buffer, "Val1=%d, Val2=%.2f", val1, val2);  // Format string into buffer
//
//    lcd_write_string((uint8_t *)buffer, lcdBuffer, 0, 0);
//
//    // Push buffer content to LCD
//    lcd_push_buffer(lcdBuffer);
//}
//
//int main(void) {
//	uart_init( 9600 ); // Initialize USB serial at 9600 baud
//
//	lcd_init_and_print();
//
//	while(1) {}
//}


/************************************************
 * EXERCISE 2.3
 ************************************************/

//#include "stm32f30x_conf.h" // STM32 config
//#include "30010_io.h"       // Input/output library for this course
//#include "lcd.h"
//
//int main(void) {
//	uart_init( 9600 ); // Initialize USB serial at 9600 baud
//	initJoystick();
//	initLed();
//	iniEXTIA4();
//	initTimer();
//
//	lcd_init_and_print();
//
//	uint32_t address = 0x0800F800;
//	uint16_t tempVal;
//	for ( int i = 0 ; i < 10 ; i++ ){
//		tempVal = *(uint16_t *)(address + i * 2); // Read Command
//		printf("%d\n", tempVal);
//	}
//
//		init_page_flash(PG31_BASE);
//		FLASH_Unlock();
//		write_word_flash(PG31_BASE, 0, 0x01010101);
//		write_word_flash(PG31_BASE, 1, 0x02020202);
//		write_word_flash(PG31_BASE, 2, 0x03030303);
//		write_word_flash(PG31_BASE, 3, 0x04040404);
//		FLASH_Lock();
//
//	int32_t value = 0;
//	for (int i = 0; i <= 3;i++){
//		value = read_word_flash(PG31_BASE,i);
//		printf("%x\n",value);
//	}
//
//
//	while(1) {
//
//	}
//}


/************************************************
 * EXERCISE 2.4 + 2.5
 ************************************************/

//#include "stm32f30x_conf.h" // STM32 config
//#include "30010_io.h" 		// Input/output library for this course
//#include "joystick.h"
//#include "led.h"
//#include "lcd.h"
//#include "interrupt.h"
//#include "timer.h"
//#include "window.h"
//#include "flash.h"
//#include "adc.h"
//#include "math.h"
//
//#define VREFINT_CAL *((uint16_t*) ((uint32_t) 0x1FFFF7BA)) //calibrated at 3.3V@ 30
//static uint8_t lcdBuffer[LCD_BUFF_SIZE];
//
//int main(void) {
//	uart_init( 9600 ); // Initialize USB serial at 9600 baud
//
//	initJoystick();
//	initLed();
//	iniEXTIA4();
//	initTimer();
//
//	lcd_init_and_print();
//
//	ADC_setup_PA();
//	ADC_Setup_VREFEN();
//
//	while(1) {
//		static uint8_t lastDeci = 255;
//		uint8_t curDeci = timeData.hundredths / 10;
//		if (curDeci != lastDeci) {
//			lastDeci = curDeci;
//
//			uint16_t pa0 = ADC_measure_PA(1);
//			uint16_t VREF = ADC_measure_VREF();
//			float V_DDA = 3.3 * VREFINT_CAL / VREF;
//
//			float Vch0 = (V_DDA *pa0)/ (pow(2,12)-1);
//
//			char line0[24], line1[24], line2[24], line3[24];
//			sprintf(line0, "VRef: %4u", VREF);
//			sprintf(line1, "VDDA: %.2fV", V_DDA);
//			sprintf(line2, "PA0: %4u", pa0);
//			sprintf(line3, "Vch0: %.2f", Vch0);
//
//			lcd_write_string((uint8_t *)line0, lcdBuffer, 0, 0);
//			lcd_write_string((uint8_t *)line1, lcdBuffer, 0, 1);
//			lcd_write_string((uint8_t *)line2, lcdBuffer, 0, 2);
//			lcd_write_string((uint8_t *)line3, lcdBuffer, 0, 3);
//			lcd_push_buffer(lcdBuffer);
//
//			for(uint32_t i = 0; i<1000;i++);
//
//		}
//	}
//}


/************************************************
 * EXERCISE 3.1 + 3.2
 ************************************************/

//#include "stm32f30x_conf.h" // STM32 config
//#include "30010_io.h" 		// Input/output library for this course
//#include "joystick.h"
//#include "led.h"
//#include "lcd.h"
//#include "interrupt.h"
//#include "timer.h"
//#include "window.h"
//#include "flash.h"
//#include "adc.h"
//#include "math.h"
//#include "pwm.h"
//
//#define VREFINT_CAL *((uint16_t*) ((uint32_t) 0x1FFFF7BA)) //calibrated at 3.3V@ 30
//static uint8_t lcdBuffer[LCD_BUFF_SIZE];
//
//int main(void) {
//	uart_init( 9600 ); // Initialize USB serial at 9600 baud
//	initJoystick();
//	initLed();
//	iniEXTIA4();
//	initTimer();
//	lcd_init_and_print();
//	ADC_setup_PA();
//	ADC_Setup_VREFEN();
//	timer16_pwm_init();
//
//	//setting initial duty cycle
//	int dutyCycle = 100;
//	setDutyCycle(dutyCycle);
//
//	while(1) {
//
//		static uint8_t lastDeci = 255;
//		uint8_t curDeci = timeData.hundredths / 10;
//		if (curDeci != lastDeci) {
//			lastDeci = curDeci;
//
//			uint16_t VREF = ADC_measure_VREF();
//			float V_DDA = 3.3 * VREFINT_CAL / VREF;
//
//			uint16_t pa0 = ADC_measure_PA(1);
//
//			// when PWM has a duty cycle of 50 it switches between
//			// PA0: 0 and 4080
//			// V = 0V and 2.83V
//
//			float Vch0 = (V_DDA *pa0)/ (4095);
//
//			float V_desired = 1.0;
//			float error = V_desired - Vch0;
//
//			float kp = 10; //increase or decrease for faster response
//			float cp = kp * error; // Proportional controller / might need to make a integral if we desire absolute 1V
//			int newDutyCycle = dutyCycle + cp;
//
//			// maybe set the boundary conditions to 5 for the lower and 250 for the upper
//			//so the duty cycle doesn't die out suddenly
//
//			if(newDutyCycle <= 0){
//				newDutyCycle = 0;
//			} else if (newDutyCycle >= 255){
//				newDutyCycle = 255;
//			}
//
//			printf("V_meas=%.2f, Error=%.2f, Duty=%d\n", Vch0, error, dutyCycle);
//
//
//
//			setDutyCycle(newDutyCycle);
//			dutyCycle = newDutyCycle;
//
//			char line0[24], line1[24], line2[24], line3[24];
////			sprintf(line0, "VRef: %4u", VREF);
//			sprintf(line1, "Duty cycle: %d", dutyCycle);
//			sprintf(line2, "PA0: %4u", pa0);
//			sprintf(line3, "V_meas: %.2f", Vch0);
//
////			lcd_write_string((uint8_t *)line0, lcdBuffer, 0, 0);
//			lcd_write_string((uint8_t *)line1, lcdBuffer, 0, 1);
//			lcd_write_string((uint8_t *)line2, lcdBuffer, 0, 2);
//			lcd_write_string((uint8_t *)line3, lcdBuffer, 0, 3);
//			lcd_push_buffer(lcdBuffer);
//
//			// for(uint32_t i = 0; i<5000000;i++); //delay function but not good if you want the system to use the software feedback
//
//		}
//	}
//}









