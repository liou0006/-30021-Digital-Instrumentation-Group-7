#ifndef UART_SD_CARD_H
#define UART_SD_CARD_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================
 * UART mapping (STM32F302R8, HAL-free)
 *   USART1_TX  -> PC4  (AF7)
 *   USART1_RX  -> PC5  (AF7)
 *   RTS        -> not used (PA12 unused; flow control disabled)
 * Baud: 57600, 8N1
 * ============================================================
 */

/* Initialize USART1 (PC4/PC5) @ 57600 8N1.
   Baud is computed from the actual APB2 clock at runtime. */
void serial1_init_57600(void);

/* Tiny delays (SysTick-based) */
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

/* Byte/stream I/O over USART1 */
void serial1_write_byte(uint8_t b);
void serial1_write(const uint8_t *data, size_t len);
int  serial1_read_byte_timeout(uint8_t *b, uint32_t timeout_ms);

/* OpenLog streaming helpers
   - NewLog (mode=0): auto LOG#####.TXT
   - Sequential (mode=1): appends to SEQLOG.txt
*/
bool openlog_wait_newlog_prompt(uint32_t timeout_ms);  /* waits for '<' after power-up (optional) */
void openlog_log_write(const char *text);              /* writes text as-is */
void openlog_log_writeline(const char *text);          /* writes text + CRLF */

/* Optional: enter command mode (Ctrl+Z x3) to use text commands. */
bool openlog_enter_command_mode(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* UART_SD_CARD_H */
