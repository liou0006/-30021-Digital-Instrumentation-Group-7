#include "stm32f30x.h"
#include "ultrasonic_sensor.h"
#include <stdint.h>



 int main(void) {
    SystemInit();
    SystemCoreClockUpdate();          // ensures APB1 freq is correct for BRR
    ultrasonic_init();                // sets up PA8/PA9 + TIM2, EXTI

    while (1) {
        uint32_t cm;
        if (ultrasonic_measure_cm(60, &cm)) {
            // prints "Distance: NN cm" inside the function
        }
        for (volatile uint32_t d=0; d<800000; ++d) __NOP();
    }
}



