#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"


void initLed(){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructAll;

	GPIO_StructInit(&GPIO_InitStructAll);
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT;;
	GPIO_InitStructAll.GPIO_PuPd = GPIO_OType_PP;
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructAll.GPIO_Speed = GPIO_Speed_1MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructAll);

	GPIO_StructInit(&GPIO_InitStructAll);
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT;;
	GPIO_InitStructAll.GPIO_PuPd = GPIO_OType_PP;
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructAll.GPIO_Speed = GPIO_Speed_1MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructAll);

	GPIO_StructInit(&GPIO_InitStructAll);
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT;;
	GPIO_InitStructAll.GPIO_PuPd = GPIO_OType_PP;
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructAll.GPIO_Speed = GPIO_Speed_1MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructAll);

}

void setLed(Red, Green, Blue){
	if (color == "Red"){

	}
}

int main(void)
{
	uart_init( 9600 ); // Initialize USB serial at 9600 baud
	initJoystick();
	initLed();



	while(1){
//		handleJoystick();



	}
}
