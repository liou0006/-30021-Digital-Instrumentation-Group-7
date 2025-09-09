/*
 * joystick.c
 *
 *  Created on: Sep 6, 2025
 *      Author: liou-
 */

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course


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


int8_t readJoystick(){

	int8_t bit = 0;

	int8_t Up = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4);
	int8_t Center= GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5);
	int8_t Down = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
	int8_t Left = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1);
	int8_t Right = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0);

	if (Up == 1){
		bit = Up;
	} else if (Down == 1){
		bit = Down << 1;
	} else if (Left == 1){
		bit = Left << 2;
	} else if (Right == 1){
		bit = Right << 3;
	} else if (Center == 1){
		bit = Center << 4;
	}

	return bit;
}

void handleJoystick(){

	static int8_t currentState = 0;  // remember last state

	int8_t newState = readJoystick();
//	if ((newState != 0) && (currentState != newState)){

    if ((currentState == 0) && (newState != 0)) {
		if (newState == 0x1){
			printf("Up");
		} else if (newState == 0x2){
			printf("Down");
		} else if (newState == 0x4){
			printf("Left");
		} else if (newState == 0x8){
			printf("Right");
		} else if (newState == 0x10){
			printf("Center");
		}
	}

	currentState = newState;

}

