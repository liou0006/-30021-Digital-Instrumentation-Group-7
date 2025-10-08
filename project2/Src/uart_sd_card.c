#include "main.h"           // includes stm32f3xx_hal.h and peripheral handles
#include "uart_sd_card.h"
#include <stdio.h>
#include <string.h>

/*
 * PINS TO USE FOR UART COMMUNICATION (preliminary):
 * PA9 TX, PA10 RX, PA12 RTS
 */
/* ---------- OpenLog command functions ---------- */

/* ---------- OpenLog command functions ---------- */

// Enter OpenLog command mode (based on config.txt: baud=115200, escape=26, esc#=3)
static HAL_StatusTypeDef openlog_enter_cmd(void) {
  // Per OpenLog documentation: 3-second quiet period before and after escape sequence
  HAL_Delay(3000);

  uint8_t esc = 26; // ASCII 26 = CTRL+Z
  for (int i = 0; i < 3; i++) {
    HAL_UART_Transmit(&huart1, &esc, 1, 100);
    HAL_Delay(100); // short gap between escape chars
  }

  HAL_Delay(3000); // quiet time after escape sequence

  // Wait for OpenLog to confirm command mode entry
  uint8_t tmp[64];
  uart_read_until(tmp, sizeof(tmp), "OK", 1500);
  uart_read_until(tmp, sizeof(tmp), ">", 1500);
  return HAL_OK;
}

/**
 * @brief Write a text file to OpenLog.
 */
HAL_StatusTypeDef openlog_write_text(const char *filename, const char *text) {
  if (!filename || !text) return HAL_ERROR;

  openlog_enter_cmd();

  char cmd[64];
  snprintf(cmd, sizeof(cmd), "write %s\r\n", filename);
  HAL_StatusTypeDef st = uart_send_str(cmd, 200);
  if (st != HAL_OK) return st;

  // Send file contents
  st = uart_send((const uint8_t*)text, (uint16_t)strlen(text), 2000);
  if (st != HAL_OK) return st;

  // Send escape character (CTRL+Z) to signal end of file
  uint8_t ctrlz = 0x1A;
  st = uart_send(&ctrlz, 1, 100);
  if (st != HAL_OK) return st;

  // Give OpenLog time to flush data to SD
  HAL_Delay(1500);

  uint8_t resp[128];
  uart_read_until(resp, sizeof(resp), ">", 3000);
  return HAL_OK;
}

/**
 * @brief Read a text file from OpenLog into a buffer.
 * @return Number of bytes read, or 0 on failure.
 */
int openlog_read_text(const char *filename, char *buf, int buflen) {
  if (!filename || !buf || buflen <= 0) return 0;

  openlog_enter_cmd();

  char cmd[64];
  snprintf(cmd, sizeof(cmd), "read %s\r\n", filename);
  if (uart_send_str(cmd, 200) != HAL_OK) return 0;

  int total = 0;
  const char *EOF_TOKEN = "EOF\r\n";

  // Read file contents until EOF or buffer full
  while (total < buflen - 1) {
    uint8_t b;
    if (HAL_UART_Receive(&huart1, &b, 1, 1500) != HAL_OK)
      break;

    buf[total++] = (char)b;
    buf[total] = 0;

    if (total >= 5 && memcmp(&buf[total - 5], EOF_TOKEN, 5) == 0) {
      total -= 5;
      buf[total] = 0;
      break;
    }
  }

  // Flush any remaining prompt characters ('>') from buffer
  uint8_t sink[64];
  uart_read_until(sink, sizeof(sink), ">", 1000);

  // Give OpenLog a brief rest before next operation
  HAL_Delay(1000);

  return total;
}

