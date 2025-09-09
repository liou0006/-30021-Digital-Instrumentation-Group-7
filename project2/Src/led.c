#include "led.h"

// Code needed in main.c is included in the bottom of the file

void initLED() {
	// Enable clock for GPIO Ports
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	// Port A
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);	// Port B
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);	// Port C
	GPIO_InitTypeDef GPIO_InitStructAll; 				// Define typedef struct for setting pins


	// Sets PA9 (BLUE) to output
	GPIO_StructInit(&GPIO_InitStructAll);				// Initialize GPIO struct
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT;		// Set as output
	GPIO_InitStructAll.GPIO_OType = GPIO_OType_PP;		// Set as Push-Pull
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_9;			// Set so the configuration is on pin 9
	GPIO_InitStructAll.GPIO_Speed = GPIO_Speed_2MHz;	// Set speed to 2 MHz
														// For all options see SPL/inc/stm32f30x_gpio.h
	GPIO_Init(GPIOA, &GPIO_InitStructAll); 				// Setup of GPIO with the settings chosen

	// Sets PB4 (RED) to output
	GPIO_StructInit(&GPIO_InitStructAll);
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructAll.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructAll.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructAll);

	// Sets PC7 (GREEN) to output
	GPIO_StructInit(&GPIO_InitStructAll);
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructAll.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructAll.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructAll);

	// Initialize LED to off
	setLED('d');
}

void setLED(char sel) {
	/* From [MBED-016.1_ApplicationShieldCookbook] pp. 3-4 section
	 * 6. RGB LED "0" is on and "1" is off.
	 */

	/*			R	G	B
	 * White: 	255 255 255
	 * Red:		255 0	0
	 * Blue:	0	0	255
	 * Green:	0	255	0
	 * Yellow	255	255	0
	 * Cyan:	0	255	255
	 * Magenta:	255	0	255
	 */

	if (sel == 'r') {
		GPIO_WriteBit(GPIOB, GPIO_Pin_4, 0);	// red
		GPIO_WriteBit(GPIOC, GPIO_Pin_7, 1);	// green
		GPIO_WriteBit(GPIOA, GPIO_Pin_9, 1);	// blue
	} else if (sel == 'g') {
		GPIO_WriteBit(GPIOB, GPIO_Pin_4, 1);	// red
		GPIO_WriteBit(GPIOC, GPIO_Pin_7, 0);	// green
		GPIO_WriteBit(GPIOA, GPIO_Pin_9, 1);	// blue
	} else if (sel == 'b') {
		GPIO_WriteBit(GPIOB, GPIO_Pin_4, 1);	// red
		GPIO_WriteBit(GPIOC, GPIO_Pin_7, 1);	// green
		GPIO_WriteBit(GPIOA, GPIO_Pin_9, 0);	// blue
	} else if (sel == 'c') {
		GPIO_WriteBit(GPIOB, GPIO_Pin_4, 1);	// red
		GPIO_WriteBit(GPIOC, GPIO_Pin_7, 0);	// green
		GPIO_WriteBit(GPIOA, GPIO_Pin_9, 0);	// blue
	} else if (sel == 'm') {
		GPIO_WriteBit(GPIOB, GPIO_Pin_4, 0);	// red
		GPIO_WriteBit(GPIOC, GPIO_Pin_7, 1);	// green
		GPIO_WriteBit(GPIOA, GPIO_Pin_9, 0);	// blue
	} else if (sel == 'y') {
		GPIO_WriteBit(GPIOB, GPIO_Pin_4, 0);	// red
		GPIO_WriteBit(GPIOC, GPIO_Pin_7, 0);	// green
		GPIO_WriteBit(GPIOA, GPIO_Pin_9, 1);	// blue
	} else if (sel == 'w') {
		GPIO_WriteBit(GPIOB, GPIO_Pin_4, 0);	// red
		GPIO_WriteBit(GPIOC, GPIO_Pin_7, 0);	// green
		GPIO_WriteBit(GPIOA, GPIO_Pin_9, 0);	// blue
	} else if (sel == 'd') {
		GPIO_WriteBit(GPIOB, GPIO_Pin_4, 1);	// red
		GPIO_WriteBit(GPIOC, GPIO_Pin_7, 1);	// green
		GPIO_WriteBit(GPIOA, GPIO_Pin_9, 1);	// blue
	}
}



// main.c code
/*
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course

#include "joystick.h"
#include "led.h"

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	initJoystick();
	initLED();

	int8_t prev_state = readJoystick();
	int8_t current_state;

	while(1) {
		current_state = readJoystick();
		if (prev_state != current_state) {
			if (current_state == 1) {
				setLED('r');
				printf("UP (red)\n");
			} else if (current_state == 2) {
				setLED('g');
				printf("DOWN (green)\n");
			} else if (current_state == 4) {
				setLED('b');
				printf("LEFT (blue)\n");
			} else if (current_state == 8) {
				setLED('c');
				printf("RIGHT (cyan)\n");
			} else if (current_state == 16) {
				setLED('m');
				printf("CENTER (magenta)\n");
			} else {
				setLED('d');
			}
			prev_state = current_state;
		}
	}
}
*/