#include "histogram.h"
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

// Compare function for qsort
static int compare_float(const void *a, const void *b) {
	float va = *(const float*)a;
	float vb = *(const float*)b;
	if (va > vb) return 1;
	if (va < vb) return -1;
	return 0;
}

// Computes histogram from the range of min and max bin height value
void compute_histogram(lsm9ds1_raw_data_t *data, int num_samples,
		sensor_t sensor, axis_t axis, int num_bins, histogram_result_t *result) {

	if (num_samples < 2) return;

	// Gyroscope sensor z-axis data values are very big and makes the
	// text overlap for num_bins = 16
	if (sensor == SENSOR_GYRO && axis == AXIS_Z) num_bins -= 2;

	// ---------- Data extraction ----------
	// Find scale factor based on sensor type
	float scale_factor = get_scale_factor(sensor);

	// Extract data for chosen sensor and axis
	float *vals = malloc(num_samples * sizeof(float));
	for (int i = 0; i < num_samples; i++) {
		vals[i] = (float)get_data_val(data, i, sensor, axis) * scale_factor;
	}

	// Sort using qsort
	qsort(vals, num_samples, sizeof(float), compare_float);

	// Percentile-based min/max
	float lower_p = 0.05f;
	float upper_p = 0.95f;

	int idx_min = (int)(num_samples * lower_p);
	int idx_max = (int)(num_samples * upper_p);

	if (idx_min < 0) idx_min = 0;
	if (idx_max >= num_samples) idx_max = num_samples - 1;

	float min_val = vals[idx_min];
	float max_val = vals[idx_max];

	free(vals);

	result->min_val = min_val;
	result->max_val = max_val;
	result->num_bins = num_bins;

	// Ensure range is not zero
	float range = max_val - min_val;
	if (range <= 0.0f) {
		// Prevent division by zero by forcing a small range
		range = 10.0f * scale_factor;
	}
	result->bin_width = range / num_bins;

	// Allocate histogram bins
	result->bin_counts = calloc(num_bins, sizeof(int));

	// Fill bins (outliers go into edge bins)
	for (int i = 0; i < num_samples; i++) {
		float v = (float)get_data_val(data, i, sensor, axis) * scale_factor;

		int index;
		if (v <= min_val) index = 0;
		else if (v >= max_val) index = num_bins - 1;
		else index = (int)floorf((v - min_val) / result->bin_width);

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
