/*
 * uart_sd_card.h
 *
 *  Created on: Oct 8, 2025
 *      Author: markmalloy
 */

#ifndef UART_SD_CARD_H_
#define UART_SD_CARD_H_

#include "main.h"          // Brings in HAL types and pin defs used by the app
#include <stdint.h>

/* Extern UART handle (defined in uart_sd_card.c) */
extern UART_HandleTypeDef huart1;

/* ---------- Public OpenLog API ---------- */
/**
 * @brief Write a text buffer to a file on OpenLog.
 * @param filename  Null-terminated filename (e.g., "log.txt")
 * @param text      Null-terminated text buffer to write
 * @return HAL status
 */
HAL_StatusTypeDef openlog_write_text(const char *filename, const char *text);

/**
 * @brief Read a text file from OpenLog into a buffer.
 * @param filename  Null-terminated filename
 * @param buf       Destination buffer
 * @param buflen    Size of destination buffer in bytes
 * @return Number of bytes copied into buf (not including terminating NUL), or 0 on failure
 */
int openlog_read_text(const char *filename, char *buf, int buflen);

/* ---------- System hooks supplied by the application ---------- */
void SystemClock_Config(void);
void Error_Handler(void);

#endif /* UART_SD_CARD_H_ */
