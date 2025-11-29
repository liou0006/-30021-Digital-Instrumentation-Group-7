#include "plot.h"
#include "manual_fft.h"
#include "histogram.h"
#include "lcd_graphics.h"
#include "lcd.h"
#include "sensors.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// Graph dimension
#define GRAPH_HEIGHT (LCD_HEIGHT - GRAPH_MARGIN_TOP - GRAPH_MARGIN_BOTTOM)
#define GRAPH_WIDTH (VIRTUAL_WIDTH_SIZE - GRAPH_X_OFFSET - GRAPH_MARGIN_RIGHT)

// y-axis label (is essentially 5 since it has [0 %, 25 %, 50 %, 75 %, 100 %]
#define NUM_Y_TICKS 4

// FFT specific
#define N_FFT 256	// Needs to be power of 2 and correspond to LSM9DS1 num samples
static float mags[N_FFT/2];	// Static to prevent overflow
static float amps[N_FFT/2];	// Static to prevent overflow

// Local graph layout helper
typedef struct {
	uint16_t graph_width;
	uint16_t x_offset;
	uint16_t usable_width;
	uint16_t usable_height;
	uint16_t bar_width;
	uint16_t y_axis_offset;
	int num_digits;
	int decimal_places;
	int max_label_chars;
	float max_label_val;
	uint16_t num_bins;
	bool int_labels;
} graph_layout_t;

// Simple cache for layout (prevent recompute every draw if parameters stay the same
static graph_layout_t cached_layout;
static bool cached_layout_valid = false;

// Compute graph layout helper function
void compute_graph_layout(uint16_t total_graph_width, uint16_t x_base_offset,
		uint16_t graph_height, int max_label_val, uint16_t num_bins, graph_layout_t *out) {
	if (!out) return;

	// Check cache (approx equality for float)
	const float epsilon = 1e-6f;
	if (cached_layout_valid
			&& cached_layout.graph_width == total_graph_width
			&& cached_layout.x_offset == x_base_offset
			&& cached_layout.usable_height == graph_height
			&& cached_layout.num_bins == num_bins
			&& fabsf(cached_layout.max_label_val - max_label_val) < epsilon) {
		// Reuse cached layout
		*out = cached_layout;
		return;
	}

	// Base values
	out->graph_width = total_graph_width;
	out->x_offset = x_base_offset;
	out->usable_height = graph_height;
	out->num_bins = num_bins;
	out->max_label_val = max_label_val;

	// Determine max number of digits needed for y-axis labels
	// (determine digits before decimals)
	int digits_before;
	if (max_label_val <= 0.0f) {
		digits_before = 1;
	} else {
		digits_before = (int)floorf(log10f(max_label_val)) + 1;
		if (digits_before < 1) digits_before = 1;
	}
	out->num_digits = digits_before;

	// * Histogram has a num digits max equal to its num samples (256)
	// * FFT has float y-axis labels therefore '.' count as a 1 pixel col digit
	if (max_label_val <= 0) {
		// Shouldn't be possible
		out->num_digits = 1;
	} else {
		out->num_digits = (int)floor(log10((double)max_label_val)) + 1;
	}

	// Default usable width for y-axis labels is 2 digits, we move it only if needed
	// to maximize the actual graph area space
	out->y_axis_offset = 0;
	if (out->num_digits == 3) {
		out->graph_width -= 4;
		out->x_offset += 4;
		out->y_axis_offset = 4;
	}

	// Update usable graph width
	out->usable_width = out->graph_width;

	// Compute bar bar width for histogram and prevent zero bins
	// Compute per bin width in pixels
	if (num_bins == 0) {
		out->bar_width = 0;
	} else {
		int16_t bw = out->usable_width / num_bins;
		out->bar_width = bw < 1 ? 1 : bw;
	}
}

void plot_fft(lsm9ds1_raw_data_t *samples, sensor_t sensor, axis_t axis) {
	// ---------- Data extraction ----------
	// Extract data for chosen sensor and axis
	static complex_t x[N_FFT];
	const uint16_t N = N_FFT;
	for (int i = 0; i < N; i++) {
		x[i].real = (float)get_data_val(samples, i, sensor, axis);
		x[i].imag = 0.0f;
	}

	// ---------- Pre-FFT data processing ----------
	// Remove DC offset (mean) before FFT (preventing DC dominating max value)
	float mean = 0.0f;
	for (int i = 0; i < N; ++i) mean += x[i].real;
	mean /= (float)N;
	for (int i = 0; i < N; ++i) x[i].real -= mean;

	// Applying Hann window: w[n] = 0.5 * (1 - cos(2*pi*n/(N-1)))
	for (int n = 0; n < N; ++n) {
		float w = 0.5f * (1.0f - cosf(2.0f * M_PI * n / (N - 1)));
		x[n].real *= w;
	}

	// ---------- FFT and magnitude computations ----------
	// Perform Fast Fourier Transform of x
	compute_fft(x, N);

	// Compute magnitudes and (normalized) max magnitude
	float max_mag = 0.0f;
	float max_mag2 = 0.0f;	// Magnitude-squared for faster comparison

	for (uint16_t k = 0; k < N/2; k++) {	// Compute magnitude (only positive frequency)
		float re = x[k].real;
		float im = x[k].imag;
		float mag2 = re*re + im*im;			// Unnormalized un-squared magnitude
		float mag = sqrtf(mag2);			// Unnormalized squared magnitude

		// Normalize to get amplitude for real input
		// For 0 and Nyquist use 1/N, otherwise scale by 2/N
		float amp;
		if (k == 0 || (k == N/2 && (N % 2 == 0))) {
			amp = mag / (float)N;
		} else {
			amp = (2.0f * mag) / (float)N;
		}

		// Compare using squared amplitude to avoid sqrt in loop
		float amp2 = amp * amp;
		if (amp2 > max_mag2) {
			max_mag2 = amp2;
			max_mag = amp;		// Store squared valued as final value
		}

		// Store magnitudes for plotting
		mags[k] = mag;			// Un-normalized magnitude
		amps[k] = amp;			// Normalized magnitude
	}

	// ---------- Printing used for debugging ----------
	// Print max magnitude for debugging
//	char line0[24];
//	sprintf(line0, "%.2f", max_mag);
//	lcd_write_string((uint8_t *)line0, virtualBuffer, 0, 0);

	// ---------- Drawing FFT bars ----------
	// Graph window dimensions
//	uint16_t hist_graph_width = GRAPH_WIDTH;
//	uint16_t hist_x_offset = GRAPH_X_OFFSET;

//	int num_digits;
//	if (max_mag_int <= 0) {
//		num_digits = 1; // Treat 0 or negative as 1 digit for '0' label
//	} else {
//		num_digits = (int)floor(log10(max_mag_int)) + 1;
//	}

//	uint16_t y_axis_offset = 0;
//	if (num_digits == 3) {
//		hist_graph_width -= 4;
//		hist_x_offset += 4;
//		y_axis_offset = 4;
//	}

	draw_graph_axis();

	// Update lcdBuffer with virtualBuffer content
	update_lcdBuffer();
}

void plot_histogram(lsm9ds1_raw_data_t* samples,sensor_t sensor, axis_t axis) {
	// Compute histogram
	int num_bins = 10;	// Fixed number of bins
	histogram_result_t hist;
	compute_histogram(samples, NUM_SAMPLES, sensor, axis, num_bins, &hist);

	// ---------- Draw histogram bars on LCD ----------
	// Graph plotting dimensions
	graph_layout_t layout;
	compute_graph_layout(GRAPH_WIDTH, GRAPH_X_OFFSET, GRAPH_HEIGHT,
			hist.max_bin_height, hist.num_bins, &layout);

	uint16_t bar_width = layout.bar_width;
	if (bar_width < 1) bar_width = 1;

	for (uint16_t b = 0; b < hist.num_bins; b++) {
		// Compute x-coordinates
		uint16_t x_start = layout.x_offset + b * layout.bar_width;
		uint16_t x_end = x_start + layout.bar_width;

		// Compute scaled bar height
		uint16_t bar_height = 0;
		if (hist.max_bin_height > 0) {
			bar_height = (uint16_t)((hist.bin_counts[b] * layout.usable_height) / hist.max_bin_height);
		}

		// Convert to LCD coordinate system
		uint16_t y_start = graph_y_to_lcd_y(0);
		uint16_t y_end = graph_y_to_lcd_y(bar_height);

		// Draw vertical bar
		lcd_draw_vertical_line(virtualBuffer, VIRTUAL_WIDTH_SIZE, x_start, y_start, y_end);
		if (x_end > x_start) {
			lcd_draw_vertical_line(virtualBuffer, VIRTUAL_WIDTH_SIZE, x_end, y_start, y_end);
		}
		lcd_draw_horizontal_line(virtualBuffer, VIRTUAL_WIDTH_SIZE, x_start, x_end, y_end);
	}

	// Draw axes
	draw_new_axis(layout.y_axis_offset);

	// Draw y-axis ticks and labels
	for (int t = 0; t <= NUM_Y_TICKS; t++) {
		float height_frac = (float)t / NUM_Y_TICKS;
		uint16_t tick_height = (uint16_t)(GRAPH_HEIGHT * height_frac);
		uint16_t tick_y = graph_y_to_lcd_y(tick_height);

		// Draw tick mark
		lcd_draw_horizontal_line(virtualBuffer, VIRTUAL_WIDTH_SIZE,
				layout.x_offset - 1, layout.x_offset, tick_y);

		if (tick_height != 0) {
			// Compute tick value
			int tick_val = (int)roundf((float)hist.max_bin_height * height_frac);
			// Draw tick label
			lcd_convert_int_to_char3x5_y_axis(virtualBuffer, VIRTUAL_WIDTH_SIZE, layout.num_digits, tick_val, 0, tick_y - 2);
		}
	}

	// Draw x-axis ticks and labels
	for (int b = 0; b <= hist.num_bins; b++) {
		uint16_t x = layout.x_offset + b * layout.bar_width;
		float val = hist.min_val + (b * hist.bin_width);	// Actual measurement value

		// Draw tick label
		lcd_convert_int_to_char3x5_x_axis(virtualBuffer, VIRTUAL_WIDTH_SIZE, (int)val, x, 27);
	}

	free(hist.bin_counts);
	update_lcdBuffer();
}
