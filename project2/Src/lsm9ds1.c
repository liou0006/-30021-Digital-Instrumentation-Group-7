#include "lsm9ds1.h"

/* ---------- LSM9DS1 (Accel/Gyro die) registers ---------- */
#define WHO_AM_I     0x0F

void init_SPI_CS(void)
{
	// Enable Clocks
	RCC->AHBENR  |= 0x00020000 | 0x00040000;    // Enable Clock for GPIO Banks A and B
	RCC->APB1ENR |= 0x00004000;                 // Enable Clock for SPI2

	// Connect pins to SPI2
	GPIOB->AFR[13 >> 0x03] &= ~(0x0000000F << ((13 & 0x00000007) * 4)); // Clear alternate function for PB13
	GPIOB->AFR[13 >> 0x03] |=  (0x00000005 << ((13 & 0x00000007) * 4)); // Set alternate 5 function for PB13 - SCLK

	GPIOB->AFR[14 >> 0x03] &= ~(0x0000000F << ((14 & 0x00000007) * 4)); // Clear alternate function for PB13
	GPIOB->AFR[14 >> 0x03] |=  (0x00000005 << ((14 & 0x00000007) * 4)); // Set alternate 5 function for PB13 - SCLK

	GPIOB->AFR[15 >> 0x03] &= ~(0x0000000F << ((15 & 0x00000007) * 4)); // Clear alternate function for PB15
	GPIOB->AFR[15 >> 0x03] |=  (0x00000005 << ((15 & 0x00000007) * 4)); // Set alternate 5 function for PB15 - MOSI

	// Configure pins PB13 and PB15 for 10 MHz alternate function
	GPIOB->OSPEEDR &= ~(0x00000003 << (13 * 2) | 0x00000003 << (14 * 2) | 0x00000003 << (15 * 2));    // Clear speed register
	GPIOB->OSPEEDR |=  (0x00000001 << (13 * 2) | 0x00000001 << (14 * 2) | 0x00000001 << (15 * 2));    // set speed register (0x01 - 10 MHz, 0x02 - 2 MHz, 0x03 - 50 MHz)
	GPIOB->OTYPER  &= ~(0x0001     << (13)     | 0x0001 	<< (14) 	| 0x0001     << (15));        // Clear output type register
	GPIOB->OTYPER  |=  (0x0000     << (13)     | 0x0000 	<< (14) 	| 0x0000     << (15));        // Set output type register (0x00 - Push pull, 0x01 - Open drain)
	GPIOB->MODER   &= ~(0x00000003 << (13 * 2) | 0x00000003 << (14 * 2) | 0x00000003 << (15 * 2));    // Clear mode register
	GPIOB->MODER   |=  (0x00000002 << (13 * 2) | 0x00000002 << (14 * 2) | 0x00000002 << (15 * 2));    // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOB->PUPDR   &= ~(0x00000003 << (13 * 2) | 0x00000003 << (13 * 2) | 0x00000003 << (15 * 2));    // Clear push/pull register
	GPIOB->PUPDR   |=  (0x00000000 << (13 * 2) | 0x00000000 << (13 * 2) | 0x00000000 << (15 * 2));    // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)

	// Configure SPI2
	SPI2->CR1 &= 0x3040; // Clear CR1 Register
	SPI2->CR1 |= 0x0000; // Configure direction (0x0000 - 2 Lines Full Duplex, 0x0400 - 2 Lines RX Only, 0x8000 - 1 Line RX, 0xC000 - 1 Line TX)
	SPI2->CR1 |= 0x0104; // Configure mode (0x0000 - Slave, 0x0104 - Master)
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

	//	lcd_reset();

	// CSM = PB4/D5 ;; CSAG = PB5/D4
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

uint8_t spi2_transfer(uint8_t data) {
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) != SET);
	SPI_SendData8(SPI2, data);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) != SET);
	return SPI_ReceiveData8(SPI2);
}

uint8_t readAG(uint8_t reg) {
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
	spi2_transfer(0x80 | reg);           // send address
	int8_t val = spi2_transfer(0x00);    // send dummy & read value
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
	return val;
}

void writeAG(uint8_t reg, uint8_t data) {
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
	spi2_transfer(0x7F & reg); // makes sure bit 7 is 0
	spi2_transfer(data);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
}

uint8_t readM(uint8_t reg) {
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_RESET);
	spi2_transfer(0x80 | reg);           // send address
	int8_t val = spi2_transfer(0x00);    // send dummy & read value
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_SET);
	return val;
}

int16_t readOutput(uint8_t lowReg){
	uint8_t lower = readAG(lowReg);
	uint8_t higher = readAG(lowReg+1);

	int16_t Gvalue = ((higher << 8) | lower);

	return Gvalue;
}

void printGyroXYZ(){
	int16_t gyroX = readOutput(0x18);
	int16_t gyroY = readOutput(0x1A);
	int16_t gyroZ = readOutput(0x1C);

	printf("GyroX= %d | GyroY= %d | GyroZ= %d\n",gyroX,gyroY,gyroZ);
}

void printAccelXYZ(){
	int16_t accelX = readOutput(0x28);
	int16_t accelY = readOutput(0x2A);
	int16_t accelZ = readOutput(0x2C);

	printf("accelX= %d | accelY= %d | accelZ= %d\n",accelX,accelY,accelZ);


}
