#include "joystick.h"

void initJoystick(){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructAll;

	GPIO_StructInit(&GPIO_InitStructAll);
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructAll.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructAll);

	GPIO_StructInit(&GPIO_InitStructAll);
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructAll.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructAll);

	GPIO_StructInit(&GPIO_InitStructAll);
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructAll.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOC, &GPIO_InitStructAll);
}


uint8_t readJoystickState() {
	// Initialize joystick state variable
	uint8_t state = 0;

	// Read joystick pins and assign corresponding state
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)) state = UP;			// PA4, 0x1
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)) state = DOWN;	// PB0, 0x2
	else if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)) state = LEFT;	// PC1, 0x4
	else if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)) state = RIGHT;	// PC0, 0x8
	else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)) state = CENTER;	// PC1, 0x10

	return bit;
}

// Not used so commented out for now
//void handleJoystick() {
//	static int8_t current_state = 0;
//	int8_t next_state = readJoystickState();
//
////	printf("next state = %d\n", next_state);
//
//	if (next_state != current_state) {
//		switch(next_state) {
//		default:
//			setLed('d');
//			break;
//
//		case 0x1:
//			setLed('r');
//			printf("UP (red)\n");
//			break;
//
//		case 0x2:
//			setLed('g');
//			printf("DOWN (green)\n");
//			break;
//
//		case 0x4:
//			setLed('b');
//			printf("LEFT (blue)\n");
//			break;
//
//		case 0x8:
//			setLed('c');
//			printf("RIGHT (cyan)\n");
//			break;
//
//		case 0x10:
//			setLed('m');
//			printf("CENTER (magenta)\n");
//			break;
//		}
//
//		current_state = next_state;
//	}
//}
