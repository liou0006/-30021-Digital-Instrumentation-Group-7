#include "fft.h"

void compute_fft() {
	int N = 16;
	double complex c[N];
	double x[N];

	// Define x[n]
	for (int n = 0; n < N; n++) {
		x[n] = cos(2 * M_PI * 2 * n / N) + 0.5 * sin(2 * M_PI * 4 * n / N);
	}

	// Compute c[k]
	for (int k = 0; k < N; k++) {
		c[k] = 0.0 + 0.0 * I;
		for (int n = 0; n < N; n++) {
			double angle = 2 * M_PI * n * k / N;
			double complex w = cos(angle) + I * sin(angle);	// e^(j*angle)
			c[k] += x[n] * w;
		}
	}

	// Print results
	for (int k = 0; k < N; k++) {
		printf("|c[%d]| = %.4f\n", k, sqrt(creal(c[k]) * creal(c[k]) + cimag(c[k]) * cimag(c[k])));
	}
}




