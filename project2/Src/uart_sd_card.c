/*
 * uart_sd_card.c  (OpenLog streaming mode, HAL-free)
 *
 * USART1 mapped to:
 *   TX  = PC4 (AF7)
 *   RX  = PC5 (AF7)
 *   RTS = (unused)  // PA12 not connected; hardware RTS disabled
 *
 * Baud: 57600, 8N1
 *
 * OpenLog notes:
 * - In NewLog mode (mode=0), OpenLog prints "12<" at boot and logs any bytes you send
 *   to LOG#####.TXT automatically (no filename required).
 * - In Sequential mode (mode=1), OpenLog appends to SEQLOG.txt (also prints "12<").
 * - Send Ctrl+Z (0x1A) three times to enter command mode (prompt '>') if you need file commands.
 *
 * Make sure OpenLog's config.txt baud matches 57600.
 */

#include "stm32f30x.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* ===== Public API (match your header) ===== */
void serial1_init_57600(void);                 /* initializes USART1 on PC4/PC5 @ 57600 */
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);
void serial1_write_byte(uint8_t b);
void serial1_write(const uint8_t *data, size_t len);
int  serial1_read_byte_timeout(uint8_t *b, uint32_t timeout_ms);

/* OpenLog (streaming) */
bool openlog_wait_newlog_prompt(uint32_t timeout_ms);   /* wait for '<' after power-up (optional) */
void openlog_log_write(const char *text);               /* write string as-is */
void openlog_log_writeline(const char *text);           /* write string + CRLF */

/* Optional: command-mode helper (Ctrl+Z x3) */
bool openlog_enter_command_mode(void);

/* ===== Tiny timebase (SysTick-based) ===== */
static volatile uint32_t systick_ms = 0;
void SysTick_Handler(void) { systick_ms++; }

static void timebase_init(void) {
    /* Use SystemCoreClock if available (system_stm32f30x.c), else fall back to 8 MHz */
    uint32_t hclk = (SystemCoreClock ? SystemCoreClock : 8000000u);
    SysTick->LOAD  = (hclk / 1000u) - 1u;              /* 1 kHz tick */
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
    /* very crude micro delay using CPU cycles */
    uint32_t core = (SystemCoreClock ? SystemCoreClock : 8000000u);
    uint32_t cycles = (core / 1000000u) * us / 5u;
    while (cycles--) { __NOP(); }
}

/* ===== Clock helpers (runtime APB2 frequency) ===== */
static uint32_t apb2_freq_hz(void) {
    /* HCLK is SystemCoreClock; APB2 prescaler in RCC->CFGR PPRE2[2:0] (bits 13:11) */
    uint32_t hclk  = (SystemCoreClock ? SystemCoreClock : 8000000u);
    uint32_t ppre2 = (RCC->CFGR >> 11) & 0x7u;

    uint32_t div;
    switch (ppre2) {                     /* RM0316: 0xx:/1, 100:/2, 101:/4, 110:/8, 111:/16 */
        case 0: case 1: case 2: div = 1;  break;
        case 4: div = 2;  break;
        case 5: div = 4;  break;
        case 6: div = 8;  break;
        case 7: div = 16; break;
        default: div = 1;  break;
    }
    return hclk / div;
}

/* ===== USART1 @ 57600, 8N1, TX/RX on PC4/PC5 ===== */
void serial1_init_57600(void) {
    /* Enable GPIOC (PC4/PC5) and USART1 clocks */
    RCC->AHBENR  |= RCC_AHBENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    /* --- PC4 (TX), PC5 (RX) to AF7 (USART1) --- */
    /* MODER: 10b = Alternate function */
    GPIOC->MODER   &= ~((3u << (4u*2u)) | (3u << (5u*2u)));
    GPIOC->MODER   |=  ((2u << (4u*2u)) | (2u << (5u*2u)));
    /* OTYPER: push-pull */
    GPIOC->OTYPER  &= ~((1u << 4) | (1u << 5));
    /* PUPDR: no pull (adjust to PU if needed) */
    GPIOC->PUPDR   &= ~((3u << (4u*2u)) | (3u << (5u*2u)));
    /* OSPEEDR: high speed */
    GPIOC->OSPEEDR |=  ((3u << (4u*2u)) | (3u << (5u*2u)));
    /* AFRL for pins 0..7 on port C: AF7 on PC4/PC5 */
    GPIOC->AFR[0]  &= ~((0xFu << (4u*4u)) | (0xFu << (5u*4u)));
    GPIOC->AFR[0]  |=  ((7u  << (4u*4u)) | (7u  << (5u*4u)));

    /* Disable USART before (re)config */
    USART1->CR1 = 0u;
    USART1->CR2 = 0u;
    USART1->CR3 = 0u;                     /* RTS/CTS disabled (no flow control) */

    /* 8N1, oversampling by 16 */
    USART1->CR1 &= ~USART_CR1_M;          /* 8-bit */
    USART1->CR1 &= ~USART_CR1_PCE;        /* no parity */
    USART1->CR2 &= ~USART_CR2_STOP;       /* 1 stop */

    /* Baud = 57600, OVER8 = 0 (oversampling by 16), using runtime APB2 */
    const uint32_t baud = 57600u;
    uint32_t pclk = apb2_freq_hz();
    uint32_t br_x16 = (pclk * 16u + (baud / 2u)) / baud;   /* rounded */
    USART1->BRR = ((br_x16 / 16u) << 4) | (br_x16 & 0xFu);

    /* Enable TX, RX, then USART */
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
    USART1->CR1 |= USART_CR1_UE;

    /* Small settle */
    for (volatile uint32_t i=0; i<1000u; ++i) { __NOP(); }

    /* Start SysTick 1 kHz if not already running */
    timebase_init();
}

/* ===== Basic TX/RX helpers ===== */
void serial1_write_byte(uint8_t b) {
    while ((USART1->ISR & USART_ISR_TXE) == 0u) { /* wait */ }
    USART1->TDR = b;
}

void serial1_write(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        serial1_write_byte(data[i]);
    }
}

int serial1_read_byte_timeout(uint8_t *b, uint32_t timeout_ms) {
    uint32_t start = systick_ms;
    while ((uint32_t)(systick_ms - start) < timeout_ms) {
        if (USART1->ISR & USART_ISR_RXNE) {
            *b = (uint8_t)USART1->RDR;
            return 1;
        }
    }
    return 0; /* timeout */
}

/* ===== OpenLog Streaming ===== */

/* Wait for "<" character from the "12<" banner after power-up.
   Many firmwares send this; if we miss it due to timing, we still proceed. */
bool openlog_wait_newlog_prompt(uint32_t timeout_ms) {
    uint8_t ch;
    uint32_t start = systick_ms;
    while ((uint32_t)(systick_ms - start) < timeout_ms) {
        if (serial1_read_byte_timeout(&ch, 10u)) {
            if (ch == '<') return true; /* ready marker seen */
        }
    }
    return true; /* proceed even if not seen */
}

void openlog_log_write(const char *text) {
    if (!text) return;
    serial1_write((const uint8_t*)text, strlen(text));
}

void openlog_log_writeline(const char *text) {
    if (text) serial1_write((const uint8_t*)text, strlen(text));
    serial1_write((const uint8_t*)"\r\n", 2);
}

/* ===== Optional: Command Mode (Ctrl+Z x3) ===== */
bool openlog_enter_command_mode(void) {
    uint8_t ctrlz = 0x1A; /* ASCII 26 */
    serial1_write(&ctrlz, 1); delay_ms(15);
    serial1_write(&ctrlz, 1); delay_ms(15);
    serial1_write(&ctrlz, 1); delay_ms(15);

    /* Wait briefly for '>' prompt */
    uint8_t ch;
    uint32_t start = systick_ms;
    while ((uint32_t)(systick_ms - start) < 1500u) {
        if (serial1_read_byte_timeout(&ch, 10u) && ch == '>') return true;
    }
    return false;
}
