#include "histogram.h"

#include <stdint.h>
#include <limits.h>
#include <string.h>

void compute_histogram(lsm9ds1_raw_data_t *data, int num_samples,
		sensor_t sensor, axis_t axis, int num_bins, histogram_result_t *result) {
	int16_t min_val = INT16_MAX;
	int16_t max_val = INT16_MIN;

	// Find the min and max values for the chosen axis
	for (int i = 0; i < num_samples; i++) {
		int16_t val = get_data_val(data, i, sensor, axis);
		if (val < min_val) min_val = val;
		if (val > max_val) max_val = val;
	}

	result->min_val = min_val;
	result->max_val = max_val;
	result->num_bins = num_bins;
	result->bin_width = (float)(max_val - min_val + 1) / num_bins;

	// Allocate bin counts
	result->bin_counts = calloc(num_bins, sizeof(int));

	// Fill bins
	for (int i = 0; i < num_samples; i++) {
		int16_t val = get_data_val(data, i, sensor, axis);
		int index = (int)((val - min_val) / result->bin_width);
		if (index >= 0 && index < num_bins) result->bin_counts[index]++;
	}

	// Find maximum bin count for y-scaling
	result->max_bin = 0;
	for (int i = 0; i < result->num_bins; i++) {
		if (result->bin_counts[i] > result->max_bin) {
			result->max_bin = result->bin_counts[i];
		}
	}
	if (result->max_bin == 0) result->max_bin = 1;	// Prevent division with 0
}


