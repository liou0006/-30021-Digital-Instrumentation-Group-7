#include "stm32f30x.h"
#include "ultrasonic_sensor.h"
#include <stddef.h>
#include <stdio.h>

/* === Pin mapping (fixed) ===
 * Trig -> PA8 (output)
 * Echo -> PA9 (input, EXTI9_5)
 */
#define US_TRIG_PORT      GPIOA
#define US_TRIG_PIN       8u
#define US_ECHO_PORT      GPIOA
#define US_ECHO_PIN       9u

/* --- Debug UART over ST-LINK VCP (USART2 on PA2/PA3) --- */
#define DBG_BAUD 115200u

/* === Internals === */
static volatile uint32_t us_t_rise = 0;
static volatile uint32_t us_t_fall = 0;
static volatile uint8_t  us_edge_state = 0;   /* 0: wait rising, 1: got rising */
static volatile uint8_t  us_done = 0;

/* ---- Clock helpers ---- */
static uint32_t apb1_freq_hz(void) {
    uint32_t hclk  = (SystemCoreClock ? SystemCoreClock : 8000000u);
    uint32_t ppre1 = (RCC->CFGR >> 8) & 0x7u;
    uint32_t div;
    switch (ppre1) { case 0: case 1: case 2: div = 1; break; case 4: div = 2; break;
                     case 5: div = 4; break; case 6: div = 8; break; case 7: div = 16; break;
                     default: div = 1; break; }
    return hclk / div;
}

/* TIM2-based microsecond delay (TIM2 runs @1 MHz) */
static void delay_us_on_tim2(uint32_t us) {
    uint32_t start = TIM2->CNT;
    while ((uint32_t)(TIM2->CNT - start) < us) { __NOP(); }
}

/* ---- Debug UART2 (ST-LINK VCP) ---- */
void debug_uart2_init(void) {
    RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* PA2 (TX), PA3 (RX) -> AF7 */
    GPIOA->MODER &= ~((3u << (2u*2u)) | (3u << (3u*2u)));
    GPIOA->MODER |=  ((2u << (2u*2u)) | (2u << (3u*2u)));
    GPIOA->OTYPER &= ~((1u << 2) | (1u << 3));
    GPIOA->PUPDR  &= ~((3u << (2u*2u)) | (3u << (3u*2u)));
    GPIOA->OSPEEDR |= ((3u << (2u*2u)) | (3u << (3u*2u)));
    GPIOA->AFR[0] &= ~((0xFu << (2*4)) | (0xFu << (3*4)));
    GPIOA->AFR[0] |=  ((7u  << (2*4)) | (7u  << (3*4)));

    USART2->CR1 = 0; USART2->CR2 = 0; USART2->CR3 = 0;
    /* 8N1, OVER8=0 */
    USART2->CR1 &= ~USART_CR1_M;
    USART2->CR1 &= ~USART_CR1_PCE;
    USART2->CR2 &= ~USART_CR2_STOP;

    /* Accurate BRR from runtime APB1 */
    uint32_t pclk = apb1_freq_hz();
    uint32_t br_x16 = (pclk * 16u + (DBG_BAUD/2u)) / DBG_BAUD;
    USART2->BRR = ((br_x16 / 16u) << 4) | (br_x16 & 0xFu);

    USART2->CR1 |= USART_CR1_TE;     /* TX only is enough */
    USART2->CR1 |= USART_CR1_UE;
}

void debug_uart2_write_char(char c) {
    while ((USART2->ISR & USART_ISR_TXE) == 0u) {}
    USART2->TDR = (uint8_t)c;
}
void debug_uart2_print(const char *s) {
    if (!s) return;
    while (*s) debug_uart2_write_char(*s++);
}

/* === Public API === */
void ultrasonic_init(void)
{
    /* Enable clocks: GPIOA, SYSCFG, TIM2 */
    RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    /* PA8 as push-pull output (Trig) */
    US_TRIG_PORT->MODER &= ~(3u << (US_TRIG_PIN * 2u));
    US_TRIG_PORT->MODER |=  (1u << (US_TRIG_PIN * 2u));      /* 01 = output */
    US_TRIG_PORT->OTYPER &= ~(1u << US_TRIG_PIN);
    US_TRIG_PORT->OSPEEDR |= (3u << (US_TRIG_PIN * 2u));     /* high speed */
    US_TRIG_PORT->PUPDR   &= ~(3u << (US_TRIG_PIN * 2u));    /* no pull */

    /* PA9 as input (Echo) */
    US_ECHO_PORT->MODER  &= ~(3u << (US_ECHO_PIN * 2u));     /* 00 = input */
    US_ECHO_PORT->PUPDR  &= ~(3u << (US_ECHO_PIN * 2u));     /* no pull */
    /* Route PA9 to EXTI9 */
    SYSCFG->EXTICR[2] &= ~(0xFu << 4); /* EXTI9 -> Port A */

    /* EXTI line 9 rising first */
    EXTI->IMR  |=  (1u << 9);
    EXTI->RTSR |=  (1u << 9);
    EXTI->FTSR &= ~(1u << 9);

    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_SetPriority(EXTI9_5_IRQn, 2);

    /* TIM2 1 MHz free-running counter */
    TIM2->CR1 = 0;
    TIM2->PSC = (apb1_freq_hz() / 1000000u) - 1u;  /* prescale to 1 MHz */
    TIM2->ARR = 0xFFFFFFFFu;
    TIM2->EGR = TIM_EGR_UG;
    TIM2->CR1 |= TIM_CR1_CEN;

    /* Init debug UART2 for PuTTY prints */
    debug_uart2_init();
    debug_uart2_print("Ultrasonic init complete\r\n");
}

void ultrasonic_trigger(void)
{
    /* 10 µs HIGH pulse on PA8 */
    US_TRIG_PORT->BSRR = (1u << US_TRIG_PIN);
    delay_us_on_tim2(10u);
    US_TRIG_PORT->BSRR = (1u << (US_TRIG_PIN + 16u));
}

bool ultrasonic_measure_cm(uint32_t timeout_ms, uint32_t *distance_cm)
{
    if (distance_cm == NULL) return false;

    /* Clear state and arm for rising edge */
    us_done = 0;
    us_edge_state = 0;
    EXTI->PR   = (1u << 9);          /* clear any pending flag */
    EXTI->RTSR |=  (1u << 9);
    EXTI->FTSR &= ~(1u << 9);

    /* Trigger the sensor */
    ultrasonic_trigger();

    /* Wait for completion with an overall timeout using TIM2 */
    uint32_t start_us = TIM2->CNT;
    const uint32_t timeout_us = timeout_ms * 1000u;

    while (!us_done) {
        if ((uint32_t)(TIM2->CNT - start_us) > timeout_us) {
            debug_uart2_print("Echo timeout\r\n");
            return false; /* timeout */
        }
    }

    /* Compute pulse width with wrap-safe subtraction (TIM2 is 32-bit) */
    uint32_t width_us = (uint32_t)(us_t_fall - us_t_rise);

    /* Convert to distance: distance[cm] = time[µs] / 58 */
    *distance_cm = (width_us + 29u) / 58u;  /* rounded */

    /* --- Print result to PuTTY over ST-LINK VCP --- */
    char msg[64];
    int n = snprintf(msg, sizeof(msg), "Distance: %lu cm\r\n", (unsigned long)*distance_cm);
    if (n > 0) debug_uart2_print(msg);

    return true;
}

/* === EXTI9_5 interrupt: handles PA9 (Echo) edges === */
void EXTI9_5_IRQn_Handler(void); /* compatibility */
void EXTI9_5_IRQHandler(void)
{
    if (EXTI->PR & (1u << 9)) {
        EXTI->PR = (1u << 9); /* clear pending */

        if (us_edge_state == 0) {
            /* Rising edge: start timing */
            us_t_rise = TIM2->CNT;
            us_edge_state = 1;

            /* Now wait for falling */
            EXTI->RTSR &= ~(1u << 9);
            EXTI->FTSR |=  (1u << 9);
        } else {
            /* Falling edge: stop timing */
            us_t_fall = TIM2->CNT;
            us_done = 1;

            /* Re-arm for rising again */
            us_edge_state = 0;
            EXTI->RTSR |=  (1u << 9);
            EXTI->FTSR &= ~(1u << 9);
        }
    }
}
