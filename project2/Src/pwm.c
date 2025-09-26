#include "pwm.h"

void timer16_pwm_init() {
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	TIM_OCInitTypeDef TIM_OCStruct;

	// From Table 23 in the datasheet APB2 clock frequency is 72MHz
	uint32_t timer_clock = 72000000;
	uint32_t PWM_freq = 10000; // 10 kHz
	uint32_t PWM_steps = 256;
	uint32_t fCK = PWM_freq * PWM_steps;
	uint16_t prescaler = (timer_clock / fCK) - 1;
	uint16_t period = PWM_steps - 1;

	// 1.
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

	// 2.
	GPIO_set_AF1_PA12();
	GPIO_set_AF1_PA6();

	// 3.
	TIM_TimeBaseStructInit(&TIM_BaseStruct);
	TIM_BaseStruct.TIM_Prescaler = prescaler;
	TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseStruct.TIM_Period = period;
	TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM16, &TIM_BaseStruct);

	// 4.
	TIM_OCStructInit(&TIM_OCStruct);
	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;

	// 5.
	TIM_OC1Init(TIM16, &TIM_OCStruct);

	// 6.
	TIM_OC1PreloadConfig(TIM16, TIM_OCPreload_Enable);

	// 7.
	TIM_CtrlPWMOutputs(TIM16, ENABLE);
	TIM_Cmd(TIM16, ENABLE);
}

void GPIO_set_AF1_PA12() {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	// enable clock

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);					// initialize GPIO struct
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			// set as AF
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;				// set so the configuration is on pin 12
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// set speed to 50 MHz

	GPIO_Init(GPIOA, &GPIO_InitStruct); 				// setup of GPIO with the settings chosen
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_1);
}

// Second channel GPIO setup (needed in Exercise 3.3)
void GPIO_set_AF1_PA6() {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	// enable clock

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);					// initialize GPIO struct
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			// set as AF
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;				// set so the configuration is on pin 6
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// set speed to 50 MHz

	GPIO_Init(GPIOA, &GPIO_InitStruct); 				// setup of GPIO with the settings chosen
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
}


void setDutyCycle(int dutyCycle){
	TIM_SetCompare1(TIM16, dutyCycle);
}

void regulate_voltage(float desired_voltage) {
	float measured_voltage;
	int duty = 128;			// 50%
	float kp = 20.0;		// proportional gain, can be tuned

	while (1) {
		measured_voltage = ADC_measure_VREF();
		float error = desired_voltage - measured_voltage;
		duty += (int)(kp * error);

		// Clamp duty cycle
		if (duty < 0) duty = 0;
		if (duty > 255) duty = 255;

		setDutyCycle(duty);
	}
}


/*
#define VREFINT_CAL *((uint16_t*) ((uint32_t) 0x1FFFF7BA)) //calibrated at 3.3V@ 30

void regulate_voltage(float V_desired) {
	uint8_t duty = 128;		// start at 50 %
	float kp = 20.0;		// proportional gain, can be tuned

	uint16_t VREF = ADC_measure_VREF();
	float V_DDA = 3.3 * VREFINT_CAL / VREF;
	float V_measured = (V_DDA * ) / 4095;

	while (1) {
		V_measured = ADC_measure_VREF();
		float error = V_desired - V_measured;
		duty += (int)(kp * error);

		// Clamp duty cycle 0-255
		if (duty < 0) duty = 0;
		if (duty > 255) duty = 255;

		timer16_PA12_set_duty(duty);
		delay_ms(10);		// allow settling, can be tuned
	}

	uint16_t pa0 = ADC_measure_PA(1);

	// when PWM has a duty cycle of 50 it switches between
	// PA0: 0 and 4080
	// V = 0V and 2.83V

	float Vch0 = (V_DDA *pa0)/ (4095);

	float V_desired = 1.0;
	float error = V_desired - Vch0;

	float kp = 20; //increase or decrease for faster response
	float cp = kp * error; // Proportional controller / might need to make a integral if we desire absolute 1V
	int newDutyCycle = dutyCycle + cp;

	// maybe set the boundary conditions to 5 for the lower and 250 for the upper
	//so the duty cycle doesn't die out suddenly

	if(newDutyCycle <= 0){
		newDutyCycle = 0;
	} else if (newDutyCycle >= 255){
		newDutyCycle = 255;
	}

	printf("V_meas=%.2f, Error=%.2f, Duty=%d\n", Vch0, error, dutyCycle);



	setDutyCycle(newDutyCycle);
	dutyCycle = newDutyCycle;


}
*/

/*
void update_servos() {
	// Get the two potentiometer positions
	uint16_t pot1 = ADC_measure_PA(1);				// potentiometer 1 position
	uint16_t pot2 = ADC_measure_PA(2);				// potentiometer 2 position

	// Map potentiometer ADC values (0-4095) to servo PWM duty cycle (0-255)
	int duty1 = (pot1 * 255) / 4095;
	int duty2 = (pot2 * 255) / 4095;

	// Boundary conditions (ensures valid duty cycle value is given to servo)
	if (duty1 < 0) duty1 = 0;
	if (duty1 > 255) duty1 = 255;
	if (duty2 < 0) duty2 = 0;
	if (duty2 > 255) duty2 = 255;

	// Set duty cycle for each servo
	setDutyCycle(duty1);
	setDutyCycle(duty2);
}
*/

