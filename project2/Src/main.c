
#include "stm32f30x.h"
#include <stdint.h>
#include <stdbool.h>
#include "uart_sd_card.h"

/* Minimal LED pin (Nucleo-F302R8: LD2 on PA5) for status blink */
static void led_init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    /* MODER5: 01b = General purpose output */
    GPIOA->MODER &= ~(3u << (5u*2u));
    GPIOA->MODER |=  (1u << (5u*2u)); // output
}
static void led_on(void)  { GPIOA->BSRR = GPIO_BSRR_BS_5; }
static void led_off(void) { GPIOA->BSRR = GPIO_BSRR_BR_5; }
static void led_toggle(void) { GPIOA->ODR ^= (1u << 5); }

int main(void) {
    //led_init();
    serial1_init_57600();

    /* Give OpenLog a moment to boot */
    delay_ms(1000);

    /* Create (or overwrite) data.txt and write a line */
    //openlog_create_data_txt();
    bool ok = openlog_write_data_txt("Hello from STM32 at 57600 baud!\r\n");

    /* Blink status: fast if OK, slow if failed */
    while (1) {
        //led_toggle();
        delay_ms(600);
    }
}
