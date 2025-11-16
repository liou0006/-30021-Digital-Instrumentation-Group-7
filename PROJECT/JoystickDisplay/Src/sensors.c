#include "sensors.h"

//#include <string.h>

// Dummy sample data for sensor at rest (25 samles)
lsm9ds1_raw_data_t dummy_data[25] = {
    {64, 56, 16410, 24, -13, 11, 438, -314, 1276},
    {-13, 63, 16390, 4, -11, 10, 361, -184, 1243},
    {57, 118, 16335, 46, -3, 1, 440, -182, 1263},
    {82, 3, 16428, 24, -33, -2, 456, -190, 1322},
    {43, -8, 16382, 21, -14, 13, 546, -248, 1144},
    {63, 51, 16373, 22, -1, 2, 367, -231, 1143},
    {50, 75, 16394, -20, -24, -11, 548, -137, 1316},
    {-1, 101, 16402, 32, -20, -7, 364, -220, 1312},
    {30, 73, 16345, 16, -23, -7, 388, -32, 1098},
    {70, 8, 16423, 19, 4, -14, 480, -218, 1075},
    {61, 19, 16353, 23, 5, 10, 517, -226, 1121},
    {51, -17, 16361, 28, -19, -6, 432, -193, 1248},
    {27, 33, 16380, 31, -21, -1, 402, -173, 1232},
    {38, 71, 16330, 22, -33, 12, 393, -158, 1181},
    {49, 11, 16444, 11, -35, -3, 513, -288, 1188},
    {-2, 107, 16371, 15, -10, 4, 435, -43, 1142},
    {100, 24, 16374, 27, 11, -1, 269, -128, 1223},
    {54, -23, 16365, 22, -14, -1, 379, -272, 1250},
    {12, 23, 16392, 10, 5, -4, 500, -151, 1175},
    {79, 83, 16348, 34, -41, 1, 438, -281, 1048},
    {76, 21, 16417, 17, -28, 14, 509, -192, 1118},
    {38, 8, 16378, -1, -32, 8, 348, -155, 1238},
    {63, 78, 16401, 23, 1, 3, 313, -72, 1202},
    {78, 41, 16384, 15, -12, 1, 464, -140, 1193},
    {21, 55, 16384, 33, -27, -10, 503, -228, 1266}
};

void sensors_read_samples(lsm9ds1_raw_data_t *data, int n_samples) {
	if (!data || n_samples <= 0) return;

	// Copy dummy_data into data, cycling if n_samples > 25 (maybe implement some stop later if we don't want that)
	for (int i = 0; i < n_samples; i++) {
		// Cycle through dummy data (we only copy the number of data samples we have into it.
		data[i] = dummy_data[i % 25];
	}
}

int16_t get_data_val(lsm9ds1_raw_data_t *data, int i, sensor_t sensor,
		axis_t axis) {
	int16_t val;

	switch (axis) {
	case AXIS_X:
		if (sensor == SENSOR_ACCEL) val = data[i].ax;
		else if (sensor == SENSOR_GYRO) val = data[i].gx;
		else val = data[i].mx;
		break;

	case AXIS_Y:
		if (sensor == SENSOR_ACCEL) val = data[i].ay;
		else if (sensor == SENSOR_GYRO) val = data[i].gy;
		else val = data[i].my;
		break;

	case AXIS_Z:
		if (sensor == SENSOR_ACCEL) val = data[i].az;
		else if (sensor == SENSOR_GYRO) val = data[i].gz;
		else val = data[i].mz;
		break;
	}

	return val;
}
