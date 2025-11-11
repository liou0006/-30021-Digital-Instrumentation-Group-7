#include "stm32f30x.h"
#include "ultrasonic_sensor.h"
#include <stdint.h>
#include "openlog_sd.h"

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();          // ensures APB1 freq is correct for BRR
    init_uart(115200);
    delay(1000); // 1s delay between logs
    int count = 0;
    char buffer[32];

    while (count < 250) {
        sprintf(buffer, "%d\r\n", count);
        openlog_writeline(buffer);

        count++;
        delay(2); // 2ms delay between logs
    }
    while (1); // halt after done
}
