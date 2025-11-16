#ifndef FFT_H_
#define FFT_H_

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course

#include <stdint.h>
#include <math.h>
//#include "fft_data.h"	// Has shared struct for sample data

typedef struct {
	float real;
	float imag;
} complex_t;

void fft256(complex_t *x);
void fft256_magnitude(complex_t *x, float *mag_out);

//void compute_fft(const float *x, float complex *out, int N);
//void compute_fft_magnitude(const float complex *fft_data,
//		float *mag, int N);

#endif /* FFT_H_ */
