#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course

#include <stdlib.h>
#include "sensors.h"

#define HIST_MAX_BINS 128

typedef struct {
	int16_t min_val;
	int16_t max_val;
	float bin_width;
	int num_bins;
	int *bin_counts;
	int max_bin_height;
} histogram_result_t;

void compute_histogram(lsm9ds1_raw_data_t *data, int num_samples,
		sensor_t sensor, axis_t axis, int num_bins, histogram_result_t *result);

#endif /* HISTOGRAM_H_ */
