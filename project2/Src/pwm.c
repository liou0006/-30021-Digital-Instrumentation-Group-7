// pwm.c
#include "pwm.h"

#define SERVO_MIN_US 1000   // ~0°
#define SERVO_MAX_US 2000   // ~180°
// If your servos support 500..2500 µs, widen the range above.

static inline uint16_t clamp_u16(int v, int lo, int hi) {
    if (v < lo) v = lo;
    if (v > hi) v = hi;
    return (uint16_t)v;
}

/* -------------------- GPIO (AF) setup -------------------- */

static void GPIO_set_AF1_PA6(void) {
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_InitTypeDef g; GPIO_StructInit(&g);
    g.GPIO_Mode  = GPIO_Mode_AF;
    g.GPIO_Pin   = GPIO_Pin_6;              // PA6
    g.GPIO_Speed = GPIO_Speed_50MHz;
    g.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &g);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1); // TIM16_CH1
}

static void GPIO_set_AF10_PB9(void) {
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_InitTypeDef g; GPIO_StructInit(&g);
    g.GPIO_Mode  = GPIO_Mode_AF;
    g.GPIO_Pin   = GPIO_Pin_9;              // PB9
    g.GPIO_Speed = GPIO_Speed_50MHz;
    g.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB, &g);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_10); // TIM17_CH1
}

/* -------------------- TIM16: PA6 (servo 1) -------------------- */

void timer16_pwm_init(void) {
    // Timer base: 50 Hz frame with 1 µs tick
    // 72 MHz / (PSC+1) = 1 MHz  => PSC = 71
    // ARR = 20000-1 for 20 ms
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
    GPIO_set_AF1_PA6();

    TIM_TimeBaseInitTypeDef tb;
    TIM_OCInitTypeDef oc;

    TIM_TimeBaseStructInit(&tb);
    tb.TIM_Prescaler     = 71;
    tb.TIM_CounterMode   = TIM_CounterMode_Up;
    tb.TIM_Period        = 20000 - 1;
    tb.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM16, &tb);

    TIM_OCStructInit(&oc);
    oc.TIM_OCMode      = TIM_OCMode_PWM1;
    oc.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(TIM16, &oc);
    TIM_OC1PreloadConfig(TIM16, TIM_OCPreload_Enable);

    TIM_CtrlPWMOutputs(TIM16, ENABLE);
    TIM_Cmd(TIM16, ENABLE);

    // Center by default
    TIM_SetCompare1(TIM16, (SERVO_MIN_US + SERVO_MAX_US) / 2);
	printf("timer16 initialized");
}

/* -------------------- TIM17: PB9 (servo 2) -------------------- */

void timer17_pwm_init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
    GPIO_set_AF10_PB9();

    TIM_TimeBaseInitTypeDef tb;
    TIM_OCInitTypeDef oc;

    TIM_TimeBaseStructInit(&tb);
    tb.TIM_Prescaler     = 71;
    tb.TIM_CounterMode   = TIM_CounterMode_Up;
    tb.TIM_Period        = 20000 - 1;
    tb.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM17, &tb);

    TIM_OCStructInit(&oc);
    oc.TIM_OCMode      = TIM_OCMode_PWM1;
    oc.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(TIM17, &oc);
    TIM_OC1PreloadConfig(TIM17, TIM_OCPreload_Enable);

    TIM_CtrlPWMOutputs(TIM17, ENABLE);
    TIM_Cmd(TIM17, ENABLE);

    // Center by default
    TIM_SetCompare1(TIM17, (SERVO_MIN_US + SERVO_MAX_US) / 2);
	printf("timer17 initialized");
}

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
static inline uint16_t pot_to_pulse(uint16_t pot) {
    return (uint16_t)(SERVO_MIN_US +
                      ((uint32_t)(SERVO_MAX_US - SERVO_MIN_US) * pot) / 4095u);
}

/* -------------------- Exercise helper -------------------- */

void update_servos(void) {
    // Read two pots on PA1/PA2 (adjust to your wiring/ADC functions)
    uint16_t pot1 = ADC_measure_PA(1);
    uint16_t pot2 = ADC_measure_PA(2);

    setServoPulse_TIM16(pot_to_pulse(pot1));  // PA6
    setServoPulse_TIM17(pot_to_pulse(pot2));  // PB9
}
