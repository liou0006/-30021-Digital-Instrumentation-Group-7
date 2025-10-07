#include "lsm9ds1.h"

/* ---------- LSM9DS1 (Accel/Gyro die) registers ---------- */
#define WHO_AM_I     0x0F

void init_spi_gyro_accel(void)
{
	/* ---- Enable clocks ---- */
	RCC->AHBENR  |= 0x00020000 | 0x00040000; // GPIOA/B
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;// SPI2

	/* ---- SPI2 pins: PB13=SCK, PB14=MISO, PB15=MOSI (AF5) ---- */
	// Connect pins to SPI2
	GPIOB->AFR[13 >> 0x03] &= ~(0x0000000F << ((13 & 0x00000007) * 4)); // Clear alternate function for PB13
	GPIOB->AFR[13 >> 0x03] |=  (0x00000005 << ((13 & 0x00000007) * 4)); // Set alternate 5 function for PB13 - SCLK / SPC
	GPIOB->AFR[14 >> 0x03] &= ~(0x0000000F << ((14 & 0x00000007) * 4)); // Clear alternate function for PB13
	GPIOB->AFR[14 >> 0x03] |=  (0x00000005 << ((14 & 0x00000007) * 4)); // Set alternate 5 function for PB13 - MISO
	GPIOB->AFR[15 >> 0x03] &= ~(0x0000000F << ((15 & 0x00000007) * 4)); // Clear alternate function for PB15
	GPIOB->AFR[15 >> 0x03] |=  (0x00000005 << ((15 & 0x00000007) * 4)); // Set alternate 5 function for PB15 - MOSI / SDA
	GPIOB->OSPEEDR |=  (0x00000001 << (13 * 2) | 0x00000001 << (14 * 2) | 0x00000001 << (15 * 2));    // set speed register (0x01 - 10 MHz, 0x02 - 2 MHz, 0x03 - 50 MHz)
	GPIOB->OSPEEDR |=  (0x00000001 << (13 * 2) | 0x00000001 << (14 * 2) | 0x00000001 << (15 * 2));    // set speed register (0x01 - 10 MHz, 0x02 - 2 MHz, 0x03 - 50 MHz)
	GPIOB->OSPEEDR &= ~(0x00000003 << (13 * 2) | 0x00000003 << (14 * 2) | 0x00000003 << (15 * 2));    // Clear speed register
	GPIOB->OTYPER  &= ~(0x0001     << (13)     | 0x0001     << (14)     | 0x0001     << (15));        // Clear output type register
	GPIOB->OTYPER  |=  (0x0000     << (13)     | 0x0000     << (14)     | 0x0000     << (15));        // Set output type register (0x00 - Push pull, 0x01 - Open drain)
	GPIOB->MODER   &= ~(0x00000003 << (13 * 2) | 0x00000003 << (14 * 2) | 0x00000003 << (15 * 2));    // Clear mode register
	GPIOB->MODER   |=  (0x00000002 << (13 * 2) | 0x00000002 << (14 * 2) | 0x00000002 << (15 * 2));    // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOB->PUPDR   &= ~(0x00000003 << (13 * 2) | 0x00000003 << (14 * 2) | 0x00000003 << (15 * 2));    // Clear push/pull register
	GPIOB->PUPDR   |=  (0x00000000 << (13 * 2) | 0x00000000 << (14 * 2) | 0x00000000 << (15 * 2));    // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)
	GPIOB->ODR |=  (0x0001 << 6); // CS = 1


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

//	GPIOB->MODER &= ~( (3 << (13*2)) | (3 << (14*2)) | (3 << (15*2)) );
//	GPIOB->MODER |=  ( (2 << (13*2)) | (2 << (14*2)) | (2 << (15*2)) ); // Alternate function
//	GPIOB->AFR[1] &= ~0xFFF00000;  // clear AFR bits for PB13–PB15
//	GPIOB->AFR[1] |=  0x55500000;  // AF5 (SPI2)
//	GPIOB->OSPEEDR |= (3 << (13*2)) | (3 << (14*2)) | (3 << (15*2)); // High speed
//
//	SPI2->CR1 = 0;
//	SPI2->CR2 = 0;
//
//	SPI2->CR1 |= SPI_CR1_MSTR;      // Master mode
//	SPI2->CR1 |= SPI_CR1_SSI | SPI_CR1_SSM; // Software slave select management
//	SPI2->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA; // Mode 3 (works for LSM9DS1)
//	SPI2->CR1 |= SPI_CR1_BR_1;      // Baud = fPCLK/8
//	SPI2->CR1 &= ~SPI_CR1_LSBFIRST; // MSB first
//	SPI2->CR2 |= (7 << 8);          // 8-bit data
//	SPI2->CR1 |= SPI_CR1_SPE;       // Enable SPI2

	// Sets PB4/D5 to output HIGH / disabled - CSAG
	// CSM
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);	// Port B
	GPIO_InitTypeDef GPIO_InitStructAll;
	GPIO_StructInit(&GPIO_InitStructAll);
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructAll.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructAll.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructAll);

	// Sets PB5/D4 to output LOW / enabled
	// CSAG
	GPIO_StructInit(&GPIO_InitStructAll);
	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructAll.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructAll.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructAll);


	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_SET);


}

//void enableCSPins(){
//	// Sets PB4 to output HIGH / disabled - CSAG
//	// CSM
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);	// Port B
//	GPIO_InitTypeDef GPIO_InitStructAll;
//	GPIO_StructInit(&GPIO_InitStructAll);
//	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructAll.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructAll.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructAll);
//
//	// Sets PB5 to output LOW / enabled
//	// CSAG
//	GPIO_StructInit(&GPIO_InitStructAll);
//	GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructAll.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_5;
//	GPIO_InitStructAll.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructAll);
//
//}



uint8_t spi2_transfer(uint8_t data) {
//    GPIOB->ODR &= ~(0x0001 << 5); // CS = 0 - Start Transmission
    printf("in spi2 transer\n");
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET) { }
    SPI_SendData8(SPI2, data);
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET) { }
//    GPIOB->ODR |=  (0x0001 << 5); // CS = 1 - End Transmission
}

