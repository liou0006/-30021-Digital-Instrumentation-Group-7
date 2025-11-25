#ifndef PLOT_H_
#define PLOT_H_

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course

#include <stdlib.h>
#include "sensors.h"

void plot_fft(sensor_t sensor, axis_t axis);
//void plot_histogram(sensor_t sensor, axis_t axis);
void plot_histogram(lsm9ds1_raw_data_t* samples,sensor_t sensor, axis_t axis);

#endif /* PLOT_H_ */
