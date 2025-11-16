#include "histogram.h"

#include <stdint.h>
#include <limits.h>
#include <string.h>

void compute_histogram(lsm9ds1_raw_data_t *data, int num_samples,
		sensor_t sensor, axis_t axis, int num_bins,
		histogram_result_t *result) {
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
	int max_bin = 0;
	for (int i = 0; i < result->num_bins; i++) {
		if (result->bin_counts[i] > max_bin) {
			result->max_bin = result->bin_counts[i];
		}
	}
	if (result->max_bin == 0) result->max_bin = 1;	// Prevent division with 0
}

//void calculate_and_print_histogram(lsm9ds1_raw_data_t *data, int num_samples, sensor_t sensor, axis_t axis, int num_bins) {
//	int16_t min_val = INT16_MAX;
//	int16_t max_val = INT16_MIN;
//
//	// Find the min and max values for the chosen axis
//	for (int i = 0; i < num_samples; i++) {
//		int16_t val = get_data_val(data, i, sensor, axis);
//		if (val < min_val) min_val = val;
//		if (val > max_val) max_val = val;
//	}
//
//	// Prevent division with zero if all data is the same
//	if (min_val == max_val) return;
//
//	// Setup bins
//	int bin_counts[num_bins];
//	for (int i = 0; i < num_bins; i++) {
//		bin_counts[i] = 0;	// Initialize all counts to zero
//	}
//
//	// Calculate the width of each bin
//	float range = (float)(max_val - min_val);
//	float bin_width = (range + 1.0f) / num_bins;
//
//	// Populate bins
//	for (int i = 0; i < num_samples; i++) {
//		int16_t val = get_data_val(data, i, sensor, axis);
//
//		// Calculate the bin index for this value
//		int bin_index = (int)((val - min_val) / bin_width);
//
//		// Add to the bin
//		if (bin_index >= 0 && bin_index < num_bins) bin_counts[bin_index]++;
//	}
//
//	// 4. Print the histogram data (this is the part you send to your plotter)
////	printf("--- Histogram Results ---\n");
////	printf("Axis: %d, Min: %d, Max: %d, Samples: %d\n", axis, min_val, max_val, num_samples);
////	printf("Bin Width: %.2f\n", bin_width);
////	printf("\nBin Range \t\t| Count\n");
////	printf("----------------------------------\n");
//
//	for (int i = 0; i < num_bins; i++) {
//		float bin_start = min_val + (i * bin_width);
//		float bin_end = bin_start + bin_width;
//
//		// Print the bin range and the count
////		printf("[%.0f to %.0f)\t| %d\t", bin_start, bin_end, bin_counts[i]);
//
//		// Bonus: Print a simple text-based bar
////		for(int j = 0; j < bin_counts[i]; j++) {
////			printf("*");
////		}
////		printf("\n");
//	}
//}
//


