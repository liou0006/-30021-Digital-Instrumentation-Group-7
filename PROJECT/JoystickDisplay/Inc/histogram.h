#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "menu.h"

typedef struct {
	int16_t ax;
	int16_t ay;
	int16_t az;
	int16_t gx;
	int16_t gy;
	int16_t gz;
	int16_t mx;
	int16_t my;
	int16_t mz;
} LSM9DS1_RawData_t;

void calculate_and_print_histogram(LSM9DS1_RawData_t *data_array, int num_samples, sensor_t sensor, axis_t axis, int num_bins);
int16_t get_data_val(LSM9DS1_RawData_t *data_array, int i, sensor_t sensor, axis_t axis);
void display_histogram(sensor_t sensor, axis_t axis);

#endif /* HISTOGRAM_H_ */
