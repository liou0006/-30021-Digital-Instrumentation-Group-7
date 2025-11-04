/*
 * timer.c
 *
 *  Created on: Sep 9, 2025
 *      Author: emma
 */

#include "timer.h"

timestruct_t timeData;

void initTimer() {
	// Set reload and prescale value for 1/100 s (100 Hz) interrupt
	RCC->APB1ENR |= RCC_APB1Periph_TIM2;		// enable clock line to timer 2
	TIM2->CR1 = 0x0001;		// configure timer 2
	TIM2->ARR = 0x03FF;		// set reload value
	TIM2->PSC = 0x0270;		// set prescale value
	TIM2->DIER |= 0x0001;	// enable timer 2 interrupts

	uint8_t priority = 2;

	NVIC_SetPriority(TIM2_IRQn, priority);		// set interrupt priority interrupts
	NVIC_EnableIRQ(TIM2_IRQn);					// enable interrupt

	TIM2_IRQHandler();
}

void TIM2_IRQHandler(void) {
	timeData.hundredths++;

	if (timeData.hundredths == 100) {
		timeData.hundredths = 0;		// reset hundredths counter
		timeData.seconds++;				// increment seconds counter
	} else if (timeData.seconds == 60) {
		timeData.seconds = 0;
		timeData.minutes++;
	} else if (timeData.minutes == 60) {
		timeData.minutes = 0;
		timeData.hours++;
	} else if (timeData.hours == 24) {
		// Reset all
		timeData.hundredths = 0;
		timeData.seconds = 0;
		timeData.minutes = 0;
		timeData.hours = 0;
	}

	// Clear interrupt bit
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
