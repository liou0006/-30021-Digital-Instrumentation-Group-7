#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* HC-SR04 on STM32F302R8 (HAL-free)
 * Trig -> PA8 (output)
 * Echo -> PA9 (input, EXTI9_5 interrupt)
 *
 * Design:
 * - TIM2 runs at 1 MHz as a free-running counter (µs tick)
 * - EXTI interrupt on PA9 captures rising and falling timestamps
 * - Driver computes high-pulse width => distance
 */

/* Initialize GPIO, EXTI, and TIM2 1 MHz timebase */
void ultrasonic_init(void);

/* Trigger a 10 µs pulse on Trig (PA8). */
void ultrasonic_trigger(void);

/* Measure one distance (blocking with timeout).
 * timeout_ms: overall timeout waiting for a complete echo window
 * Returns: true on success, false on timeout.
 * On success, *distance_cm is set (integer centimeters).
 */
bool ultrasonic_measure_cm(uint32_t timeout_ms, uint32_t *distance_cm);

void debug_uart2_init(void);

void debug_uart2_print(const char *s);

#ifdef __cplusplus
}
#endif

#endif /* ULTRASONIC_SENSOR_H */
