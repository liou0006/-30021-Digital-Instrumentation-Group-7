#include "pwm.h"

#define SERVO_MIN_US 1000   // ~0°
#define SERVO_MAX_US 2000   // ~180°

void timer16_pwm_init() {
	// 1. Enable clocks for TIM16
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

	// 2. Configuring GPIO PA12
	GPIO_set_AF1_PA12();

	// 3. Timer
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_TimeBaseStructInit(&TIM_InitStructure);

	// From Table 23 in the datasheet APB2 clock frequency is 72MHz
	uint32_t timer_clock = 72000000;
	uint32_t PWM_freq = 50; // 10 kHz
	uint32_t PWM_steps = 256;
	uint32_t fCK = PWM_freq * PWM_steps;
	uint16_t prescaler = (timer_clock / fCK) - 1;
	uint16_t period = PWM_steps - 1;

//	TIM_InitStructure.TIM_Prescaler = prescaler;
//	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_InitStructure.TIM_Period = period;
//	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseInit(TIM16, &TIM_InitStructure);

    TIM_InitStructure.TIM_Prescaler = 71;           // 72MHz / (71+1) = 1MHz
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStructure.TIM_Period = 19999;           // 20ms period (20000µs)
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM16, &TIM_InitStructure);

	// 4. Configure PWM
//	TIM_OCInitTypeDef OCInitStruct;
//	TIM_OCStructInit(&OCInitStruct);
//	OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
//	OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;

    TIM_OCInitTypeDef OCInitStruct;
    TIM_OCStructInit(&OCInitStruct);
    OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    OCInitStruct.TIM_Pulse = 1500;  // Initial position (1.5ms = 90°)

	// 5.
	TIM_OC1Init(TIM16, &OCInitStruct);

	// 6.
	TIM_OC1PreloadConfig(TIM16, TIM_OCPreload_Enable);

	// 7.
	TIM_CtrlPWMOutputs(TIM16, ENABLE);
	TIM_Cmd(TIM16, ENABLE);
}

void GPIO_set_AF1_PA12() {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	// enable clock

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);				// initialize GPIO struct
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;		// set as AF
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;			// set so the configuration is on pin 12
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	// set speed to 2 MHz

	GPIO_Init(GPIOA, &GPIO_InitStruct); 			// setup of GPIO with the settings chosen
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_1);
}

void setDutyCycle(int dutyCycle){
	TIM_SetCompare1(TIM16, dutyCycle);
}



/*
 * In main.c include pwm.h and call timer16_pwm_init()
 */


void timer2_pwm_init() {
	// 1. Enable clocks for TIM16
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// 2. Configuring GPIO PA12
	GPIO_set_AF1_PB11();

	// 3. Timer
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_TimeBaseStructInit(&TIM_InitStructure);

	// From Table 23 in the datasheet APB2 clock frequency is 72MHz
	uint32_t timer_clock = 72000000;
	uint32_t PWM_freq = 10000; // 10 kHz
	uint32_t PWM_steps = 256;
	uint32_t fCK = PWM_freq * PWM_steps;
	uint16_t prescaler = (timer_clock / fCK) - 1;
	uint16_t period = PWM_steps - 1;

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
	TIM_OC1Init(TIM2, &OCInitStruct);

	// 6.
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

	// 7.
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

void GPIO_set_AF1_PB11(void) {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);	// enable clock

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);				// initialize GPIO struct
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;		// set as AF
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;			// set so the configuration is on pin 12
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	// set speed to 2 MHz

	GPIO_Init(GPIOB, &GPIO_InitStruct); 			// setup of GPIO with the settings chosen
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_1);
}

static inline uint16_t clamp_u16(int v, int lo, int hi) {
    if (v < lo) v = lo;
    if (v > hi) v = hi;
    return (uint16_t)v;
}

/* -------------------- GPIO (AF) setup -------------------- */

//void GPIO_set_AF1_PB9(void) {
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
//
//    GPIO_InitTypeDef g; GPIO_StructInit(&g);
//    g.GPIO_Mode  = GPIO_Mode_AF;
//    g.GPIO_Pin   = GPIO_Pin_9;              // PB9
//    g.GPIO_Speed = GPIO_Speed_50MHz;
//    g.GPIO_PuPd  = GPIO_PuPd_DOWN;
//    GPIO_Init(GPIOB, &g);
//
//    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_1); // TIM17_CH1
//}

/* -------------------- TIM16: PA6 (servo 1) -------------------- */

//void timer16_pwm_initnotusedd(void) {
//    // Timer base: 50 Hz frame and 1 µs tick
//    // 72 MHz / (PSC+1) = 1 MHz  => PSC = 71
//    // ARR = 20000-1 for 20 ms
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
//    GPIO_set_AF1_PA6();
//
//    TIM_TimeBaseInitTypeDef tb;
//    TIM_OCInitTypeDef oc;
//
//    TIM_TimeBaseStructInit(&tb);
//    tb.TIM_Prescaler     = 71;
//    tb.TIM_CounterMode   = TIM_CounterMode_Up;
//    tb.TIM_Period        = 20000 - 1;
//    tb.TIM_ClockDivision = TIM_CKD_DIV1;
//    TIM_TimeBaseInit(TIM16, &tb);
//
//    TIM_OCStructInit(&oc);
//    oc.TIM_OCMode      = TIM_OCMode_PWM1;
//    oc.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OC1Init(TIM16, &oc);
//    TIM_OC1PreloadConfig(TIM16, TIM_OCPreload_Enable);
//
//    TIM_CtrlPWMOutputs(TIM16, ENABLE);
//    TIM_Cmd(TIM16, ENABLE);
//
//    TIM_SetCompare1(TIM16, (SERVO_MIN_US + SERVO_MAX_US) / 2);
//	printf("timer16 initialized");
//}

/* -------------------- TIM17: PB9 (servo 2) -------------------- */

//
//void timer17_pwm_init() {
//	// 1. Enable clocks for TIM16
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
//
//	// 2. Configuring GPIO PA12
//	GPIO_set_AF1_PB9();
//
//	// 3. Timer
//	TIM_TimeBaseInitTypeDef TIM_InitStructure;
//	TIM_TimeBaseStructInit(&TIM_InitStructure);
//
//	// From Table 23 in the datasheet APB2 clock frequency is 72MHz
//	uint32_t timer_clock = 72000000;
//	uint32_t PWM_freq = 10000; // 10 kHz
//	uint32_t PWM_steps = 256;
//	uint32_t fCK = PWM_freq * PWM_steps;
//	uint16_t prescaler = (timer_clock / fCK) - 1;
//	uint16_t period = PWM_steps - 1;
//
//	TIM_InitStructure.TIM_Prescaler = prescaler;
//	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_InitStructure.TIM_Period = period;
//	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseInit(TIM17, &TIM_InitStructure);
//
//	// 4. Configure PWM
//	TIM_OCInitTypeDef OCInitStruct;
//	TIM_OCStructInit(&OCInitStruct);
//	OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
//	OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
//
//	// 5.
//	TIM_OC1Init(TIM17, &OCInitStruct);
//
//	// 6.
//	TIM_OC1PreloadConfig(TIM17, TIM_OCPreload_Enable);
//
//	// 7.
//	TIM_CtrlPWMOutputs(TIM17, ENABLE);
//	TIM_Cmd(TIM17, ENABLE);
//}

/* -------------------- Helpers -------------------- */

// Set absolute pulse width in microseconds on each timer
void setServoPulse_TIM16(uint16_t us) {
    us = clamp_u16(us, SERVO_MIN_US, SERVO_MAX_US);
    TIM_SetCompare1(TIM16, us);
}

void setServoPulse_TIM17(uint16_t us) {
    us = clamp_u16(us, SERVO_MIN_US, SERVO_MAX_US);
    TIM_SetCompare1(TIM17, us);
}

// Map 0..4095 potentiometer reading to SERVO_MIN_US..SERVO_MAX_US
uint16_t pot_to_pulse(uint16_t pot) {
    return (uint16_t)(SERVO_MIN_US +
                      ((uint32_t)(SERVO_MAX_US - SERVO_MIN_US) * pot) / 4095);
}

/* -------------------- Exercise helper -------------------- */

void update_servos(void) {
    // Read potentiometers


    uint16_t pot1 = ADC_measure_PA(1);
    uint16_t pot2 = ADC_measure_PA(2);

    printf("pot1: %d\n",pot1);

    setServoPulse_TIM16(pot_to_pulse(pot1));  // PA6
    setServoPulse_TIM17(pot_to_pulse(pot2));  // PB9
}
