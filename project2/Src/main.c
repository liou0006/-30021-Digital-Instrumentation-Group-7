
#include "stm32f30x.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "uart_sd_card.h"

int main(void) {
    // led_init(); // enable if you want a status blink
    serial1_init_57600();          // USART2 on PA2/PA3 @ 57600 (matches config.txt)
	printf("Initialize Complete.. Waiting for newlog prompt");	// moves cursor to row 4 column 20
    (void)openlog_wait_newlog_prompt(1500); // optional: look for '<'

    /* Stream a couple of lines into LOG#####.TXT */
    //openlog_log_writeline("ValueA=123, ValueB=456");

    /* Idle loop */
    while (1) {
        // led_toggle();
        openlog_log_writeline("X");
        delay_ms(1000);
    }
}
