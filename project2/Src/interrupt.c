/*
 * interrupt.c
 *
 *  Created on: Sep 9, 2025
 *      Author: liou-
 */

#include "interrupt.h"

//Mostly template for now
//Keep in mind to switch the GPIOx port and EXTI_PinSourcex and the EXTI_Linex

void iniEXTIA4(){
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

// IRQHandler for when the joystick is being pushed upwards
void EXTI4_IRQHandler(void){
    if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
        // --- Your handler code here ---
        setLED('r');
        EXTI_ClearITPendingBit(EXTI_Line4);
    }

}
