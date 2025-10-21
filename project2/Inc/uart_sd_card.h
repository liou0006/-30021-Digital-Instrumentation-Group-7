/*
 * uart_sd_card.h
 *
 *  Created on: Oct 8, 2025
 *      Author: markmalloy
 */


#ifndef UART_SD_CARD_H
#define UART_SD_CARD_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Initialize USART1 (PA9=TX, PA10=RX) at 57600 8N1 (no flow control). */
void serial1_init_57600(void);

/* Send / receive bytes */
void serial1_write_byte(uint8_t b);
void serial1_write(const uint8_t *data, size_t len);
int  serial1_read_byte_timeout(uint8_t *b, uint32_t timeout_ms);

/* OpenLog helpers (UART command interface) */
bool openlog_enter_command_mode(void);
/* Create/overwrite "data.txt" and write the provided string (null-terminated). */
bool openlog_write_data_txt(const char *text);
/* Create empty "data.txt" (if it exists it will be overwritten). */
bool openlog_create_data_txt(void);

/* Tiny delay helpers (busy-wait, approximate) */
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

#endif /* UART_SD_CARD_H */
