#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course

#include "joystick.h"

void initJoystick() {
	// Enable clock for GPIO Ports
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	// Port A
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);	// Port B
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);	// Port C
	GPIO_InitTypeDef GPIO_InitStructAll; 				// Define typedef struct for setting pins

	// Setup for GPIO Ports
	// -- A
	GPIO_StructInit(&GPIO_InitStructAll);				// Initialize GPIO struct
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_IN;		// Set as input
	GPIO_InitStructAll.GPIO_PuPd = GPIO_PuPd_DOWN; 		// Set as pull down
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_4; 			// Set so the configuration is on pin 4
	GPIO_Init(GPIOA, &GPIO_InitStructAll); 				// Setup of GPIO with the settings chosen

	// -- B
	GPIO_StructInit(&GPIO_InitStructAll);
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructAll.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructAll);

	// -- C
	GPIO_StructInit(&GPIO_InitStructAll);
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructAll.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOC, &GPIO_InitStructAll);
}


int8_t readJoystickState() {
	// Output of state depending on joystick state
	// up: 1    down: 2    left: 4    right: 8    center: 16

	// Initialize joystick state variable
	int8_t state = 0;

	// Read joystick pins
	int8_t up = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);		// PA4
	int8_t down = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);		// PB0
	int8_t center = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);	// PB5
	int8_t right = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0);	// PC0
	int8_t left = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1);		// PC1

	if (up == 1) state = up;					// 1,  0x1
	else if (down == 1) state = down << 1;		// 2,  0x2
	else if (left == 1) state = left << 2;		// 4,  0x4
	else if (right == 1) state = right << 3;	// 8,  0x8
	else if (center == 1) state = center << 4;	// 16, 0x10

	return state;
}

// Tilføj start/stop states fra 6-1_v2

void handleJoystick() {
	static int8_t current_state = 0;
	int8_t next_state = readJoystickState();

//	printf("next state = %d\n", next_state);

	if (next_state != current_state) {
		switch(next_state) {
		default:
			setLed('d');
			break;

		case 0x1:
			setLed('r');
			printf("UP (red)\n");
			break;

		case 0x2:
			setLed('g');
			printf("DOWN (green)\n");
			break;

		case 0x4:
			setLed('b');
			printf("LEFT (blue)\n");
			break;

		case 0x8:
			setLed('c');
			printf("RIGHT (cyan)\n");
			break;

		case 0x10:
			setLed('m');
			printf("CENTER (magenta)\n");
			break;
		}

		current_state = next_state;
	}
}
