#include "histogram.h"
#include "stdint.h"
#include "limits.h"

void calculate_and_print_histogram(LSM9DS1_RawData_t *data_array, int num_samples, sensor_t sensor, axis_t axis, int num_bins) {
	int16_t min_val = INT16_MAX;
	int16_t max_val = INT16_MIN;

	// Find the min and max values for the chosen axis
	for (int i = 0; i < num_samples; i++) {
		int16_t val = get_data_val(data_array, i, sensor, axis);
		if (val < min_val) min_val = val;
		if (val > max_val) max_val = val;
	}

	// Prevent division with zero if all data is the same
	if (min_val == max_val) return;

	// Setup bins
	int bin_counts[num_bins];
	for (int i = 0; i < num_bins; i++) {
		bin_counts[i] = 0;	// Initialize all counts to zero
	}

	// Calculate the width of each bin
	float range = (float)(max_val - min_val);
	float bin_width = (range + 1.0f) / num_bins;

	// Populate bins
	for (int i = 0; i < num_samples; i++) {
		int16_t val = get_data_val(data_array, i, sensor, axis);

		// Calculate the bin index for this value
		int bin_index = (int)((val - min_val) / bin_width);

		// Add to the bin
		if (bin_index >= 0 && bin_index < num_bins) bin_counts[bin_index]++;
	}

	// 4. Print the histogram data (this is the part you send to your plotter)
	printf("--- Histogram Results ---\n");
	printf("Axis: %d, Min: %d, Max: %d, Samples: %d\n", axis, min_val, max_val, num_samples);
	printf("Bin Width: %.2f\n", bin_width);
	printf("\nBin Range \t\t| Count\n");
	printf("----------------------------------\n");

	for (int i = 0; i < num_bins; i++) {
		float bin_start = min_val + (i * bin_width);
		float bin_end = bin_start + bin_width;

		// Print the bin range and the count
		printf("[%.0f to %.0f)\t| %d\t", bin_start, bin_end, bin_counts[i]);

		// Bonus: Print a simple text-based bar
		for(int j = 0; j < bin_counts[i]; j++) {
			printf("*");
		}
		printf("\n");
	}
}

int16_t get_data_val(LSM9DS1_RawData_t *data_array, int i, sensor_t sensor, axis_t axis) {
	int16_t val;

	switch (axis) {
	case AXIS_X:
		if (sensor == SENSOR_ACCEL) val = data_array[i].ax;
		else if (sensor == SENSOR_GYRO) val = data_array[i].gx;
		else val = data_array[i].mx;
		break;

	case AXIS_Y:
		if (sensor == SENSOR_ACCEL) val = data_array[i].ay;
		else if (sensor == SENSOR_GYRO) val = data_array[i].gy;
		else val = data_array[i].my;
		break;

	case AXIS_Z:
		if (sensor == SENSOR_ACCEL) val = data_array[i].az;
		else if (sensor == SENSOR_GYRO) val = data_array[i].gz;
		else val = data_array[i].mz;
		break;
	}

	return val;
}

void display_histogram(sensor_t sensor, axis_t axis) {
	//
}

