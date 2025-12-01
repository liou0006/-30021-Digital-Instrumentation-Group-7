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

	return state;
}
