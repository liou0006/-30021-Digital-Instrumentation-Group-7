#include "stm32f0xx.h"   // or your exact device header

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

static inline void cs_ag_low(void)  { CS_AG_PORT->BRR  = (1U << CS_AG_PIN); }
static inline void cs_ag_high(void) { CS_AG_PORT->BSRR = (1U << CS_AG_PIN); }

/* ---------- Low-level SPI helpers ---------- */
static uint8_t spi2_txrx(uint8_t byte)
{
    while (!(SPI2->SR & SPI_SR_TXE)) { /* wait */ }
    *((__IO uint8_t*)&SPI2->DR) = byte;              // 8-bit access
    while (!(SPI2->SR & SPI_SR_RXNE)) { /* wait */ }
    return *((__IO uint8_t*)&SPI2->DR);
}

static void ag_write_reg(uint8_t reg, uint8_t value)
{
    cs_ag_low();
    spi2_txrx(reg & 0x3F);            // write, no read bit
    spi2_txrx(value);
    cs_ag_high();
}

static uint8_t ag_read_reg(uint8_t reg)
{
    cs_ag_low();
    spi2_txrx(LSM9DS1_SPI_READ | (reg & 0x3F));
    uint8_t v = spi2_txrx(0x00);
    cs_ag_high();
    return v;
}

static void ag_write_burst(uint8_t start_reg, const uint8_t *data, uint16_t len)
{
    cs_ag_low();
    spi2_txrx((start_reg & 0x3F));        // write
    for (uint16_t i = 0; i < len; ++i) spi2_txrx(data[i]);
    cs_ag_high();
}

static void ag_read_burst(uint8_t start_reg, uint8_t *data, uint16_t len)
{
    cs_ag_low();
    spi2_txrx(LSM9DS1_SPI_READ | LSM9DS1_SPI_AUTOINC | (start_reg & 0x3F));
    for (uint16_t i = 0; i < len; ++i) data[i] = spi2_txrx(0x00);
    cs_ag_high();
}

/* ---------- Public init ---------- */
void init_spi_gyro(void)
{
    /* ---- Enable clocks ---- */
    RCC->AHBENR  |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN; // GPIOA/B
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;                      // SPI2

    /* ---- SPI2 pins: PB13=SCK, PB14=MISO, PB15=MOSI (AF5) ---- */
    // Alternate function selection AF5
    GPIOB->AFR[13 >> 3] &= ~((0xF << ((13 & 7) * 4)) | (0xF << ((14 & 7) * 4)) | (0xF << ((15 & 7) * 4)));
    GPIOB->AFR[13 >> 3] |=  ((0x5 << ((13 & 7) * 4)) | (0x5 << ((14 & 7) * 4)) | (0x5 << ((15 & 7) * 4)));

    // Mode: Alternate Function
    GPIOB->MODER &= ~((3U << (13*2)) | (3U << (14*2)) | (3U << (15*2)));
    GPIOB->MODER |=  ((2U << (13*2)) | (2U << (14*2)) | (2U << (15*2)));

    // Speed: medium (10 MHz) is fine to start
    GPIOB->OSPEEDR &= ~((3U << (13*2)) | (3U << (14*2)) | (3U << (15*2)));
    GPIOB->OSPEEDR |=  ((1U << (13*2)) | (1U << (14*2)) | (1U << (15*2)));

    // Push-pull, no pull resistors
    GPIOB->OTYPER &= ~((1U << 13) | (1U << 14) | (1U << 15));
    GPIOB->PUPDR  &= ~((3U << (13*2)) | (3U << (14*2)) | (3U << (15*2)));

    /* ---- CS pin for AG: PB6 as GPIO output, idle high ---- */
    GPIOB->MODER   &= ~(3U << (CS_AG_PIN*2));
    GPIOB->MODER   |=  (1U << (CS_AG_PIN*2));  // output
    GPIOB->OTYPER  &= ~(1U << CS_AG_PIN);
    GPIOB->OSPEEDR &= ~(3U << (CS_AG_PIN*2));
    GPIOB->OSPEEDR |=  (1U << (CS_AG_PIN*2));
    GPIOB->PUPDR   &= ~(3U << (CS_AG_PIN*2));
    cs_ag_high();

    /* ---- SPI2 configuration for LSM9DS1 ----
     * Mode 3: CPOL=1, CPHA=1
     * Master, software NSS, 8-bit frames
     * Start with a slow prescaler, then you can increase.
     */
    SPI2->CR1 = 0;                             // reset writable bits
    SPI2->CR1 |= SPI_CR1_MSTR;                 // master
    SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;    // software NSS
    SPI2->CR1 |= SPI_CR1_BR_1;                 // fPCLK/8 (adjust as needed)
    SPI2->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA;  // mode 3

    SPI2->CR2 = 0;
    SPI2->CR2 |= (7U << SPI_CR2_DS_Pos);       // 8-bit data size
    SPI2->CR2 |= SPI_CR2_FRXTH;                // RXNE when 8-bit
    SPI2->I2SCFGR &= ~SPI_I2SCFGR_I2SMOD;      // ensure SPI mode

    SPI2->CR1 |= SPI_CR1_SPE;                  // enable SPI2

    /* ---- Optional: sanity check WHO_AM_I (AG should be 0x68) ---- */
    uint8_t who = ag_read_reg(LSM9DS1_AG_WHO_AM_I);
    (void)who; // you can assert/printf if desired

    /* ---- Example gyro configuration ----
     * CTRL_REG1_G (0x10): ODR_G + bandwidth + FS_G
     *   Example: ODR = 119 Hz, BW = automatic, FS = 2000 dps
     * CTRL_REG2_G (0x11): High-pass filter settings (example off)
     * CTRL_REG3_G (0x12): LP mode & HP cutoff (example defaults)
     * CTRL_REG4   (0x1E): Additional settings (e.g., orientation, LSB/MSB first); keep defaults or enable axes if needed.
     *
     * Adjust these values per your needs/datasheet.
     */
    ag_write_reg(LSM9DS1_AG_CTRL_REG1_G, 0xC0); // <-- example: 0b1100_0000 (set an ODR; tweak to taste)
    ag_write_reg(LSM9DS1_AG_CTRL_REG2_G, 0x00); // HPF off
    ag_write_reg(LSM9DS1_AG_CTRL_REG3_G, 0x00); // defaults
    ag_write_reg(LSM9DS1_AG_CTRL_REG4,   0x38); // example: enable X/Y/Z if applicable (adjust per datasheet)
}
