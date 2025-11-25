#include "ADC.h"

void ADC_setup_PA(void) {

    RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div8);


    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);


    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1;
    gpio.GPIO_Mode = GPIO_Mode_AN;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gpio);

    ADC_InitTypeDef adc;
    ADC_StructInit(&adc);  // reset to defaults

    adc.ADC_ContinuousConvMode    = DISABLE;                     // single conversion
    adc.ADC_Resolution            = ADC_Resolution_12b;          // 12-bit
    adc.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0; // SW trigger
    adc.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
    adc.ADC_DataAlign             = ADC_DataAlign_Right;         // right alignment
    adc.ADC_OverrunMode           = DISABLE;                     // simplest: overwrite off
    adc.ADC_AutoInjMode           = DISABLE;                     // no injected group
    adc.ADC_NbrOfRegChannel       = 1;                           // 1 channel only

    ADC_Init(ADC1, &adc);

    ADC_Cmd(ADC1, ENABLE);
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY)) {}
	ADC_VoltageRegulatorCmd(ADC1,ENABLE);
	for(uint32_t i = 0; i<10000;i++);


    ADC_Cmd(ADC1, DISABLE);
    while (ADC_GetDisableCmdStatus(ADC1)) {}
    ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1)) {}
    for(uint32_t i = 0; i<100;i++);

    ADC_Cmd(ADC1, ENABLE);
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY)) {}
}

// --- Measure PA0 (ch=1) or PA1 (ch=2) once ---
uint16_t ADC_measure_PA(uint8_t ch) {
    uint32_t channel = (ch == 1) ? ADC_Channel_1 : ADC_Channel_2;      // PA0->ch1, PA1->ch2
    ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_1Cycles5); // rank=1, Ts=1.5
    ADC_StartConversion(ADC1);                                         // start
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0) {}              // wait EOC
    return ADC_GetConversionValue(ADC1);                               // 0..4095
}
