#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "lcd.h"
#include "interrupt.h"
#include "timer.h"
#include "window.h"
#include "flash.h"
#include "lsm9ds1.h"
#include "uart_sd_card.h"

// ---- LSM9DS1 (AG die) registers ----
#define WHO_AM_I_AG   0x0F   // read-only, should return 0x68
#define CTRL_REG8     0x22   // writable
#define IF_ADD_INC    (1u<<2)

// Prototypes from your SPI code
uint8_t lsm9_ag_read(uint8_t reg);
void    lsm9_ag_write(uint8_t reg, uint8_t value);

int main(void)
{
    uart_init(9600);
    initJoystick();
    initLed();
    iniEXTIA4();
    initTimer();

    lcd_init_and_print();
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    HAL_Delay(500); // allow OpenLog to boot and show '>' in the terminal

    while (1)
    {
        HAL_Delay(1000); // Allow OpenLog idle time before writing to the SD card

        // Write to SD
        HAL_StatusTypeDef st = openlog_write_text("log.txt", "Hello from STM32!\r\nLine 2...\r\n");
        if (st != HAL_OK) {
        	printf("Problem writing to SD Card");
        }

        HAL_Delay(2000); // Allow OpenLog to flush and blink LEDs before reading

        // Read back file
        char readbuf[512];
        int n = openlog_read_text("log.txt", readbuf, sizeof(readbuf));

        // TODO: test/see if one should use readbuf data struct here to read from the SD card
        HAL_Delay(5000); // Pause before next cycle
    }


}
