#include "fft.h"

void compute_fft_mags(uint16_t *x) {
	//
}

void compute_fft(uint8_t *buffer, uint16_t buff_width) {
	int N = 32;				// Number of samples
	double complex c[N];
	double x[N];
	uint16_t mags[N];
	uint16_t max_mag = 1;	// Set to 1 to avoid division with 0

	// Define x[n]	(in here for now)
	for (int n = 0; n < N; n++) {
//		x[n] = cos(2 * M_PI * 2 * n / N) + 0.5 * sin(2 * M_PI * 4 * n / N);
		x[n] = 0.8 + 0.7 * sin(2 * M_PI * 50 * n / N) + sin(2 * M_PI * 120 * n / N);
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

	// Scale x and y to fit graph axes
	for (int k = 0; k < N; k++) {
		// Horizontal scaling
		uint16_t x_scaled = (uint16_t)((float)k * (buff_width - GRAPH_X_OFFSET) / N);
//		uint16_t x_scaled = (uint16_t)((float)k * (LCD_LINE_SIZE - GRAPH_X_OFFSET) / N);	// If we don't want scrolling

		// Vertical scaling
		uint16_t mag_scaled = (uint16_t)((float)mags[k] * GRAPH_HEIGHT / max_mag);
		uint16_t y_start = graph_y_to_lcd_y(0);
		uint16_t y_end = graph_y_to_lcd_y(mag_scaled);

		// Draw vertical line
		lcd_draw_vertical_line(buffer, buff_width, GRAPH_X_OFFSET + x_scaled, y_start, y_end);
	}
}




