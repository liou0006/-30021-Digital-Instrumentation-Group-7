#include "main.h"
#include <stdio.h>
#include <string.h>

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;   // OpenLog via USART1 (PA9 TX, PA10 RX, PA12 RTS)

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

/* ---------- UART helper functions ---------- */
static HAL_StatusTypeDef uart_send_str(const char *s, uint32_t to_ms) {
  return HAL_UART_Transmit(&huart1, (uint8_t*)s, (uint16_t)strlen(s), to_ms);
}
static HAL_StatusTypeDef uart_send(const uint8_t *d, uint16_t n, uint32_t to_ms) {
  return HAL_UART_Transmit(&huart1, (uint8_t*)d, n, to_ms);
}

static uint16_t uart_read_until(uint8_t *buf, uint16_t maxlen,
                                const char *token, uint32_t timeout_ms)
{
  uint32_t start = HAL_GetTick();
  uint16_t i = 0;
  size_t tlen = token ? strlen(token) : 0;

  while ((HAL_GetTick() - start) < timeout_ms && i < (uint16_t)(maxlen - 1)) {
    uint8_t b;
    if (HAL_UART_Receive(&huart1, &b, 1, 10) == HAL_OK) {
      buf[i++] = b;
      buf[i] = 0;
      if (token && i >= tlen && memcmp(&buf[i - tlen], token, tlen) == 0) break;
      if (b == '>') break;
    }
  }
  buf[i] = 0;
  return i;
}

/* ---------- OpenLog command functions ---------- */

// Enter OpenLog command mode: "+++" with 3 s guard time (per config.txt)
static HAL_StatusTypeDef openlog_enter_cmd(void) {
  HAL_Delay(3000);                 // quiet before
  HAL_StatusTypeDef st = uart_send_str("+++", 50);
  HAL_Delay(3000);                 // quiet after
  uint8_t tmp[64];
  uart_read_until(tmp, sizeof(tmp), "OK", 1200);
  uart_read_until(tmp, sizeof(tmp), NULL, 1200); // get '>'
  return st;
}

// Write text file
HAL_StatusTypeDef openlog_write_text(const char *filename, const char *text) {
  openlog_enter_cmd();
  char cmd[64];
  //snprintf(cmd, sizeof(cmd), "write %s\r\n", filename);
  HAL_StatusTypeDef st = uart_send_str(cmd, 200);
  if (st != HAL_OK) return st;

  st = uart_send((const uint8_t*)text, (uint16_t)strlen(text), 2000);
  if (st != HAL_OK) return st;

  uint8_t ctrlz = 0x1A; // escape=26
  st = uart_send(&ctrlz, 1, 50);
  if (st != HAL_OK) return st;

  uint8_t resp[128];
  uart_read_until(resp, sizeof(resp), NULL, 3000);
  return HAL_OK;
}

// Read text file
int openlog_read_text(const char *filename, char *buf, int buflen) {
  if (buflen <= 0) return 0;
  openlog_enter_cmd();

  char cmd[64];
  //snprintf(cmd, sizeof(cmd), "read %s\r\n", filename);
  if (uart_send_str(cmd, 200) != HAL_OK) return 0;

  int total = 0;
  const char *EOF_TOKEN = "EOF\r\n";
  while (total < buflen - 1) {
    uint8_t b;
    if (HAL_UART_Receive(&huart1, &b, 1, 1500) != HAL_OK) break;
    buf[total++] = (char)b;
    buf[total] = 0;
    if (total >= 5 && memcmp(&buf[total - 5], EOF_TOKEN, 5) == 0) {
      total -= 5; buf[total] = 0; break;
    }
  }
  uint8_t sink[64];
  uart_read_until(sink, sizeof(sink), NULL, 500);
  return total;
}

/* ---------- Main ---------- */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();

  HAL_Delay(500); // allow OpenLog to boot and show '>'

  while (1)
  {
    uart_send_str("\r\nWriting to log.txt...\r\n", 100);
    HAL_StatusTypeDef st = openlog_write_text("log.txt", "Hello from STM32!\r\nLine 2...\r\n");
    if (st != HAL_OK) {
      uart_send_str("Write failed!\r\n", 100);
    } else {
      uart_send_str("Write complete.\r\n", 100);
    }

    HAL_Delay(1000);

    uart_send_str("Reading from log.txt...\r\n", 100);
    char readbuf[512];
    int n = openlog_read_text("log.txt", readbuf, sizeof(readbuf));

    if (n > 0) {
      uart_send_str("\r\nFile contents:\r\n", 100);
      uart_send((uint8_t*)readbuf, (uint16_t)n, 1000);
      uart_send_str("\r\n---- End of file ----\r\n", 100);
    } else {
      uart_send_str("Read failed or empty file.\r\n", 100);
    }

    HAL_Delay(1000);
  }
}

/* ---------- System Clock ---------- */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    Error_Handler();

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    Error_Handler();
}

/* ---------- USART1 @115200 with RTS ---------- */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;                    // per config.txt
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_RTS;       // PA12 RTS active
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
    Error_Handler();
}

/* ---------- GPIO ---------- */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /* B1 user button */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /* LD2 LED */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* --- USART1 GPIO Configuration ---
   * PA9  -> USART1_TX  (to OpenLog RXI)
   * PA10 -> USART1_RX  (from OpenLog TXO)
   * PA12 -> USART1_RTS (to Serial1_RTS)
   */
  GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* ---------- Error Handler ---------- */
void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}
