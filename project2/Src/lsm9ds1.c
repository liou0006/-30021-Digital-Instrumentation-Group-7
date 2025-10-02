#include "lsm9ds1.h"

/* ---------- LSM9DS1 (Accel/Gyro die) registers ---------- */
#define WHO_AM_I     0x0F


void init_spi_gyro_accel(void)
{
	/* ---- Enable clocks ---- */
	RCC->AHBENR  |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN; // GPIOA/B
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;                      // SPI2

	/* ---- SPI2 pins: PB13=SCK, PB14=MISO, PB15=MOSI (AF5) ---- */
	// Connect pins to SPI2
	GPIOB->AFR[13 >> 0x03] &= ~(0x0000000F << ((13 & 0x00000007) * 4)); // Clear alternate function for PB13
	GPIOB->AFR[13 >> 0x03] |=  (0x00000005 << ((13 & 0x00000007) * 4)); // Set alternate 5 function for PB13 - SCLK / SPC
	GPIOB->AFR[14 >> 0x03] &= ~(0x0000000F << ((14 & 0x00000007) * 4)); // Clear alternate function for PB13
	GPIOB->AFR[14 >> 0x03] |=  (0x00000005 << ((14 & 0x00000007) * 4)); // Set alternate 5 function for PB13 - MISO
	GPIOB->AFR[15 >> 0x03] &= ~(0x0000000F << ((15 & 0x00000007) * 4)); // Clear alternate function for PB15
	GPIOB->AFR[15 >> 0x03] |=  (0x00000005 << ((15 & 0x00000007) * 4)); // Set alternate 5 function for PB15 - MOSI / SDA

	// Configure pins PB13 and PB15 for 10 MHz alternate function
	GPIOB->OSPEEDR &= ~(0x00000003 << (13 * 2) | 0x00000003 << (14 * 2) | 0x00000003 << (15 * 2));    // Clear speed register
	GPIOB->OSPEEDR |=  (0x00000001 << (13 * 2) | 0x00000001 << (14 * 2) | 0x00000001 << (15 * 2));    // set speed register (0x01 - 10 MHz, 0x02 - 2 MHz, 0x03 - 50 MHz)
	GPIOB->OTYPER  &= ~(0x0001     << (13)     | 0x0001     << (14)     | 0x0001     << (15));        // Clear output type register
	GPIOB->OTYPER  |=  (0x0000     << (13)     | 0x0000     << (14)     | 0x0000     << (15));        // Set output type register (0x00 - Push pull, 0x01 - Open drain)
	GPIOB->MODER   &= ~(0x00000003 << (13 * 2) | 0x00000003 << (14 * 2) | 0x00000003 << (15 * 2));    // Clear mode register
	GPIOB->MODER   |=  (0x00000002 << (13 * 2) | 0x00000002 << (14 * 2) | 0x00000002 << (15 * 2));    // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOB->PUPDR   &= ~(0x00000003 << (13 * 2) | 0x00000003 << (14 * 2) | 0x00000003 << (15 * 2));    // Clear push/pull register
	GPIOB->PUPDR   |=  (0x00000000 << (13 * 2) | 0x00000000 << (14 * 2) | 0x00000000 << (15 * 2));    // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)

	// Sets PB4 to output HIGH / disabled - CSAG
	// CSM
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);	// Port B
	GPIO_InitTypeDef GPIO_InitStructAll;
	GPIO_StructInit(&GPIO_InitStructAll);
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructAll.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructAll.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructAll);
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, 1);

	// Sets PB5 to output LOW / enabled
	// CSAG
	GPIO_StructInit(&GPIO_InitStructAll);
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructAll.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructAll.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructAll);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, 1);

	SPI2->CR1 &= 0x3040; // Clear CR1 Register
	SPI2->CR1 |= 0x0000; // Configure direction (0x0000 - 2 Lines Full Duplex, 0x0400 - 2 Lines RX Only, 0x8000 - 1 Line RX, 0xC000 - 1 Line TX)
	SPI2->CR1 |= 0x0104; // Configure mode (0x0000 - Slave, 0x0104 - Master)
	SPI2->CR1 |= 0x0002; // Configure clock polarity (0x0000 - Low, 0x0002 - High)
	SPI2->CR1 |= 0x0001; // Configure clock phase (0x0000 - 1 Edge, 0x0001 - 2 Edge)
	SPI2->CR1 |= 0x0200; // Configure chip select (0x0000 - Hardware based, 0x0200 - Software based)
	SPI2->CR1 |= 0x0008; // Set Baud Rate Prescaler (0x0000 - 2, 0x0008 - 4, 0x0018 - 8, 0x0020 - 16, 0x0028 - 32, 0x0028 - 64, 0x0030 - 128, 0x0038 - 128)
	SPI2->CR1 |= 0x0000; // Set Bit Order (0x0000 - MSB First, 0x0080 - LSB First)
	SPI2->CR2 &= ~0x0F00; // Clear CR2 Register
	SPI2->CR2 |= 0x0700; // Set Number of Bits (0x0300 - 4, 0x0400 - 5, 0x0500 - 6, ...);
	SPI2->I2SCFGR &= ~0x0800; // Disable I2S
	SPI2->CRCPR = 7; // Set CRC polynomial order
	SPI2->CR2 &= ~0x1000;
	SPI2->CR2 |= 0x1000; // Configure RXFIFO return at (0x0000 - Half-full (16 bits), 0x1000 - Quarter-full (8 bits))
	SPI2->CR1 |= 0x0040; // Enable SPI2

}



void spi_transmit(uint8_t data) {
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET) { }
	SPI_SendData8(SPI2, data);
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET) { }
}
