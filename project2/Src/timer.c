/*
 * timer.c
 *
 *  Created on: Sep 9, 2025
 *      Author: emma
 */

#include "timer.h"

timestruct_t timeData;

void initTimer() {
	// -------------------------------------------------
	/*
	// Timer
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_TimeBaseStructInit(&TIM_InitStructure);
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = 6138;
	TIM_InitStructure.TIM_Prescaler = 100;
	TIM_TimeBaseInit(TIM2, &TIM_InitStructure);

	// NVIC for timer
	NVIC_InitTypeDef NVIC_InitStructure;					// code added (not in example)
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	 */
	// -------------------------------------------------

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
//
//void TIM2_IRQHandler(void) {
//
//	// ... Do whatever you want here, but make sure it doesn’t take too much time
//	timeData.hundredths++;
//
//	if (timeData.hundredths == 100) {
//		timeData.hundredths = 0;		// reset hundredths counter
//		timeData.seconds++;				// increment seconds counter
//	} else if (timeData.seconds == 60) {
//		timeData.seconds = 0;
//		timeData.minutes++;
//	} else if (timeData.minutes == 60) {
//		timeData.minutes = 0;
//		timeData.hours++;
//	} else if (timeData.hours == 24) {
//		// Reset all
//		timeData.hundredths = 0;
//		timeData.seconds = 0;
//		timeData.minutes = 0;
//		timeData.hours = 0;
//	}
//
//	// Clear interrupt bit
//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//}
//
//void stopWatch(uint8_t *flag) {
//	uint8_t state = readJoystick();				// read joystick state
//
//	// Joystick state determines action
//	if (state == 2) {			// DOWN = Stop clock and set time to 00:00
//		NVIC_DisableIRQ(TIM2_IRQn);				// disable interrupt
//		*flag = 0;								// reset flag
//		timeData.hundredths = 0;
//		timeData.seconds = 0;
//		timeData.minutes = 0;
//		timeData.hours = 0;
//	} else if (state == 4) {	// LEFT = Split time 1
//		printf("\033[4;20H");	// moves cursor to row 4 column 20
//		printTime();
//	} else if (state == 8) {	// RIGHT = Split time 2
//		printf("\033[5;20H");	// moves cursor to row 5 column 20
//		printTime();
//	} else if (state == 16) {	// CENTER = Start/stop
//		if (*flag) {
//			NVIC_DisableIRQ(TIM2_IRQn);	// disable interrupt
//			*flag = 0;
//		} else {
//			NVIC_EnableIRQ(TIM2_IRQn);	// enable interrupt
//			*flag = 1;
//		}
//	}
//}

void printTime() {
	printf("\033[0m");		// white text on black background
	printf("%d:%02d:%02d:%02d", timeData.hours, timeData.minutes, timeData.seconds, timeData.hundredths);
}

void stopWatchFunction(){

	initStopWatchWindow();
	printf("\033[3;1H"); // moves cursor to row 3 column 1
	printf("Time since start: \n");
	printf("Split time 1: \n");
	printf("Split time 2: \n");

	/* include this in main for it to work
	 * 	// --------------- TIMER ---------------
	 * 	void main(){
		stopWatchFunction();
		uint8_t stopWatchFlag = 0;

	while(1) {
		stopWatch(&stopWatchFlag);
		printf("\033[3;20H"); // moves cursor to row 3 column 20
		printTime();
	}
	}
	 */

}
