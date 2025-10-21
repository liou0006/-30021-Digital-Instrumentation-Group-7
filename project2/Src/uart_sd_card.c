/*
 * uart_sd_card.c
 *
 * HAL-free UART driver + minimal OpenLog helpers for STM32F302R8.
 * TARGET UART: USART2 on PA2 (TX / D1) and PA3 (RX / D0) at 57600 8N1.
 *
 * Wiring (as per your photo):
 *   OpenLog TX  -> Nucleo RX / D0 (PA3 / USART2_RX)
 *   OpenLog RX  -> Nucleo TX / D1 (PA2 / USART2_TX)
 *
 * Notes:
 * - This uses a tiny SysTick timebase for delays/timeouts (no HAL).
 * - The OpenLog "command mode" uses Ctrl+Z. We create/overwrite data.txt
 *   and write a string via the "write" command, then terminate with Ctrl+Z.
 * - Ensure OpenLog's baud in config.txt is set to 57600 to match this side.
 */

#include "stm32f30x.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Public prototypes (usually in uart_sd_card.h) */
void serial1_init_57600(void); /* actually initializes USART2 at 57600 */
void serial1_write_byte(uint8_t b);
void serial1_write(const uint8_t *data, size_t len);
int  serial1_read_byte_timeout(uint8_t *b, uint32_t timeout_ms);

bool openlog_enter_command_mode(void);
bool openlog_create_data_txt(void);
bool openlog_write_data_txt(const char *text);

void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

/* ===== Configuration ===== */
/* APB1 clock that feeds USART2. If you run default HSI=8 MHz without PLL, leave 8 MHz.
 * If you later set your clocks so APB1 differs, update UART_PCLK accordingly. */
#ifndef UART_PCLK
#define UART_PCLK 8000000u
#endif

#define OPENLOG_BAUD 57600u

/* ===== Tiny timebase (SysTick-based, HAL-free) ===== */
static volatile uint32_t systick_ms = 0;

void SysTick_Handler(void) {
    systick_ms++;
}

static void timebase_init(void) {
    /* Configure SysTick to 1 kHz using core clock. Assume SystemCoreClock is set. */
    uint32_t hclk = SystemCoreClock;
    if (hclk == 0u) hclk = 8000000u;
    SysTick->LOAD  = (hclk / 1000u) - 1u;
    SysTick->VAL   = 0u;
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_TICKINT_Msk   |
                     SysTick_CTRL_ENABLE_Msk;
}

void delay_ms(uint32_t ms) {
    uint32_t start = systick_ms;
    while ((uint32_t)(systick_ms - start) < ms) { __NOP(); }
}

void delay_us(uint32_t us) {
    /* Very crude loop based on CPU frequency; good enough for short waits */
    uint32_t core = (SystemCoreClock ? SystemCoreClock : 8000000u);
    uint32_t cycles = (core / 1000000u) * us / 5u;
    while (cycles--) { __NOP(); }
}

/* ===== USART2 @ 57600, 8N1, TX/RX on PA2/PA3 =====
   Functions keep the "serial1_*" naming for compatibility, but they use USART2. */
void serial1_init_57600(void) {
    /* Enable clocks: GPIOA (PA2/PA3), USART2 on APB1 */
    RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* Configure PA2 -> AF7 (USART2_TX), PA3 -> AF7 (USART2_RX) */
    /* MODER: 10b = Alternate function */
    /* MODER2/3: 10b = Alternate function */
    GPIOA->MODER &= ~((3u << (2u*2u)) | (3u << (3u*2u)));
    GPIOA->MODER |=  ((2u << (2u*2u)) | (2u << (3u*2u)));

    /* OTYPER: Push-pull */
    GPIOA->OTYPER &= ~((1u << 2) | (1u << 3));

    /* PUPDR: no pull or pull-up as you prefer; OpenLog has its own drive.
       We'll leave floating (00) which works fine. */
    GPIOA->PUPDR &= ~((3u << (2u*2u)) | (3u << (3u*2u)));

    /* AFRL for pins 0..7: select AF7 on PA2/PA3 */
    GPIOA->AFR[0] &= ~((0xF << (2 * 4)) | (0xF << (3 * 4)));
    GPIOA->AFR[0] |=  ((7   << (2 * 4)) | (7   << (3 * 4)));

    /* Disable USART before config */
    USART2->CR1 = 0u;
    USART2->CR2 = 0u;
    USART2->CR3 = 0u;

    /* Word length, parity, stop bits: 8N1, oversampling by 16 */
    USART2->CR1 &= ~USART_CR1_M;     /* 8-bit */
    USART2->CR1 &= ~USART_CR1_PCE;   /* no parity */
    USART2->CR2 &= ~USART_CR2_STOP;  /* 1 stop bit */
    USART2->CR3  = 0u;               /* no flow control */

    /* Baud rate: BRR for oversampling by 16:
       USARTDIV = UART_PCLK / (16 * baud) -> BRR = mantissa<<4 | fraction */
    uint32_t usartdiv_x16 = (UART_PCLK + (OPENLOG_BAUD/2)) / OPENLOG_BAUD; /* = USARTDIV * 16 */
    uint32_t mantissa = usartdiv_x16 / 16u;
    uint32_t fraction = usartdiv_x16 - (mantissa * 16u);
    USART2->BRR = (mantissa << 4) | (fraction & 0xFu);

    /* Enable TX, RX and USART */
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;
    USART2->CR1 |= USART_CR1_UE;

    /* Small settle delay */
    for (volatile uint32_t i = 0; i < 1000u; ++i) { __NOP(); }

    /* Start SysTick timebase if not already running */
    timebase_init();
}

void serial1_write_byte(uint8_t b) {
    while ((USART2->ISR & USART_ISR_TXE) == 0u) { /* wait */ }
    USART2->TDR = b;
}

void serial1_write(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; ++i) serial1_write_byte(data[i]);
}

int serial1_read_byte_timeout(uint8_t *b, uint32_t timeout_ms) {
    uint32_t start = systick_ms;
    while ((uint32_t)(systick_ms - start) < timeout_ms) {
        if (USART2->ISR & USART_ISR_RXNE) {
            *b = (uint8_t)USART2->RDR;
            return 1;
        }
    }
    return 0; /* timeout */
}

/* ===== OpenLog helpers ===== */

/* Enter command mode: send Ctrl+Z three times, expect '>' prompt (some fw may vary) */
bool openlog_enter_command_mode(void) {
    uint8_t ctrlz = 0x1A; /* Ctrl+Z */
    serial1_write(&ctrlz, 1);
    delay_ms(20);
    serial1_write(&ctrlz, 1);
    delay_ms(20);
    serial1_write(&ctrlz, 1);
    delay_ms(20);

    /* Wait for '>' prompt */
    uint8_t ch;
    uint32_t start = systick_ms;
    while ((uint32_t)(systick_ms - start) < 1500u) {
        if (serial1_read_byte_timeout(&ch, 10u)) {
            if (ch == '>') return true;
        }
    }

    /* Some firmware may not immediately show '>'—try forcing with '?' */
    const char q = '?';
    serial1_write((const uint8_t *)&q, 1);
    serial1_write((const uint8_t *)"\r\n", 2);
    start = systick_ms;
    while ((uint32_t)(systick_ms - start) < 500u) {
        if (serial1_read_byte_timeout(&ch, 10u) && ch == '>') return true;
    }
    return false;
}

/* Internal helpers */
static void send_str(const char *s) {
    while (*s) serial1_write_byte((uint8_t)*s++);
}

/* Drain incoming bytes until '>' or timeout */
static void drain_to_prompt(uint32_t timeout_ms) {
    uint8_t ch;
    uint32_t start = systick_ms;
    while ((uint32_t)(systick_ms - start) < timeout_ms) {
        if (serial1_read_byte_timeout(&ch, 10u)) {
            if (ch == '>') break;
        }
    }
}

/* Create/overwrite an empty data.txt using 'write data.txt' then immediate Ctrl+Z */
bool openlog_create_data_txt(void) {
    if (!openlog_enter_command_mode()) return false;
    send_str("write data.txt\r\n");
    uint8_t ctrlz = 0x1A;
    serial1_write(&ctrlz, 1); /* end file immediately => empty file created */
    drain_to_prompt(1500u);
    return true;
}

/* Overwrite data.txt with provided text; Ctrl+Z to finish and save */
bool openlog_write_data_txt(const char *text) {
    if (text == NULL) return false;
    if (!openlog_enter_command_mode()) return false;

    send_str("write LOG00134.TXT\r\n");
    send_str(text);
    uint8_t ctrlz = 0x1A;
    serial1_write(&ctrlz, 1);

    drain_to_prompt(3000u);
    return true;
}
