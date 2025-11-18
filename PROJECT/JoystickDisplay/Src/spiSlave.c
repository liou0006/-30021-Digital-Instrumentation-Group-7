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


	//	 Configure SPI3
	//	 FIX 6: Changed all SPI2-> to SPI3->
	SPI3->CR1 = 0; // Clear CR1 Register

	// Set Slave Mode (MSTR=0)
	// Set CPOL=1, CPHA=1 (Mode 3, to match master)
	SPI3->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA;

	// Set Software CS management (SSM=1)
	SPI3->CR1 |= SPI_CR1_SSM;

	// --- THIS IS THE FIX ---
	// Force the internal slave select to HIGH.
	// This is required when SSM=1 on a slave.
	SPI3->CR1 |= SPI_CR1_SSI;
	// --- END FIX ---

	SPI3->CR2 = 0; // Clear CR2 Register
	// Set Data Size to 8-bit (0111)
	SPI3->CR2 |= SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0;
	// Set RXFIFO threshold to 8-bit (for RXNE event)
	SPI3->CR2 |= SPI_CR2_FRXTH;

	// Enable SPI3
	SPI3->CR1 |= SPI_CR1_SPE;

	//	SPI3->CR1 = 0;

	//		// 3. Set all CR1 settings at once
	//		SPI3->CR1 |= SPI_CR1_MSTR; // Set CPOL = 1 (bit 1)
	//		SPI3->CR1 |= SPI_CR1_CPHA; // Set CPHA = 1 (bit 0)
	//		SPI3->CR1 |= SPI_CR1_SSM;   // Enable Software Slave Management (bit 9, 0x0200)
	//		SPI3->CR1 |= SPI_CR1_SSI;   // <-- THE FIX: Force slave to be "selected" (bit 8, 0x0100)
	//		// Note: Slave mode (MSTR=0) is the default (0), so we don't need to set it.
	//		// Note: MSB first is the default (0).
	//
	//		// 4. Clear the entire CR2 register
	//		SPI3->CR2 = 0;
	//
	//		// 5. Set all CR2 settings at once
	//		SPI3->CR2 |= SPI_DataSize_8b; // Set 8-bit data (0x0700)
	//		SPI3->CR2 |= SPI_CR2_FRXTH;  // Set RXFIFO threshold to 8-bit (0x1000)
	//
	//		// 6. Disable I2S mode
	//		SPI3->I2SCFGR = 0;
	//
	//		// 7. Finally, enable the SPI3 peripheral
	//		SPI3->CR1 |= SPI_CR1_SPE; // Enable SPI (bit 6, 0x0040)


	//	// Configure SPI2
	//	SPI3->CR1 &= 0x3040; // Clear CR1 Register
	//	SPI3->CR1 |= 0x0000; // Configure direction (0x0000 - 2 Lines Full Duplex, 0x0400 - 2 Lines RX Only, 0x8000 - 1 Line RX, 0xC000 - 1 Line TX)
	//	SPI3->CR1 |= 0x0000; // Configure mode (0x0000 - Slave, 0x0104 - Master)
	//	SPI3->CR1 |= 0x0002; // Configure clock polarity (0x0000 - Low, 0x0002 - High)
	//	SPI3->CR1 |= 0x0001; // Configure clock phase (0x0000 - 1 Edge, 0x0001 - 2 Edge)
	//	SPI3->CR1 |= 0x0200; // Configure chip select (0x0000 - Hardware based, 0x0200 - Software based)
	//	SPI3->CR1 |= 0x0020; // Set Baud Rate Prescaler (0x0000 - 2, 0x0008 - 4, 0x0018 - 8, 0x0020 - 16, 0x0028 - 32, 0x0028 - 64, 0x0030 - 128, 0x0038 - 128)
	//	SPI3->CR1 |= 0x0000; // Set Bit Order (0x0000 - MSB First, 0x0080 - LSB First)
	//	SPI3->CR2 &= ~0x0F00; // Clear CR2 Register
	//	SPI3->CR2 |= 0x0700; // Set Number of Bits (0x0300 - 4, 0x0400 - 5, 0x0500 - 6, ...);
	//	SPI3->I2SCFGR &= ~0x0800; // Disable I2S
	//	SPI3->CRCPR = 7; // Set CRC polynomial order
	//	SPI3->CR2 &= ~0x1000;
	//	SPI3->CR2 |= 0x1000; // Configure RXFIFO return at (0x0000 - Half-full (16 bits), 0x1000 - Quarter-full (8 bits))
	//	SPI3->CR1 |= 0x0040; // Enable SPI2


}
