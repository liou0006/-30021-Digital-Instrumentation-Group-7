#ifndef SENSORS_H_
#define SENSORS_H_

#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course

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

#endif /* SENSORS_H_ */
