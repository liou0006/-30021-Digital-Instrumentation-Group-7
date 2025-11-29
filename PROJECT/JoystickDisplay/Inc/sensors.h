#ifndef SENSORS_H_
#define SENSORS_H_

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course

#include <stdint.h>

// LSM9DS1 sampling frequency
#define SAMPLING_FREQ_HZ 200.0f	// NEEDS TO BE UPDATED WITH ACTUAL SAMPLING FREQ!

typedef enum {
	SENSOR_ACCEL,
	SENSOR_GYRO,
	SENSOR_MAGNET
} sensor_t;

typedef enum {
	AXIS_X,
	AXIS_Y,
	AXIS_Z
} axis_t;

// Data struct for raw LSM9DS1 readings
typedef struct {
	int16_t gx, gy, gz;
	int16_t ax, ay, az;
	int16_t mx, my, mz;
	int16_t T;
} lsm9ds1_raw_data_t;

void sensors_read_samples(lsm9ds1_raw_data_t *data, int n_samples);
int16_t get_data_val(lsm9ds1_raw_data_t *data, int i, sensor_t sensor,
		axis_t axis);

#endif /* SENSORS_H_ */
