#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "sensors.h"

typedef struct {
	int16_t ax, ay, az;
	int16_t gx, gy, gz;
	int16_t mx, my, mz;
} lsm9ds1_raw_data_t;

void calculate_and_print_histogram(lsm9ds1_raw_data_t *data_array,
		int num_samples, sensor_t sensor, axis_t axis, int num_bins);
int16_t get_data_val(lsm9ds1_raw_data_t *data_array, int i,
		sensor_t sensor, axis_t axis);
void display_histogram(sensor_t sensor, axis_t axis);

#endif /* HISTOGRAM_H_ */
