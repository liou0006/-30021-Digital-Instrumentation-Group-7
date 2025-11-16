#include "plot.h"

#include "fft.h"
#include "histogram.h"
#include "lcd_graphics.h"
#include "lcd.h"
#include "sensors.h"

#define NUM_SAMPLES 256

void plot_fft(sensor_t sensor, axis_t axis) {
	lsm9ds1_raw_data_t samples[NUM_SAMPLES];
	sensors_read_samples(samples, NUM_SAMPLES);

	// Compute FFT using samples
}

void plot_histogram(sensor_t sensor, axis_t axis) {
	// Get data
	lsm9ds1_raw_data_t samples[NUM_SAMPLES];
	sensors_read_samples(samples, NUM_SAMPLES);

	// Compute histogram
	histogram_result_t hist;
	compute_histogram(samples, NUM_SAMPLES, sensor, axis, 16, &hist);

	// Draw histogram bars on LCD
	lcd_clear_buffer(virtualBuffer, VIRTUAL_WIDTH_SIZE);

	// Plotting dimensions
	uint16_t usable_width = GRAPH_WIDTH;
	uint16_t usable_height = GRAPH_HEIGHT;

	for (uint16_t b = 0; b < hist.num_bins; b++) {
		// Horizontal scaling
		uint16_t x = GRAPH_X_OFFSET + (uint16_t)((b * usable_width) / hist.num_bins);

		// Vertical scaling
		uint16_t bar_height = (uint16_t)((hist.bin_counts[b] * usable_height) / hist.max_bin);

		// Convert to LCD coordinate system
		uint16_t y_start = graph_y_to_lcd_y(0);
		uint16_t y_end = graph_y_to_lcd_y(bar_height);

		// Draw vertical bar
		lcd_draw_vertical_line(virtualBuffer, VIRTUAL_WIDTH_SIZE, x, y_start, y_end);
	}

	free(hist.bin_counts);
	update_lcdBuffer();
}
