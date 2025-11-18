#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "lcd.h"

#include "lsm9ds1.h"
#include "spiSlave.h"

// === DMA + SPI Configuration ===
#define DATA_BUFFER_SIZE 6 // 3 axes * 2 bytes/axis
// This is our main buffer for receiving data via DMA
volatile uint8_t g_rxBuffer[DATA_BUFFER_SIZE];
// This flag is set to 1 by an interrupt when data is ready
volatile int g_data_ready = 0;
volatile int8_t CSflag = 0;
// === End Configuration ===


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
	DMA_DeInit(DMA1_Channel2); // CHANGED from Channel 4

	// 3. Configure DMA1 Channel 2 (SPI3_RX)
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&(SPI3->DR)); // CHANGED from SPI2
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
	DMA_Init(DMA1_Channel2, &DMA_InitStruct); // CHANGED from Channel 4

	// 4. Enable DMA Transfer Complete Interrupt
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE); // CHANGED from Channel 4

	// 5. Enable the DMA interrupt in the NVIC
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel2_IRQn; // CHANGED from Channel 4
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	// 6. Link SPI3's RXNE event to the DMA
	SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Rx, ENABLE); // CHANGED from SPI2
}

// === INTERRUPT HANDLERS ===

/**
 * @brief  This interrupt fires when the CS pin (PB12) goes LOW.
 */
void EXTI15_10_IRQHandler(void) {
	// Check if the interrupt was on line 12
	if(EXTI_GetITStatus(EXTI_Line12) != RESET) {

		CSflag = 1;

		// This is the trigger: The master wants to talk.
		// We must re-configure and enable the DMA for the *next* 6 bytes.

		// 1. Disable DMA channel to re-configure it
		DMA_Cmd(DMA1_Channel2, DISABLE); // CHANGED from Channel 4

		// 2. Set the number of bytes to receive
		DMA_SetCurrDataCounter(DMA1_Channel2, DATA_BUFFER_SIZE); // CHANGED from Channel 4

		// 3. Set the memory address by writing directly to the register
		DMA1_Channel2->CMAR = (uint32_t)g_rxBuffer; // CHANGED from Channel 4

		// 4. Enable DMA. It will now wait for SPI3_RXNE events.
		DMA_Cmd(DMA1_Channel2, ENABLE); // CHANGED from Channel 4

		// 5. Clear the EXTI pending bit
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
}

/**
 * @brief  This interrupt fires when DMA1_Channel2 has finished
 * transferring all 6 bytes.
 */
// **** RENAMED THE FUNCTION ****
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

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	initSlaveSPI();
	init_DMA();
	init_CS_Interrupt();

	// These are probably not needed on the slave, but I'll leave them.
	// initAG();
	// initMag();

	int sizeofRxArray = 6, sizeOfDataArray = sizeofRxArray / 2;
	// We use g_rxBuffer (global) for DMA
	int16_t dataArray[sizeOfDataArray]; // Local array for processed data

	printf("Started \n");

	while(1) {

		if(CSflag == 1){
			printf("CS interrupt activated\n");
			CSflag = 0;
		}



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

		// The CPU is free to do other things here while waiting
		// for the next DMA transfer.
		// E.g., read local sensors, update an LCD, etc.

	}

}

