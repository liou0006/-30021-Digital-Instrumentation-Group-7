#include "interrupts.h"

void initInterrupts() {
	// Enable clocks for GPIO and SYSCFG (system config)
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);		// enable GPIOB clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);	// enable SYSCFG clock

	// PB0 is initialized as input in joystick.c there GPIOB clock is initialized

	// Connect GPIO pin to EXTI line (here setting port B pin 0 to the IRQ)
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);

	// Define and set setting for EXTI
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;				// use EXTI0
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				// enable it
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		// we want interrupts
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	// trigger on rising edge
	EXTI_Init(&EXTI_InitStructure);

	// Configure NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;		// EXTI0 interrupt channel
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI9_5_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {			// check if interrupt was from EXTI0
		printf("Right : %d | Up : %d | Center : %d | Left : %d | Down : %d\n",
				GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0), GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4),
				GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5), GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1),
				GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0));

		// Toggle LED
		GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_RESET);
//		GPIO_ToggleBits(GPIOC, GPIO_Pin_7);

		// Clear the interrupt pending bit
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
}
