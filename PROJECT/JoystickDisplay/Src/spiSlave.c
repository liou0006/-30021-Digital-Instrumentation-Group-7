/*
 * spiSlave.c
 *
 *  Created on: Oct 28, 2025
 *      Author: liou-
 */
#include "spiSlave.h"

// === DMA + SPI Configuration ===
#define DATA_BUFFER_SIZE 6 // 3 axes * 2 bytes/axis
// This is our main buffer for receiving data via DMA
volatile uint8_t g_rxBuffer[DATA_BUFFER_SIZE];
// This flag is set to 1 by an interrupt when data is ready
volatile int g_data_ready = 0;
volatile int8_t CSflag = 0;
// === End Configuration ===


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

void iniPB12(){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitTypeDef gpio_cs;
	gpio_cs.GPIO_Pin = GPIO_Pin_12;
	gpio_cs.GPIO_Mode = GPIO_Mode_IN;
	gpio_cs.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &gpio_cs);
}


/*

void main(){
	//	init_DMA();
	//	init_CS_Interrupt();
	//
	//	// These are probably not needed on the slave, but I'll leave them.
	////	initAG();
	//	// initMag();
	//
	//	int sizeofRxArray = 6, sizeOfDataArray = sizeofRxArray / 2;
	//	// We use g_rxBuffer (global) for DMA
	//	int16_t dataArray[sizeOfDataArray]; // Local array for processed data
	//
	 *
	 * while(){
	 *

		// Check if the DMA interrupt has set the "data ready" flag
		if (g_data_ready == 1) {

			// --- 1. Process the received data ---
			// Re-combine the 8-bit bytes from g_rxBuffer into
			// 16-bit integers in dataArray.

			// Disable interrupts while copying, just to be safe
			__disable_irq();

			for (int i = 0; i < sizeOfDataArray; i++) {
				// (High Byte * 256) + Low Byte
				// Or, more efficiently: (High Byte << 8) | Low Byte
				uint8_t highByte = g_rxBuffer[i * 2];
				uint8_t lowByte  = g_rxBuffer[i * 2 + 1];

				dataArray[i] = (int16_t)((highByte << 8) | lowByte);
			}

			// Clear the flag and re-enable interrupts
			g_data_ready = 0;
			__enable_irq();

			// --- 2. Use the data ---
			// Now you have the 16-bit gyro values
			printf("Received Gyro data: X=%d, Y=%d, Z=%d\n",
					dataArray[0], dataArray[1], dataArray[2]);
		}
	}

}

		 */





/**
 * @brief  Initializes the GPIO pin for the "Chip Select" interrupt.
 * This pin is connected to the Master's CS/NSS pin.
 */
void init_CS_Interrupt(void) {
	// --- ASSUMPTIONS ---
	// We connect the Master's PB3 (CS) pin to the Slave's PB12 pin.
	// We will use EXTI line 12.

	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	// 1. Enable Clocks
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); // Clock for PB12
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); // Clock for SYSCFG (EXTI)

	// 2. Configure Pin (PB12) as Input
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; // Input mode
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // Pull-up is safest
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	// 3. Connect EXTI Line 12 to PB12
	//    This tells the system that EXTI line 12 should listen to Port B.
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);

	// 4. Configure EXTI Line 12
	EXTI_InitStruct.EXTI_Line = EXTI_Line12;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // Trigger when CS goes LOW
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	// 5. Configure the Interrupt in the NVIC (Nested Vector Interrupt Controller)
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn; // IRQ for lines 10-15
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}


/**
 * @brief  Initializes DMA1 Channel 4 for SPI1 RX
 */
void init_DMA(void) {
	// --- ALL CHANGES FOR SPI3 ARE IN THIS FUNCTION ---

	DMA_InitTypeDef DMA_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	// 1. Enable DMA1 Clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	// 2. De-initialize DMA channel 2 for a clean setup
	DMA_DeInit(DMA1_Channel2); //

	// 3. Configure DMA1 Channel 2 (SPI3_RX)
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&(SPI3->DR)); //
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)g_rxBuffer; // To our buffer
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC; // Data from peripheral
	DMA_InitStruct.DMA_BufferSize = DATA_BUFFER_SIZE; // 6 bytes
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // Don't inc SPI address
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable; // Do inc buffer address
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 8-bit
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // 8-bit
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal; // Not circular
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel2, &DMA_InitStruct); //

	// 4. Enable DMA Transfer Complete Interrupt
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE); //

	// 5. Enable the DMA interrupt in the NVIC
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	// 6. Link SPI3's RXNE event to the DMA
	SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Rx, ENABLE);
}

void EXTI15_10_IRQHandler(void) {
	// Check if the interrupt was on line 12
	if(EXTI_GetITStatus(EXTI_Line12) != RESET) {

		CSflag = 1;

		// This is the trigger: The master wants to talk.

		// Disable DMA channel to re-configure it
		DMA_Cmd(DMA1_Channel2, DISABLE);

		// Set the number of bytes to receive
		DMA_SetCurrDataCounter(DMA1_Channel2, DATA_BUFFER_SIZE);

		// Set the memory address by writing directly to the register
		DMA1_Channel2->CMAR = (uint32_t)g_rxBuffer;

		// Enable DMA
		DMA_Cmd(DMA1_Channel2, ENABLE);
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
}

void DMA1_Channel2_IRQHandler(void) {
	// Check if the "Transfer Complete" flag is set
	if (DMA_GetITStatus(DMA1_IT_TC2)) { // CHANGED from TC4

		// 1. Disable the DMA channel
		DMA_Cmd(DMA1_Channel2, DISABLE); // CHANGED from Channel 4

		// 2. Set our global flag so the main loop can process the data
		g_data_ready = 1;

		// 3. Clear the DMA interrupt pending bit
		DMA_ClearITPendingBit(DMA1_IT_GL2); // CHANGED from GL4
	}
}

