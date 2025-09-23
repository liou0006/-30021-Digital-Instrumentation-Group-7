#include "pwm.h"

void pwm_init() {
	// 1. Enable clocks for TIM16
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	// enable clOCInitStructk

	// 2. Configuring GPIO PA16
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);				// initialize GPIO struct
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;		// set as AF
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;		// set as Push-Pull
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;			// set so the configuration is on pin 12
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	// set speed to 2 MHz

	GPIO_Init(GPIOA, &GPIO_InitStruct); 				// setup of GPIO with the settings chosen
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_1);

	// 3. Timer
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_TimeBaseStructInit(&TIM_InitStructure);

	// Assume APB2 Timer ClOCInitStructk is 72MHz (check your board for actual value)
	uint32_t timer_clock = 72000000;
	uint32_t PWM_freq = 10000; // 10 kHz
	uint32_t PWM_steps = 256;
	uint32_t fCK = PWM_freq * PWM_steps;
	uint16_t prescaler = (timer_clock / fCK) - 1;
	uint16_t period = PWM_steps - 1;

	// Used for debugging
//	printf("fCK = %l\n", fCK);
//	printf("prescaler = %d\n", prescaler);
//	printf("period = %d\n", period);

	TIM_InitStructure.TIM_Prescaler = prescaler;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = period;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM16, &TIM_InitStructure);

	// 4. Configure PWM
	TIM_OCInitTypeDef OCInitStruct;
	TIM_OCStructInit(&OCInitStruct);

	OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;

	// 5.
	TIM_OC1Init(TIM16, &OCInitStruct);

	// 6.
	TIM_OC1PreloadConfig(TIM16, TIM_OCPreload_Enable);

	// 7.
	TIM_CtrlPWMOutputs(TIM16, ENABLE);
	TIM_Cmd(TIM16, ENABLE);
}
