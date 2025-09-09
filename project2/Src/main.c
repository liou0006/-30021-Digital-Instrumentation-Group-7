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

void setLed(int8_t state) {
    // First turn everything off
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);
    GPIO_ResetBits(GPIOC, GPIO_Pin_7);
    GPIO_ResetBits(GPIOA, GPIO_Pin_9);

    // Turn on based on joystick state
    if (state & (1 << 0)) { // Up → Red
        GPIO_SetBits(GPIOB, GPIO_Pin_4);
    }
    if (state & (1 << 1)) { // Down → Green
        GPIO_SetBits(GPIOC, GPIO_Pin_7);
    }
    if (state & (1 << 2)) { // Left → Blue
        GPIO_SetBits(GPIOA, GPIO_Pin_9);
    }
    if (state & (1 << 3)) { // Left → Blue
        GPIO_SetBits(GPIOA, GPIO_Pin_9);
        GPIO_SetBits(GPIOC, GPIO_Pin_7);
    }
    if (state & (1 << 4)) { // Left → Blue
        GPIO_SetBits(GPIOB, GPIO_Pin_4);
        GPIO_SetBits(GPIOC, GPIO_Pin_7);
    }
}

int main(void)
{
	uart_init( 9600 ); // Initialize USB serial at 9600 baud
	initJoystick();
	initLed();



	while(1){
		handleJoystick();
		


	}
}
