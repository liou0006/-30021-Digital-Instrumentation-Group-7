#include "afg2021.h"

volatile uint32_t capture1 = 0;
volatile uint32_t capture2 = 0;

volatile float freq = 0;
volatile float duty = 0;
volatile float high_us = 0;
volatile uint32_t tim2_hits = 0;
volatile uint8_t flag = 0;

void TimeBaseInit(){
	// 1. Enable TIM clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// 2.5 Configure TimeBaseInit
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	TIM_TimeBaseStructInit(&TIM_InitStruct);

	// --- From Table 23 in the datasheet APB1 clock frequency is 36MHz
	uint32_t timer_clock = 72000000;          // TIM2 clock (2 × APB1 = 72 MHz) something about we
	uint32_t target_freq = 1000000;           // 1 MHz
	uint16_t prescaler = (timer_clock / target_freq) - 1;  // = 71
	uint32_t period = 0xFFFFFFFF; 	 //TIM2 is 32-bit - look at page 550 in RM

	TIM_InitStruct.TIM_Prescaler = prescaler;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = period;
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
}

void TimeICInit(){

	GPIO_set_AF1_PA0();
	TimeBaseInit();

	// 3. Configure TIM_ICInitStrct
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICStructInit(&TIM_ICInitStruct);

	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICFilter = 0;

	// 5. Configure PWM input mode
	TIM_PWMIConfig(TIM2, &TIM_ICInitStruct);

	// Enable both CC1 and CC2 captures
	TIM_CCxCmd(TIM2, TIM_Channel_1, TIM_CCx_Enable);
	TIM_CCxCmd(TIM2, TIM_Channel_2, TIM_CCx_Enable);

	// Select input trigger as TI1FP1
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);

	// Reset mode
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);		// timer resets on every rising edge
	TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);

	TIM_SetCounter(TIM2, 0);  // reset the counter to start correctly


	// 6. Enable NVIC
	uint8_t priority = 2;
	NVIC_SetPriority(TIM2_IRQn, priority);		// set interrupt priority interrupts
	NVIC_EnableIRQ(TIM2_IRQn);					// enable interrupt

	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

	// 7. Enable corresponding interrupt to read captured value
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

	// 8. Enable TIM counter
	TIM_Cmd(TIM2, ENABLE);

}

void GPIO_set_AF1_PA0() {
	// Initializes the selected pins in AF mode

	// 2. Configuring GPIO PA0
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);    // enable clock

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_1);
}

void TIM2_IRQHandler(void) {
	// Check for capture on Channel 1
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

		// Clear the interrupt flag
		// Read captured value for period (rising-to-rising)
		 capture1 = TIM_GetCapture1(TIM2);	// read capture1 for period
		 capture2 = TIM_GetCapture2(TIM2);	// read capture2 for pulse width

		// --- Compute results ---
		// Timer tick period = 1 µs (1 MHz counter)
		float period_us = (float)capture1;
		high_us   = (float)capture2;

		if (period_us > 0) {
			freq = 1e6f / period_us;
			duty = (high_us / period_us) * 100.0f;
		}

		flag = 1;
	}
}
