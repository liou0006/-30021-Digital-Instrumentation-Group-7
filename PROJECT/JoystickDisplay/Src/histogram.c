#include "histogram.h"

#include <stdint.h>
#include <limits.h>
#include <string.h>

/*
 * Compare function for qsort
 */
static int compare_int16(const void *a, const void *b) {
	int16_t va = *(const int16_t*)a;
	int16_t vb = *(const int16_t*)b;
	return (va > vb) - (va < vb);
}

/*
 * Computes histogram from the range of min and max bin height value.
 */
void compute_histogram(lsm9ds1_raw_data_t *data, int num_samples,
		sensor_t sensor, axis_t axis, int num_bins, histogram_result_t *result) {

	if (num_samples < 2) return;

	// Gyroscope sensor z-axis data values are very big and makes the
	// text overlap for num_bins = 16
	if (sensor == SENSOR_GYRO && axis == AXIS_Z) num_bins -= 2;

	// Extract values
	int16_t *vals = malloc(num_samples * sizeof(int16_t));
	for (int i = 0; i < num_samples; i++) {
		vals[i] = get_data_val(data, i, sensor, axis);
	}

	// Sort using qsort
	qsort(vals, num_samples, sizeof(int16_t), compare_int16);

	// Percentile-based min/max
	float lower_p = 0.05f;
	float upper_p = 0.95f;

	int idx_min = (int)(num_samples * lower_p);
	int idx_max = (int)(num_samples * upper_p);

	if (idx_min < 0) idx_min = 0;
	if (idx_max >= num_samples) idx_max = num_samples - 1;

	int16_t min_val = vals[idx_min];
	int16_t max_val = vals[idx_max];

	free(vals);

	result->min_val = min_val;
	result->max_val = max_val;
	result->num_bins = num_bins;
	result->bin_width = (float)(max_val - min_val + 1) / num_bins;

	// Allocate histogram bins
	result->bin_counts = calloc(num_bins, sizeof(int));

	// Fill bins (outliers go into edge bins)
	for (int i = 0; i < num_samples; i++) {
		int16_t v = get_data_val(data, i, sensor, axis);

		int index;

		if (v <= min_val)
			index = 0;
		else if (v >= max_val)
			index = num_bins - 1;
		else
			index = (int)((v - min_val) / result->bin_width);

		if (index < 0) index = 0;
		if (index >= num_bins) index = num_bins - 1;

		result->bin_counts[index]++;
	}

	// Find max bin height for y-scaling
	result->max_bin_height = 1;
	for (int i = 0; i < num_bins; i++) {
		if (result->bin_counts[i] > result->max_bin_height) {
			result->max_bin_height = result->bin_counts[i];
		}
	}
}
