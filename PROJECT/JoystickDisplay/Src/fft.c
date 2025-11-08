#include "fft.h"

void compute_fft(uint8_t *buffer, uint16_t buff_width) {
	int N = 32;				// Number of samples
	double complex c[N];
	double x[N];
	uint16_t mags[N];
	uint16_t max_mag = 1;	// Set to 1 to avoid division with 0

	// Define x[n]	(in here for now)
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

		// Compute magnitude and update max magnitude
		mags[k] = (uint16_t)sqrt(creal(c[k]) * creal(c[k]) + cimag(c[k]) * cimag(c[k]));
		if (mags[k] > max_mag) max_mag = mags[k];
	}

	// Print results
//	for (int k = 0; k < N; k++) {
//		printf("|c[%d]| = %.4f\n", k, sqrt(creal(c[k]) * creal(c[k]) + cimag(c[k]) * cimag(c[k])));
//	}

	for (int k = 0; k < N; k++) {
		// Horizontal scaling
		uint16_t x_pos = (uint16_t)((float)k * buff_width / N);

		// Vertical scaling
		uint16_t y_scaled = (uint16_t)((float)mags[k] * LCD_HEIGHT / max_mag);
		uint16_t y_start = LCD_HEIGHT - 1;
		uint16_t y_end = LCD_HEIGHT - 1 - y_scaled;

		// Draw vertical line
		lcd_draw_vertical_line(buffer, buff_width, x_pos, y_start, y_end);
	}
}




