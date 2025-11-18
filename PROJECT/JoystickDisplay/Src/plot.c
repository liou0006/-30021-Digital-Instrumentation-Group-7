#include "plot.h"

#include "fft.h"
#include "histogram.h"
#include "lcd_graphics.h"
#include "lcd.h"
#include "sensors.h"

#define NUM_SAMPLES 256
#define GRAPH_HEIGHT (LCD_HEIGHT - GRAPH_MARGIN_TOP - GRAPH_MARGIN_BOTTOM)
#define GRAPH_WIDTH (VIRTUAL_WIDTH_SIZE - GRAPH_X_OFFSET - GRAPH_MARGIN_RIGHT)

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

	// ----- Draw histogram bars on LCD -----
	// Graph plotting dimensions
	uint16_t usable_width = GRAPH_WIDTH;
	uint16_t usable_height = GRAPH_HEIGHT;

	// Compute per bin width in pixels
	uint16_t bar_width = usable_width / hist.num_bins;
	if (bar_width < 1) bar_width = 1;

	for (uint16_t b = 0; b < hist.num_bins; b++) {
		// Compute x-coordinates
		uint16_t x_start = GRAPH_X_OFFSET + b * bar_width;
		uint16_t x_end = x_start + bar_width;

		// Compute scaled bar height
		uint16_t bar_height = (uint16_t)((hist.bin_counts[b] * usable_height) / hist.max_bin);

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

	draw_graph_axis();
	free(hist.bin_counts);
	update_lcdBuffer();
}
