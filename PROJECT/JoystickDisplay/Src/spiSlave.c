/*
 * spiSlave.c
 *
 *  Created on: Oct 28, 2025
 *      Author: liou-
 */
#include "spiSlave.h"


void initSlaveSPI(void)
{
	// Enable Clocks
//	RCC->AHBENR  |= 0x00020000 | 0x00040000;    // Enable Clock for GPIO Banks A and B
	RCC->AHBENR  |= 0x00080000;    // Enable Clock for GPIO Banks C
	RCC->APB1ENR |= 0x8000;						// Enable Clock for SPI3 (i think)

	// Connect pins to SPI2
	GPIOB->AFR[10 >> 0x03] &= ~(0x0000000F << ((10 & 0x00000007) * 4)); // Clear alternate function for PC10
	GPIOB->AFR[10 >> 0x03] |=  (0x00000005 << ((10 & 0x00000007) * 4)); // Set alternate 5 function for PC10 - SCLK

	GPIOB->AFR[11 >> 0x03] &= ~(0x0000000F << ((11 & 0x00000007) * 4)); // Clear alternate function for PC11
	GPIOB->AFR[11 >> 0x03] |=  (0x00000005 << ((11 & 0x00000007) * 4)); // Set alternate 5 function for PC11 - MISO

	GPIOB->AFR[12 >> 0x03] &= ~(0x0000000F << ((12 & 0x00000007) * 4)); // Clear alternate function for PC12
	GPIOB->AFR[12 >> 0x03] |=  (0x00000005 << ((12 & 0x00000007) * 4)); // Set alternate 5 function for PC12 - MOSI

	// Configure pins PB10 and PB12 for 10 MHz alternate function
	GPIOB->OSPEEDR &= ~(0x00000003 << (10 * 2) | 0x00000003 << (11 * 2) | 0x00000003 << (12 * 2));    // Clear speed register
	GPIOB->OSPEEDR |=  (0x00000001 << (10 * 2) | 0x00000001 << (11 * 2) | 0x00000001 << (12 * 2));    // set speed register (0x01 - 10 MHz, 0x02 - 2 MHz, 0x03 - 50 MHz)
	GPIOB->OTYPER  &= ~(0x0001     << (10)     | 0x0001 	<< (11) 	| 0x0001     << (12));        // Clear output type register
	GPIOB->OTYPER  |=  (0x0000     << (10)     | 0x0000 	<< (11) 	| 0x0000     << (12));        // Set output type register (0x00 - Push pull, 0x01 - Open drain)
	GPIOB->MODER   &= ~(0x00000003 << (10 * 2) | 0x00000003 << (11 * 2) | 0x00000003 << (12 * 2));    // Clear mode register
	GPIOB->MODER   |=  (0x00000002 << (10 * 2) | 0x00000002 << (11 * 2) | 0x00000002 << (12 * 2));    // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOB->PUPDR   &= ~(0x00000003 << (10 * 2) | 0x00000003 << (11 * 2) | 0x00000003 << (12 * 2));    // Clear push/pull register
	GPIOB->PUPDR   |=  (0x00000000 << (10 * 2) | 0x00000000 << (11 * 2) | 0x00000000 << (12 * 2));    // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)

	// Configure SPI2
	SPI2->CR1 &= 0x3040; // Clear CR1 Register
	SPI2->CR1 |= 0x0000; // Configure direction (0x0000 - 2 Lines Full Duplex, 0x0400 - 2 Lines RX Only, 0x8000 - 1 Line RX, 0xC000 - 1 Line TX)
	SPI2->CR1 |= 0x0000; // Configure mode (0x0000 - Slave, 0x0104 - Master)
	SPI2->CR1 |= 0x0002; // Configure clock polarity (0x0000 - Low, 0x0002 - High)
	SPI2->CR1 |= 0x0001; // Configure clock phase (0x0000 - 1 Edge, 0x0001 - 2 Edge)
	SPI2->CR1 |= 0x0200; // Configure chip select (0x0000 - Hardware based, 0x0200 - Software based)
	SPI2->CR1 |= 0x0018; // Set Baud Rate Prescaler (0x0000 - 2, 0x0008 - 4, 0x0018 - 8, 0x0020 - 16, 0x0028 - 32, 0x0028 - 64, 0x0030 - 128, 0x0038 - 128)
	SPI2->CR1 |= 0x0000; // Set Bit Order (0x0000 - MSB First, 0x0080 - LSB First)
	SPI2->CR2 &= ~0x0F00; // Clear CR2 Register
	SPI2->CR2 |= 0x0700; // Set Number of Bits (0x0300 - 4, 0x0400 - 5, 0x0500 - 6, ...);
	SPI2->I2SCFGR &= ~0x0800; // Disable I2S
	SPI2->CRCPR = 7; // Set CRC polynomial order
	SPI2->CR2 &= ~0x1000;
	SPI2->CR2 |= 0x1000; // Configure RXFIFO return at (0x0000 - Half-full (16 bits), 0x1000 - Quarter-full (8 bits))
	SPI2->CR1 |= 0x0040; // Enable SPI2

	// CS = D2
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);	// Port B
	GPIO_InitTypeDef GPIO_InitStructAll;
	GPIO_StructInit(&GPIO_InitStructAll);
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructAll.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructAll.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructAll);

	// Bit_SET = high / disabled
	// Bit_RESET = low / enabled
	// PB3
	GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_SET);


}
