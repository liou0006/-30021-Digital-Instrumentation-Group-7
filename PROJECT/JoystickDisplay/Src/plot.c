#include "plot.h"

#include "fft.h"
#include "histogram.h"
#include "lcd_graphics.h"
#include "lcd.h"
#include "sensors.h"

#define NUM_SAMPLES 256
#define GRAPH_HEIGHT (LCD_HEIGHT - GRAPH_MARGIN_TOP - GRAPH_MARGIN_BOTTOM)
#define GRAPH_WIDTH (VIRTUAL_WIDTH_SIZE - GRAPH_X_OFFSET - GRAPH_MARGIN_RIGHT)
#define NUM_Y_TICKS 4

void plot_fft(sensor_t sensor, axis_t axis) {
	lcd_write_string("FFT plot", virtualBuffer, 0, 0);
//	lsm9ds1_raw_data_t samples[FFT_NUM_SAMPLES];
//	sensors_read_samples(samples, FFT_NUM_SAMPLES);
}

void plot_histogram(sensor_t sensor, axis_t axis) {
	// Get data
	lsm9ds1_raw_data_t samples[NUM_SAMPLES];
	sensors_read_samples(samples, NUM_SAMPLES);

	int num_bins = 10;

	// Compute histogram
	histogram_result_t hist;
	compute_histogram(samples, NUM_SAMPLES, sensor, axis, num_bins, &hist);

	uint16_t hist_graph_width = GRAPH_WIDTH;
	uint16_t hist_x_offset = GRAPH_X_OFFSET;

	int num_digits;
	if (hist.max_bin_height <= 0) {
		num_digits = 1; // Treat 0 or negative as 1 digit for '0' label
	} else {
		num_digits = (int)floor(log10(hist.max_bin_height)) + 1;
	}

	uint16_t y_axis_offset = 0;
	if (num_digits == 3) {
		hist_graph_width -= 4;
		hist_x_offset += 4;
		y_axis_offset = 4;
	}

	// ----- Draw histogram bars on LCD -----
	// Graph plotting dimensions
	uint16_t usable_width = hist_graph_width;
	uint16_t usable_height = GRAPH_HEIGHT;

	// Compute per bin width in pixels
	uint16_t bar_width = usable_width / hist.num_bins;
	if (bar_width < 1) bar_width = 1;

	for (uint16_t b = 0; b < hist.num_bins; b++) {
		// Compute x-coordinates
		uint16_t x_start = hist_x_offset + b * bar_width;
		uint16_t x_end = x_start + bar_width;

		// Compute scaled bar height
		uint16_t bar_height = (uint16_t)((hist.bin_counts[b] * usable_height) / hist.max_bin_height);

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
	draw_new_axis(y_axis_offset);

	// Draw y-axis ticks and labels
	for (int t = 0; t <= NUM_Y_TICKS; t++) {
		float height_frac = (float)t / NUM_Y_TICKS;
		uint16_t tick_height = (uint16_t)(GRAPH_HEIGHT * height_frac);
		uint16_t tick_y = graph_y_to_lcd_y(tick_height);

		// Draw tick mark
		lcd_draw_horizontal_line(virtualBuffer, VIRTUAL_WIDTH_SIZE, hist_x_offset - 1, hist_x_offset, tick_y);

		if (tick_height != 0) {
			// Draw tick label
			int tick_val = (int)(hist.max_bin_height * height_frac);
			lcd_convert_int_to_char3x5_y_axis(virtualBuffer, VIRTUAL_WIDTH_SIZE, num_digits, tick_val, 0, tick_y - 2);
		}
	}

	// Draw x-axis ticks and labels
	for (int b = 0; b <= hist.num_bins; b++) {
		uint16_t x = hist_x_offset + b * bar_width;

		// Actual measurement value
		float val = hist.min_val + (b * hist.bin_width);

		// Draw tick label
		lcd_convert_int_to_char3x5_x_axis(virtualBuffer, VIRTUAL_WIDTH_SIZE, (int)val, x, 27);
	}

	free(hist.bin_counts);
	update_lcdBuffer();
}
