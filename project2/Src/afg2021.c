#include "afg2021.h"

void GPIO_set_AF1_PA0() {
	// Initializes the selected pins in AF mode

	// 1. Enable TIM clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

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

	// 2.5 Configure TimeBaseInit
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	TIM_TimeBaseStructInit(&TIM_InitStruct);

	// --- From Table 23 in the datasheet APB1 clock frequency is 36MHz
	uint32_t timer_clock = 36000000;          // TIM2 clock (2 × APB1 = 72 MHz)
	uint32_t target_freq = 1000000;           // 1 MHz
	uint16_t prescaler = (timer_clock / target_freq) - 1;  // = 71
	uint32_t period = 0xFFFFFFFF; 							// max

	TIM_InitStruct.TIM_Prescaler = prescaler;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = period;
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
	TIM_SetAutoreload(TIM2, 0xFFFFFFFF);

	// 3. Configure TIM_ICInitStrct
	TIM_ICInitTypeDef TIM_ICInitStruct;
//	TIM_ICInit(TIM2, &TIM_ICInitStruct);
	TIM_ICStructInit(&TIM_ICInitStruct);

	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICFilter = 0;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;

	// 5.
	TIM_PWMIConfig(TIM2, &TIM_ICInitStruct);

	// Configure trigger/slave mode
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);
	TIM_SelectInputTrigger(TIM2, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);

	// 6. Enable NVIC
	uint8_t priority = 2;
	NVIC_SetPriority(TIM2_IRQn, priority);		// set interrupt priority interrupts
	NVIC_EnableIRQ(TIM2_IRQn);					// enable interrupt

	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 | TIM_IT_CC2);

	// 7. Enable corresponding interrupt to read captured value
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

	// 8. Enable TIM counter
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void) {
    // Check for capture on Channel 1
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {
        // Read captured value for period (rising-to-rising)
        uint16_t capture1 = TIM_GetCapture1(TIM2);

        // Optionally, read capture2 for pulse width
        uint16_t capture2 = TIM_GetCapture2(TIM2);

        // Clear the interrupt flag
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

        // --- Compute results ---
        // Timer tick period = 1 µs (1 MHz counter)
        float period_us = (float)capture1;
        float high_us   = (float)capture2;

        float freq = 1e6f / period_us;        // in Hz
        float duty = (high_us / period_us) * 100.0f; // in %

        printf("%f\n",freq);

        // You could store these in global/static variables:
//        float measuredFreq = freq;
//        float measuredDuty = duty;
    }
}
