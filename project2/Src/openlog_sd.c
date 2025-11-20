/*
 * openlog_sd.c
 *
 *  Created on: 6 Oct 2025
 *      Author: markh
 */

#include "stm32f30x_rcc.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x.h"
#include "openlog_sd.h"
#include <stdio.h>
#include "30010_io.h"
#include "system_stm32f30x.h"

void init_uart(uint32_t baud) {

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	// RTS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);

	// TX: PB9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// RX: PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// Connect pins to USART3 alternate function
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_7);  // TX
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_7);  // RX
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_7); //RTS

	// USART configuration
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART3, &USART_InitStructure);


	USART_Cmd(USART3, ENABLE);


}

void openlog_writechar(char c) {
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);// Wait until buffer empty
	USART_SendData(USART3, (uint8_t) c);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);// Wait until actual transmission done
}

void openlog_writestring(char *s) {
	while (*s) {
		openlog_writechar(*s++);
	}
}

void openlog_writeline(char *s) {
	openlog_writestring(s);
	openlog_writestring("\r\n");
}

void openlog_writebytes(const uint8_t *data, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        openlog_writechar((char)data[i]);
    }
}


/*******************/
// Openlog commands
/*******************/
void reset(void)
{
    // Pull RTS (PB14) low to reset OpenLog
    GPIO_ResetBits(GPIOB, GPIO_Pin_14);
    for (volatile int i = 0; i < 100000; i++);
    GPIO_SetBits(GPIOB, GPIO_Pin_14);

    // Wait for OpenLog to reboot
    for (volatile int i = 0; i < 4000000; i++); 
}

//Enter Command Mode (Ctrl+Z ×3)
void commandmode(void) {
    for (int i = 0; i < 3; i++) {
    	openlog_writechar(26); // ASCII 26 = Ctrl+Z
    	delay(50);
    }
}

// Append to a File
void openlog_append(const char* filename) {
    // Send "append filename\r"
    char cmd[50];
    snprintf(cmd, sizeof(cmd), "append %s\r\n", filename);
    openlog_writestring(cmd);

}

void log_data(uint8_t sample_start_nr,float data, const char* unit) {
    char line[100];
    snprintf(line, sizeof(line), "%d | %.2f %s\r\n", sample_start_nr, data, unit);
    openlog_writestring(line); // Send to OpenLog
}

void delay(uint32_t ms){
	{
	    // adjusted for 64 MHz and roughly 100 ms for 2,000,000 iterations
	    uint32_t count = ms * 20000; // 100ms ≈ 2,000,000 iterations → 1ms ≈ 20,000
	    for (volatile uint32_t i = 0; i < count; i++);
	}
}

