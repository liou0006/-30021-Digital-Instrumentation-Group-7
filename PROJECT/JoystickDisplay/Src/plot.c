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
//static float mags[N_FFT/2];	// Static to prevent overflow		(not storing to save space)
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
	float max_label_val;
	uint16_t num_bins;
	bool hist_mode;
} graph_layout_t;

// Simple cache for layout (prevent recompute every draw if parameters stay the same
static graph_layout_t cached_layout;
static bool cached_layout_valid = false;

// Compute graph layout helper function
void compute_graph_layout(bool hist, uint16_t total_graph_width, uint16_t x_base_offset,
		uint16_t graph_height, float max_label_val, uint16_t num_bins, graph_layout_t *out) {
	if (!out) return;

	// Check cache (approx equality for float)
	const float epsilon = 1e-6f;
	if (cached_layout_valid
			&& cached_layout.graph_width == total_graph_width
			&& cached_layout.x_offset == x_base_offset
			&& cached_layout.usable_height == graph_height
			&& cached_layout.num_bins == num_bins
			&& cached_layout.hist_mode == hist
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
	out->hist_mode = hist;

	// Determine number of integer digits required (digits before decimal)
	int digits_before = 1;
	if (max_label_val > 0.0f) {
		digits_before = (int)floorf(log10f(max_label_val)) + 1;
		if (digits_before < 1) digits_before = 1;
	}
	out->num_digits = digits_before;

	// FFT has float labels and histogram has positive int labels
	out->y_axis_offset = 0;
	if (hist) {
		// Default usable width for y-axis labels is 2 digits, we move it only if needed
		// to maximize the actual graph area space
		if (out->num_digits == 3) {
			out->graph_width -= 4;
			out->x_offset += 4;
			out->y_axis_offset = 4;
		}
	} else {
		// FFT float label
		if (digits_before >= 3) {
			// Prevent making graph wider several times
//			if (out->graph_width > 4) {
//				out->graph_width -= 4;
//			} else {
//				out->graph_width = 0;
//			}
			out->graph_width -= 4;
			out->x_offset += 4;
			out->y_axis_offset = 4;
		} else {
//			out->y_axis_offset = 0;

			out->graph_width -= 6;
			out->x_offset += 6;
			out->y_axis_offset = 6;
		}
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
	static complex_t x[N_FFT];
	const uint16_t N = N_FFT;

	// Find scale factor based on sensor type
	float scale_factor = get_scale_factor(sensor);

	// Extract data for chosen sensor and axis
	for (int i = 0; i < N; i++) {
		x[i].real = (float)get_data_val(samples, i, sensor, axis) * scale_factor;
		x[i].imag = 0.0f;
	}

	// ---------- Pre-FFT data processing ----------
	// Here doing operations after incrementing to not include index=0

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

	// Applying Hamming window: w[n] = 0.54 - 0.46 * cos(2*pi*n/(N-1)))
//	for (int n = 0; n < N; ++n) {
//		float w = 0.54f - 0.46 * cosf(2.0f * M_PI * n / (N - 1));
//		x[n].real *= w;
//	}

	// ---------- FFT and magnitude computations ----------
	// Perform Fast Fourier Transform of x
	compute_fft(x, N);

	// Compute magnitudes and (normalized) max magnitude
	float max_mag = 0.0f;
	for (uint16_t k = 0; k < N/2; k++) {	// Compute magnitude (only positive frequency)
		float re = x[k].real;
		float im = x[k].imag;
		float mag = sqrtf(re*re + im*im);

		// Normalize to get amplitude for real input
		// For 0 and Nyquist use 1/N, otherwise scale by 2/N
		float amp;
		if (k == 0 || (k == N/2 && (N % 2 == 0))) amp = mag / (float)N;
		else amp = (2.0f * mag) / (float)N;
		amps[k] = amp;		// Store normalized amplitude for plotting

		// Update max value
		if (amp > max_mag) max_mag = amp;
	}

	// ---------- Drawing FFT bars ----------
	float max_mag_scaled = max_mag * 1000.0f;

	graph_layout_t layout;
	compute_graph_layout(false, GRAPH_WIDTH, GRAPH_X_OFFSET, GRAPH_HEIGHT, max_mag_scaled, 0, &layout);

	// Draw FFT
	const uint16_t n_bins = N / 2;
	for (uint16_t k = 0; k < n_bins; k++) {
		// Map k to x px inside usable graph width
		uint16_t x_pos = layout.x_offset + (uint16_t)((float)k * layout.usable_width / n_bins);

		// Scale bar height to usable graph height
		uint16_t bar_height = 0;
		if (max_mag_scaled > 0.0f) {
			bar_height = (uint16_t)((amps[k] * (float)layout.usable_height) / max_mag_scaled);
		}

		uint16_t y_start = graph_y_to_lcd_y(0);
		uint16_t y_end = graph_y_to_lcd_y(bar_height);

		// Draw vertical line
		lcd_draw_vertical_line(virtualBuffer, VIRTUAL_WIDTH_SIZE, x_pos, y_start, y_end);
	}

	draw_new_axis(layout.y_axis_offset);

	// Decide number of decimals based on num digits before decimal
	int decimals;
	if (max_mag_scaled >= 100.0f) decimals = 0;
	else if (max_mag_scaled >= 10.0f) decimals = 1;
	else decimals = 2;

	for (int t = 0; t <= NUM_Y_TICKS; t++) {
		float frac = (float)t / (float)NUM_Y_TICKS;
		uint16_t tick_h = (uint16_t)(layout.usable_height * frac);
		uint16_t tick_y = graph_y_to_lcd_y(tick_h);

		lcd_draw_horizontal_line(virtualBuffer, VIRTUAL_WIDTH_SIZE, layout.x_offset - 1, layout.x_offset, tick_y);

		if (tick_h != 0) {
			float tick_val = max_mag_scaled * (float)frac;

			int label_x = (int)layout.x_offset - (int)layout.y_axis_offset;
			if (label_x < 0) label_x = 0;

			lcd_convert_float_to_char3x5_y_axis(virtualBuffer, VIRTUAL_WIDTH_SIZE, layout.num_digits, tick_val, (uint16_t)label_x, tick_y - 2, decimals);
//			printf("max_mag=%.6  tick_val=%.6f\n", max_mag, tick_val);
		}
	}
//	printf("\n");

	// Update lcdBuffer with virtualBuffer content
	update_lcdBuffer();
}

void plot_histogram(lsm9ds1_raw_data_t* samples,sensor_t sensor, axis_t axis) {
	// Compute histogram
	int num_bins = 7;	// Fixed number of bins
	histogram_result_t hist;
	compute_histogram(samples, NUM_SAMPLES, sensor, axis, num_bins, &hist);

	// ---------- Draw histogram bars on LCD ----------
	// Graph plotting dimensions
	graph_layout_t layout;
	compute_graph_layout(true, GRAPH_WIDTH, GRAPH_X_OFFSET, GRAPH_HEIGHT, (float)hist.max_bin_height, hist.num_bins, &layout);

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

	// Decide number of decimals based on num digits before decimal
	int decimals;
	if (hist.max_bin_height >= 100.0f) decimals = 0;
	else if (hist.max_bin_height >= 10.0f) decimals = 1;
	else decimals = 2;

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

		// Actual measurement value (scaled by e+3 to display)
		float val = (hist.min_val + (b * hist.bin_width)) * 1000.0f;

		// Draw scaled tick value label
		lcd_convert_float_to_char3x5_x_axis(virtualBuffer, VIRTUAL_WIDTH_SIZE, val, x, 27, decimals);
	}

	free(hist.bin_counts);
	update_lcdBuffer();
}
