#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"

void ini_EXTI_NVIC(void){
	// EXTI setup
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource4); // sets port B pin 5 to the IRQ
	// define and set setting for EXTI
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line4; // line 5 see [RM p. 215]
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
	// setup NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitTypeDef NVIC_InitStructure;
	//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI9_5_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line5) != RESET){
		printf("Right : %d | Up : %d | Center : %d | Left : %d | Down : %d\n",GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0),GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4),GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5),GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1),GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0));
		GPIO_WriteBit(GPIOC,GPIO_Pin_7,Bit_RESET);
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
}


void configEXTIA4(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource4);
	EXTI_InitTypeDef EXTI_InitStructureUp;
	EXTI_InitStructureUp.EXTI_Line = EXTI_Line4; // line 5 see [RM p. 215]
	EXTI_InitStructureUp.EXTI_LineCmd = ENABLE;
	EXTI_InitStructureUp.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructureUp.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructureUp);

	// setup NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	NVIC_InitTypeDef NVIC_InitStructureUp;
	NVIC_InitStructureUp.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructureUp.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructureUp.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructureUp.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructureUp);

}

void EXTI4_IRQHandler(void){
    if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
        // --- Your handler code here ---
        setLED('r');
        // Clear the interrupt flag
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
	//Do some important stuff here!...
//	NVIC_SetPriority(EXTI4_IRQn, priority); // Set interrupt priority
//	NVIC_EnableIRQ(EXTI4_IRQn); // Enable interrupt
//	EXTI_ClearITPendingBit(EXTI_Line4); //Clear the interrupt pending bit

}


void EXTI0_IRQHandler(void){
	//Do some important stuff here!...

	EXTI_ClearITPendingBit(EXTI_Line0); //Clear the interrupt pending bit

}

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	initJoystick();
	initLED();
//	ini_EXTI_NVIC();
	configEXTIA4();


	int8_t prev_state = readJoystick();
	int8_t current_state;

	while(1) {
		current_state = readJoystick();
		if (prev_state != current_state) {
			if (current_state == 1) {
//				setLED('r');
				printf("UP (red)\n");
			} else if (current_state == 2) {
//				setLED('g');
				printf("DOWN (green)\n");
			} else if (current_state == 4) {
//				setLED('b');
				printf("LEFT (blue)\n");
			} else if (current_state == 8) {
//				setLED('c');
				printf("RIGHT (cyan)\n");
			} else if (current_state == 16) {
//				setLED('m');
				printf("CENTER (magenta)\n");
			} else {
				setLED('d');
			}
			prev_state = current_state;
		}
	}
}
