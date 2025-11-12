#include "fft.h"

void compute_fft_mags(uint16_t *x) {
	//
}

void compute_fft(uint8_t *buffer, uint16_t buff_width) {
	int N = 64;				// Number of samples (has to be power of 2)
	double Fs = 80.0;		// Sampling frequency
//	double f = 50.0;		// Signal frequency

	double x[N];
	double complex c[N];
	uint16_t mags[N/2];		// Only store first half of FFT
	uint16_t max_mag = 1.0;	// Set to 1 to avoid division with 0

	// Define x[n]	(generating test signal)
	for (int n = 0; n < N; n++) {
//		x[n] = cos(2 * M_PI * 2 * n / N) + 0.5 * sin(2 * M_PI * 4 * n / N);
//		x[n] = 0.8 + 0.7 * sin(2 * M_PI * 50 * n / N) + sin(2 * M_PI * 120 * n / N);
		x[n] = cos(2 * M_PI * 50 * n / Fs);
//		x[n] = 3*cos(2*M_PI*2*n/Fs) + 2*cos(2*M_PI*4*n/Fs) + sin(2*M_PI*6*n/Fs);
	}

	// Compute DFT
	for (int k = 0; k < N; k++) {
		c[k] = 0.0;
		for (int n = 0; n < N; n++) {
			double angle = -2 * M_PI * n * k / N;
			c[k] += x[n] * (cos(angle) + I * sin(angle));
		}
	}

	// Compute magnitude and update max magnitude
	for (int k = 0; k < N/2; k++) {
		mags[k] = sqrt((creal(c[k]) * creal(c[k])) + (cimag(c[k]) * cimag(c[k])));
		if (mags[k] > max_mag) max_mag = mags[k];
	}

	// Scale x and y to fit graph axes
	for (int k = 0; k < N/2; k++) {
//		double freq = (double)k * Fs / N;	// Frequency corresponding to bin k

		// Horizontal scaling
		uint16_t x_scaled = (uint16_t)((float)k * (buff_width - 1 - GRAPH_X_OFFSET - GRAPH_MARGIN_RIGHT) / (N/2));
		uint16_t x_pos = GRAPH_X_OFFSET + x_scaled;

		// Vertical scaling
		uint16_t mag_scaled = (uint16_t)((float)mags[k] * GRAPH_HEIGHT / max_mag);
		uint16_t y_start = graph_y_to_lcd_y(0);
		uint16_t y_end = graph_y_to_lcd_y(mag_scaled);

		// Draw vertical line
		lcd_draw_vertical_line(buffer, buff_width, x_pos, y_start, y_end);
	}

	// X-axis ticks and labels
	const int num_ticks = 5;
	const double f_max = Fs / 2.0;	// Nyquist frequency

	for (int i = 0; i <= num_ticks; i++) {
		double freq = i * f_max / num_ticks;
		int k = (int)((freq * N) / Fs);
		uint16_t x_tick = GRAPH_X_OFFSET + (uint16_t)((float)k * (buff_width - 1 - GRAPH_X_OFFSET - GRAPH_MARGIN_RIGHT) / (N/2));

		// Tick mark
		lcd_draw_vertical_line(buffer, buff_width, x_tick, graph_y_to_lcd_y(0) + 1, graph_y_to_lcd_y(0));

		// Tick label
		if (i == 0) {
			lcd_draw_char3x5(buffer, buff_width, x_tick - 1, graph_y_to_lcd_y(0) + 2, '0');
		} else if (i == 1) {
			lcd_draw_char3x5(buffer, buff_width, x_tick - 5, graph_y_to_lcd_y(0) + 2, '1');
			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 4, graph_y_to_lcd_y(0) + 2, '0');
			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 8, graph_y_to_lcd_y(0) + 2, '0');
		} else if (i == 2) {
			lcd_draw_char3x5(buffer, buff_width, x_tick - 5, graph_y_to_lcd_y(0) + 2, '2');
			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 4, graph_y_to_lcd_y(0) + 2, '0');
			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 8, graph_y_to_lcd_y(0) + 2, '0');
		} else if (i == 3) {
			lcd_draw_char3x5(buffer, buff_width, x_tick - 5, graph_y_to_lcd_y(0) + 2, '3');
			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 4, graph_y_to_lcd_y(0) + 2, '0');
			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 8, graph_y_to_lcd_y(0) + 2, '0');
		} else if (i == 4) {
			lcd_draw_char3x5(buffer, buff_width, x_tick - 5, graph_y_to_lcd_y(0) + 2, '4');
			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 4, graph_y_to_lcd_y(0) + 2, '0');
			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 8, graph_y_to_lcd_y(0) + 2, '0');
		} else if (i == 5) {
			lcd_draw_char3x5(buffer, buff_width, x_tick - 5, graph_y_to_lcd_y(0) + 2, '5');
			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 4, graph_y_to_lcd_y(0) + 2, '0');
			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 8, graph_y_to_lcd_y(0) + 2, '0');
		}


//		if (i == 0) {
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 1, graph_y_to_lcd_y(0) + 2, '0');
//		} else if (i == 1) {
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 3, graph_y_to_lcd_y(0) + 2, '5');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 3 + 4, graph_y_to_lcd_y(0) + 2, '0');
//		} else if (i == 2) {
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5, graph_y_to_lcd_y(0) + 2, '1');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 4, graph_y_to_lcd_y(0) + 2, '0');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 8, graph_y_to_lcd_y(0) + 2, '0');
//		} else if (i == 3) {
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5, graph_y_to_lcd_y(0) + 2, '1');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 4, graph_y_to_lcd_y(0) + 2, '5');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 8, graph_y_to_lcd_y(0) + 2, '0');
//		} else if (i == 4) {
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5, graph_y_to_lcd_y(0) + 2, '2');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 4, graph_y_to_lcd_y(0) + 2, '0');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 8, graph_y_to_lcd_y(0) + 2, '0');
//		} else if (i == 5) {
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5, graph_y_to_lcd_y(0) + 2, '2');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 4, graph_y_to_lcd_y(0) + 2, '5');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 8, graph_y_to_lcd_y(0) + 2, '0');
//		} else if (i == 6) {
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5, graph_y_to_lcd_y(0) + 2, '3');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 4, graph_y_to_lcd_y(0) + 2, '0');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 8, graph_y_to_lcd_y(0) + 2, '0');
//		} else if (i == 7) {
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5, graph_y_to_lcd_y(0) + 2, '3');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 4, graph_y_to_lcd_y(0) + 2, '5');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 8, graph_y_to_lcd_y(0) + 2, '0');
//		} else if (i == 8) {
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5, graph_y_to_lcd_y(0) + 2, '4');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 4, graph_y_to_lcd_y(0) + 2, '0');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 8, graph_y_to_lcd_y(0) + 2, '0');
//		} else if (i == 9) {
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5, graph_y_to_lcd_y(0) + 2, '4');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 4, graph_y_to_lcd_y(0) + 2, '5');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 8, graph_y_to_lcd_y(0) + 2, '0');
//		} else if (i == 10) {
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5, graph_y_to_lcd_y(0) + 2, '5');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 4, graph_y_to_lcd_y(0) + 2, '0');
//			lcd_draw_char3x5(buffer, buff_width, x_tick - 5 + 8, graph_y_to_lcd_y(0) + 2, '0');
//		}
	}
}




