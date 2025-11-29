#ifndef MANUAL_FFT_H_
#define MANUAL_FFT_H_

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include <stdint.h>
#include <stdlib.h>
#include "sensors.h"

#define FFT_SIZE 256
//#define FFT_HALF 128	// Positive freq spectrum (Nyquist limit)
#define LOG2_N 8		// log2(256) = 8

typedef struct {
	float real;
	float imag;
} complex_t;

typedef struct {
	complex_t x_in[FFT_SIZE];	// Complex spectrum
	complex_t x_out[FFT_SIZE];
	float mags[FFT_SIZE/2];	// Magnitudes (positive frequency)
	float max_mag;			// Maximum magnitude
} fft_result_t;

void compute_fft(complex_t *x, uint16_t N);

/*
void compute_fft(lsm9ds1_raw_data_t *samples, int num_samples,
		sensor_t sensor, axis_t axis, fft_result_t *fft_result);
*/

#endif /* MANUAL_FFT_H_ */
