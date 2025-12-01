#include "manual_fft.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

// Bit-reversal permutation (helper function)
static void bit_reverse(complex_t *x, uint16_t N) {
    uint16_t i, j, k;
    for (i = 1, j = 0; i < N; i++) {
        k = N >> 1;
        while (j >= k) {
            j -= k;
            k >>= 1;
        }
        j += k;
        if (i < j) {
            complex_t temp = x[i];
            x[i] = x[j];
            x[j] = temp;
        }
    }
}

void compute_fft(complex_t *x, uint16_t N) {
	bit_reverse(x, N);

	for (uint16_t len = 2; len <= N; len <<= 1) {
		float angle = -2.0f * M_PI / len;
		complex_t wlen = { cosf(angle), sinf(angle) };
		for (uint16_t i = 0; i < N; i += len) {
			complex_t w = { 1.0f, 0.0f };
			for (uint16_t j = 0; j < len/2; j++) {
				complex_t u = x[i + j];
				complex_t t = x[i + j + len/2];

				// Complex multiply v = w * t
				complex_t v;
				v.real = w.real * t.real - w.imag * t.imag;
				v.imag = w.real * t.imag + w.imag * t.real;

				x[i + j].real = u.real + v.real;
				x[i + j].imag = u.imag + v.imag;

				x[i + j + len/2].real = u.real - v.real;
				x[i + j + len/2].imag = u.imag - v.imag;

				// Update w
				float w_real = w.real * wlen.real - w.imag * wlen.imag;
				w.imag = w.real * wlen.imag + w.imag * wlen.real;
				w.real = w_real;
			}
		}
	}
}


