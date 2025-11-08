#ifndef FFT_H_
#define FFT_H_

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include <stdint.h>
#include <math.h>
#include <complex.h>
#include "lcd_graphics.h"

void compute_fft(uint8_t *buffer, uint16_t buff_width);

#endif /* FFT_H_ */
