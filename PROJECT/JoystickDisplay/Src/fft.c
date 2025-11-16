#include "fft.h"

// Functions are part of the API layer and are purely FFT maths

#define N 256			// FFT size
#define LOG2_N 8		// log2(256) = 8
#define HALF_N (N/2)	// 128, Nyquist limit

// Pre-computed twiddle factors
static float cos_table[HALF_N];
static float sin_table[HALF_N];

static int init = 0;

static void fft_init() {
	if (init) return;
	for (int k = 0; k < HALF_N; k++) {
		float angle = -2.0f * M_PI * k / N;
		cos_table[k] = cosf(angle);
		sin_table[k] = sinf(angle);
	}
	init = 1;
}

// Bit reversal
static unsigned bit_reverse(unsigned x, unsigned bits) {
	unsigned r = 0;
	for (unsigned i = 0; i < bits; i++) {
		r = (r << 1) | (x & 1);
		x >>= 1;
	}
	return r;
}

// In-place radix-2 FFT (taken from ChatGPT)
void fft256(complex_t *x) {
	fft_init();

	// Bit-reverse reorder
	for (unsigned i = 0; i < N; i++) {
		unsigned j = bit_reverse(i, LOG2_N);
		if (j > i) {
			complex_t temp = x[i];
			x[i] = x[j];
			x[j] = temp;
		}
	}

	// FFT stages
	for (unsigned step = 1; step < N; step <<= 1) {
		unsigned jump = step << 1;
		unsigned twiddle_strike = HALF_N / step;

		for (unsigned group = 0; group < step; group++) {
			float c = cos_table[group * twiddle_strike];
			float s = sin_table[group * twiddle_strike];

			for (unsigned pair = group; pair < N; pair += jump) {
				unsigned match = pair + step;

				float tr = c * x[match].real - s * x[match].imag;
				float ti = s * x[match].real + c * x[match].imag;

				float ur = x[pair].real;
				float ui = x[pair].imag;

				x[pair].real = ur + tr;
				x[pair].imag = ui + ti;

				x[match].real = ur - tr;
				x[match].imag = ui - ti;
			}
		}
	}
}

// Magnitudes (first 128 bins only)
void fft256_magnitude(complex_t *x, float *mag_out) {
	for (int k = 0; k < HALF_N; k++) {
		float real = x[k].real;
		float imag = x[k].imag;
		mag_out[k] = sqrtf((real * real) + (imag * imag));
	}
}





//void compute_fft(const float *x, float complex *out, int N) {
//	// Compute Discrete Fourier Transform (DFT)
//	for (int k = 0; k < N; k++) {
//		out[k] = 0.0;
//		for (int n = 0; n < N; n++) {
//			double angle = -2 * M_PI * n * k / N;
//			out[k] += x[n] * (cos(angle) + I * sin(angle));
//		}
//	}
//}
//
//void compute_fft_magnitude(const float complex *fft_data,
//		float *mag, int N) {
//	// Compute magnitude of DFT data
//	for (int k = 0; k < N/2; k++) {
//		float re = crealf(fft_data[k]);
//		float im = cimagf(fft_data[k]);
//		mag[k] = sqrtf((re * re) + (im * im));
//	}
//}

