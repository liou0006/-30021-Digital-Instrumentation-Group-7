#include "lsm9ds1.h"

/* ---------- LSM9DS1 (Accel/Gyro die) registers ---------- */
#define LSM9DS1_AG_WHO_AM_I     0x0F   // WHO_AM_I
#define LSM9DS1_AG_CTRL_REG1_G  0x10  //
#define LSM9DS1_AG_CTRL_REG2_G  0x11
#define LSM9DS1_AG_CTRL_REG3_G  0x12
#define LSM9DS1_AG_ORIENT_CFG_G 0x13
#define LSM9DS1_AG_CTRL_REG4    0x1E
#define LSM9DS1_AG_CTRL_REG5_XL 0x1F   // accel (shown for completeness)

/* ---------- SPI command bits for LSM9DS1 ----------
 * IMPORTANT: You set the bit value on bit7 to 1 for READ and 0 for WRITE
 * bit6 = 1 -> auto-increment address for multi-byte
 * bits5..0 = register address
 */
#define LSM9DS1_SPI_READ        0x80
#define LSM9DS1_SPI_AUTOINC     0x40

/* ---------- Pins (SPI2 on GPIOB) ----------
 * PB13 -> SCK (AF5)
 * PB14 -> MISO (AF5)
 * PB15 -> MOSI (AF5)
 * PB06 -> CS_AG (GPIO output, manual)
 */
#define CS_AG_PORT  GPIOB
#define CS_AG_PIN   6

void init_spi_gyro(void)
{
	/* ---- Enable clocks ---- */
	RCC->AHBENR  |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN; // GPIOA/B
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;                      // SPI2

	/* ---- SPI2 pins: PB13=SCK, PB14=MISO, PB15=MOSI (AF5) ---- */
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
	GPIOB->OTYPER  &= ~(0x0001     << (13)     | 0x0001     << (14)     | 0x0001     << (15));        // Clear output type register
	GPIOB->OTYPER  |=  (0x0000     << (13)     | 0x0000     << (14)     | 0x0000     << (15));        // Set output type register (0x00 - Push pull, 0x01 - Open drain)
	GPIOB->MODER   &= ~(0x00000003 << (13 * 2) | 0x00000003 << (14 * 2) | 0x00000003 << (15 * 2));    // Clear mode register
	GPIOB->MODER   |=  (0x00000002 << (13 * 2) | 0x00000002 << (14 * 2) | 0x00000002 << (15 * 2));    // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOB->PUPDR   &= ~(0x00000003 << (13 * 2) | 0x00000003 << (14 * 2) | 0x00000003 << (15 * 2));    // Clear push/pull register
	GPIOB->PUPDR   |=  (0x00000000 << (13 * 2) | 0x00000000 << (14 * 2) | 0x00000000 << (15 * 2));    // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)


//	// Mode: Alternate Function
//	GPIOB->MODER &= ~((3U << (13*2)) | (3U << (14*2)) | (3U << (15*2)));
//	GPIOB->MODER |=  ((2U << (13*2)) | (2U << (14*2)) | (2U << (15*2)));
//
//	// Speed: medium (10 MHz) is fine to start
//	GPIOB->OSPEEDR &= ~((3U << (13*2)) | (3U << (14*2)) | (3U << (15*2)));
//	GPIOB->OSPEEDR |=  ((1U << (13*2)) | (1U << (14*2)) | (1U << (15*2)));
//
//	// Push-pull, no pull resistors
//	GPIOB->OTYPER &= ~((1U << 13) | (1U << 14) | (1U << 15));
//	GPIOB->PUPDR  &= ~((3U << (13*2)) | (3U << (14*2)) | (3U << (15*2)));

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
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, 0);


	/* ---- CS pin for AG: PB6 as GPIO output, idle high ---- */
//	GPIOB->MODER   &= ~(3U << (CS_AG_PIN*2));
//	GPIOB->MODER   |=  (1U << (CS_AG_PIN*2));  // output
//	GPIOB->OTYPER  &= ~(1U << CS_AG_PIN);
//	GPIOB->OSPEEDR &= ~(3U << (CS_AG_PIN*2));
//	GPIOB->OSPEEDR |=  (1U << (CS_AG_PIN*2));
//	GPIOB->PUPDR   &= ~(3U << (CS_AG_PIN*2));
//	cs_ag_high();

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



//
//	/* ---- SPI2 configuration for LSM9DS1 ----
//	 * Mode 3: CPOL=1, CPHA=1
//	 * Master, software NSS, 8-bit frames
//	 * Start with a slow prescaler, then you can increase.
//	 */
//	SPI2->CR1 = 0;                             // reset writable bits
//	SPI2->CR1 |= SPI_CR1_MSTR;                 // master
//	SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;    // software NSS
//	SPI2->CR1 |= SPI_CR1_BR_1;                 // fPCLK/8 (adjust as needed)
//	SPI2->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA;  // mode 3
//
//	SPI2->CR2 = 0;
//	SPI2->CR2 |= (7U << SPI_CR2_DS_Pos);       // 8-bit data size
//	SPI2->CR2 |= SPI_CR2_FRXTH;                // RXNE when 8-bit
//	SPI2->I2SCFGR &= ~SPI_I2SCFGR_I2SMOD;      // ensure SPI mode
//
//	SPI2->CR1 |= SPI_CR1_SPE;                  // enable SPI2
//
//	/* ---- Optional: sanity check WHO_AM_I (AG should be 0x68) ---- */
//	uint8_t who = ag_read_reg(LSM9DS1_AG_WHO_AM_I);
//	(void)who; // you can assert/printf if desired
//
//	/* ---- Example gyro configuration ----
//	 * CTRL_REG1_G (0x10): ODR_G + bandwidth + FS_G
//	 *   Example: ODR = 119 Hz, BW = automatic, FS = 2000 dps
//	 * CTRL_REG2_G (0x11): High-pass filter settings (example off)
//	 * CTRL_REG3_G (0x12): LP mode & HP cutoff (example defaults)
//	 * CTRL_REG4   (0x1E): Additional settings (e.g., orientation, LSB/MSB first); keep defaults or enable axes if needed.
//	 *
//	 * Adjust these values per your needs/datasheet.
//	 */
//	ag_write_reg(LSM9DS1_AG_CTRL_REG1_G, 0xC0); // <-- example: 0b1100_0000 (set an ODR; tweak to taste)
//	ag_write_reg(LSM9DS1_AG_CTRL_REG2_G, 0x00); // HPF off
//	ag_write_reg(LSM9DS1_AG_CTRL_REG3_G, 0x00); // defaults
//	ag_write_reg(LSM9DS1_AG_CTRL_REG4,   0x38); // example: enable X/Y/Z if applicable (adjust per datasheet)


}
