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

	GPIOC->AFR[1] &= ~(0x0000000F << ((10-8) * 4)); // Pin 10
	GPIOC->AFR[1] &= ~(0x0000000F << ((11-8) * 4)); // Pin 11
	GPIOC->AFR[1] &= ~(0x0000000F << ((12-8) * 4)); // Pin 12

	// Set AF6 for PC10 (SCK), PC11 (MISO), PC12 (MOSI)
	GPIOC->AFR[1] |=  (0x00000006 << ((10-8) * 4)); // Set AF6 for PC10
	GPIOC->AFR[1] |=  (0x00000006 << ((11-8) * 4)); // Set AF6 for PC11
	GPIOC->AFR[1] |=  (0x00000006 << ((12-8) * 4)); // Set AF6 for PC12

	// Configure pins PC10, PC11, PC12 for Alternate Function
	// FIX 5: Changed all GPIOB-> to GPIOC->
	GPIOC->OSPEEDR &= ~(0x00000003 << (10 * 2) | 0x00000003 << (11 * 2) | 0x00000003 << (12 * 2));    // Clear speed register
	GPIOC->OSPEEDR |=  (0x00000001 << (10 * 2) | 0x00000001 << (11 * 2) | 0x00000001 << (12 * 2));    // set speed register (10 MHz)
	GPIOC->OTYPER  &= ~(0x0001     << (10)     | 0x0001 	<< (11) 	| 0x0001     << (12));        // Clear output type register
	GPIOC->OTYPER  |=  (0x0000     << (10)     | 0x0000 	<< (11) 	| 0x0000     << (12));        // Set output type register (Push pull)
	GPIOC->MODER   &= ~(0x00000003 << (10 * 2) | 0x00000003 << (11 * 2) | 0x00000003 << (12 * 2));    // Clear mode register
	GPIOC->MODER   |=  (0x00000002 << (10 * 2) | 0x00000002 << (11 * 2) | 0x00000002 << (12 * 2));    // Set mode register (Alternate Function)
	GPIOC->PUPDR   &= ~(0x00000003 << (10 * 2) | 0x00000003 << (11 * 2) | 0x00000003 << (12 * 2));    // Clear push/pull register
	GPIOC->PUPDR   |=  (0x00000000 << (10 * 2) | 0x00000000 << (11 * 2) | 0x00000000 << (12 * 2));    // Set push/pull register (No pull)



	SPI3->CR1 = 0; // Clear CR1 Register
	SPI3->CR1 |= 0x0000; // Configure direction (0x0000 - 2 Lines Full Duplex, 0x0400 - 2 Lines RX Only, 0x8000 - 1 Line RX, 0xC000 - 1 Line TX)
	SPI3->CR1 |= 0x0000; // Configure mode (0x0000 - Slave, 0x0104 - Master)
	SPI3->CR1 |= 0x0002; // Configure clock polarity (0x0000 - Low, 0x0002 - High)
	SPI3->CR1 |= 0x0001; // Configure clock phase (0x0000 - 1 Edge, 0x0001 - 2 Edge)
	SPI3->CR1 |= 0x0200; // Configure chip select (0x0000 - Hardware based, 0x0200 - Software based)
	SPI3->CR1 |= 0x0018; // Set Baud Rate Prescaler (0x0000 - 2, 0x0008 - 4, 0x0018 - 8, 0x0020 - 16, 0x0028 - 32, 0x0028 - 64, 0x0030 - 128, 0x0038 - 128)
	SPI3->CR1 |= 0x0000; // Set Bit Order (0x0000 - MSB First, 0x0080 - LSB First)
	SPI3->CR2 &= ~0x0F00; // Clear CR2 Register
	SPI3->CR2 |= 0x0700; // Set Number of Bits (0x0300 - 4, 0x0400 - 5, 0x0500 - 6, ...);
	SPI3->I2SCFGR &= ~0x0800; // Disable I2S
	SPI3->CRCPR = 7; // Set CRC polynomial order
	SPI3->CR2 &= ~0x1000;
	SPI3->CR2 |= 0x1000; // Configure RXFIFO return at (0x0000 - Half-full (16 bits), 0x1000 - Quarter-full (8 bits))
	SPI3->CR1 |= 0x0040; // Enable SPI2


//	initCSPins();
}

void initCSPins(){
	RCC->AHBENR 	|= 0x00040000;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);	// Port B
	GPIO_InitTypeDef GPIO_InitStructAll;
	GPIO_StructInit(&GPIO_InitStructAll);
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructAll.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructAll.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructAll);

	// Bit_SET = high / disabled
	// Bit_RESET = low / enabled
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_SET);

}

uint8_t spi3_transfer(uint8_t data) {
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) != SET);
	SPI_SendData8(SPI3, data);
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) != SET);
	return SPI_ReceiveData8(SPI3);
}

uint8_t readAGSPI3(uint8_t reg){
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
	spi3_transfer(0x80 | reg);           // send address
	uint8_t val = spi3_transfer(0x00);    // send dummy & read value
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY) == SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
	return val;
}
uint8_t readMSPI3(uint8_t reg){
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_RESET);
	spi3_transfer(0x80 | reg);           // send address
	uint8_t val = spi3_transfer(0x00);    // send dummy & read value
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY) == SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_SET);
	return val;
}


