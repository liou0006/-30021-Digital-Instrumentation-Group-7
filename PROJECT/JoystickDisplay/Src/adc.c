/*
 * ADC.c
 *
 *  Created on: Sep 16, 2025
 *      Authors: fcadi, elb (modifications and comments)
 */

#include "ADC.h"

// ============= Set up ADC for PA0 and PA1 ============
void ADC_setup_PA(void) {
	// Configure ADC clock frequency (64 MHz / 8 = 8 MHz)
	RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div8);

	// Enable ADC interface clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);

	// ============ Configure ADC GPIO pins ============
	// Enable GPIO A clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	// Configure PA0 and PA1 in analog mode
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1;	// PA0, PA1
	gpio.GPIO_Mode = GPIO_Mode_AN;				// Analog mode
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &gpio);

	// Configure ADC
	ADC_InitTypeDef adc;
	ADC_StructInit(&adc);  // reset to defaults
	adc.ADC_ContinuousConvMode    = DISABLE;                     // Single conversion
	adc.ADC_Resolution            = ADC_Resolution_12b;          // 12-bit
	adc.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0; // SW trigger
	adc.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
	adc.ADC_DataAlign             = ADC_DataAlign_Right;         // Right alignment
	adc.ADC_OverrunMode           = DISABLE;                     // Simplest: overwrite off
	adc.ADC_AutoInjMode           = DISABLE;                     // No injected group
	adc.ADC_NbrOfRegChannel       = 1;                           // 1 channel only
	ADC_Init(ADC1, &adc);

	// Activate ADC peripheral and wait for ADC to be ready
	ADC_Cmd(ADC1, ENABLE);
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY)) {}

	// ================== Calibration ==================
	// Set internal reference voltage source and wait
	ADC_VoltageRegulatorCmd(ADC1, ENABLE);
	for(uint32_t i = 0; i < 10000; i++);		// Wait 10 us before continuing

	// Perform calibration
	ADC_Cmd(ADC1, DISABLE);						// Disable ADC before calibration start
	while (ADC_GetDisableCmdStatus(ADC1)) {}	// Wait for disable of ADC
	ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1)) {}
	for(uint32_t i = 0; i < 100; i++);

	// Enable ADC and wait for ADC to be ready
	ADC_Cmd(ADC1, ENABLE);
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY)) {}
}

// ======= Measure PA0 (ch=1) or PA1 (ch=2) once =======
uint16_t ADC_measure_PA(uint8_t ch) {
	// Selecting given channel (PA0->ch1, PA1->ch2)
	uint32_t channel = (ch == 1) ? ADC_Channel_1 : ADC_Channel_2;

	// Configure ADC channel (rank=1, Ts=1.5)
	ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_1Cycles5);

	// Perform measurement
	ADC_StartConversion(ADC1);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0) {}

	// Return read ADC value (0..4095)
	return ADC_GetConversionValue(ADC1);
}

// --- More smooth values ---
uint16_t ADC_measure_PA_avg(uint8_t ch, uint8_t samples) {
	uint32_t sum = 0;
	for (uint8_t i = 0; i < samples; i++) {
		sum += ADC_measure_PA(ch);
	}
	return (uint16_t)(sum / samples);
}
