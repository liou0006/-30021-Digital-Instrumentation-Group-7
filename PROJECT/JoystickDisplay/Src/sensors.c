#include "sensors.h"

// Moving up and down x-axis
#define NUM_RAW_SAMPLES 49
lsm9ds1_raw_data_t dummy_data[NUM_RAW_SAMPLES] = {
	{46,     42,     -2,     12836,  -346,   -9652,  439,    1022,   -193,   14},
	{44,     45,     0,      13003,  -329,   -9569,  444,    1026,   -173,   14},
	{46,     43,     4,      12843,  -400,   -9643,  443,    1027,   -162,   14},
	{41,     47,     -1,     12920,  -445,   -9715,  443,    1033,   -154,   14},
	{47,     47,     1,      12893,  -372,   -9692,  442,    1028,   -160,   14},
	{44,     51,     3,      12853,  -399,   -9659,  431,    1033,   -179,   14},
	{46,     48,     2,      12960,  -420,   -9721,  446,    1020,   -173,   14},
	{51,     48,     3,      12814,  -419,   -9612,  432,    1035,   -164,   14},
	{45,     52,     3,      13028,  -375,   -9830,  440,    1032,   -156,   14},
	{45,     36,     -3,     12865,  -399,   -9584,  438,    1027,   -165,   14},
	{40,     54,     0,      12834,  -410,   -9641,  446,    1034,   -165,   14},
	{50,     48,     -3,     12920,  -394,   -9690,  428,    1024,   -176,   14},
	{47,     50,     1,      12903,  -389,   -9738,  438,    1029,   -159,   14},
	{44,     44,     -1,     12939,  -365,   -9694,  442,    1023,   -189,   14},
	{47,     46,     2,      12837,  -394,   -9663,  434,    1029,   -156,   14},
	{45,     47,     1,      12866,  -444,   -9691,  433,    1037,   -169,   14},
	{48,     50,     1,      12875,  -414,   -9677,  429,    1029,   -159,   14},
	{42,     41,     1,      12816,  -386,   -9657,  434,    1030,   -168,   14},
	{45,     47,     2,      12865,  -375,   -9674,  446,    1026,   -174,   14},
	{46,     44,     -3,     12860,  -445,   -9645,  450,    1027,   -169,   14},
	{56,     47,     -1,     12878,  -410,   -9718,  446,    1010,   -183,   14},
	{45,     44,     1,      12919,  -403,   -9703,  441,    1029,   -158,   14},
	{45,     50,     0,      12883,  -384,   -9681,  431,    1032,   -170,   14},
	{48,     50,     0,      12910,  -384,   -9713,  430,    1020,   -167,   14},
	{47,     48,     3,      12927,  -396,   -9687,  433,    1035,   -165,   14},
	{50,     48,     4,      12922,  -418,   -9692,  431,    1019,   -183,   14},
	{49,     50,     2,      12883,  -409,   -9689,  424,    1026,   -170,   14},
	{48,     43,     2,      12910,  -432,   -9688,  438,    1024,   -178,   14},
	{49,     45,     3,      12865,  -403,   -9732,  430,    1027,   -171,   14},
	{46,     46,     2,      12861,  -396,   -9700,  442,    1021,   -171,   14},
	{47,     44,     2,      12843,  -382,   -9692,  434,    1015,   -181,   14},
	{49,     49,     2,      12901,  -405,   -9707,  440,    1034,   -173,   14},
	{48,     49,     5,      12872,  -416,   -9698,  430,    1021,   -175,   14},
	{51,     49,     3,      12874,  -367,   -9686,  433,    1017,   -160,   14},
	{45,     47,     2,      12923,  -425,   -9671,  434,    1026,   -183,   14},
	{53,     50,     3,      12857,  -449,   -9629,  435,    1020,   -177,   14},
	{55,     56,     4,      12940,  -389,   -9759,  441,    1015,   -176,   14},
	{49,     49,     1,      12877,  -420,   -9672,  441,    1029,   -180,   14},
	{44,     48,     4,      12931,  -413,   -9712,  430,    1030,   -168,   14},
	{50,     46,     -4,     12903,  -393,   -9681,  426,    1019,   -173,   14},
	{44,     46,     -1,     12900,  -394,   -9680,  429,    1012,   -172,   14},
	{45,     47,     2,      12924,  -411,   -9676,  436,    1031,   -179,   14},
	{48,     48,     1,      12864,  -410,   -9740,  430,    1015,   -178,   14},
	{48,     47,     1,      12882,  -455,   -9738,  440,    1019,   -186,   14},
	{49,     47,     1,      12874,  -375,   -9648,  440,    1018,   -162,   14},
	{50,     50,     3,      12912,  -389,   -9700,  427,    1018,   -166,   14},
	{47,     49,     -3,     12917,  -397,   -9690,  437,    1033,   -185,   14},
	{46,     45,     2,      12884,  -401,   -9734,  445,    1025,   -170,   14},
	{49,     44,     -5,     12936,  -433,   -9687,  447,    1020,   -192,   14}
};

void sensors_read_samples(lsm9ds1_raw_data_t *data, int n_samples) {
	if (!data || n_samples <= 0) return;

	// Copy dummy_data into data, cycling if n_samples > NUM_RAW_SAMPLES
	for (int i = 0; i < n_samples; i++) {
		// Cycle through dummy data (we only copy the number of data samples we have into it.
		data[i] = dummy_data[i % NUM_RAW_SAMPLES];
	}
}


// int16_t has range -32768 to +32767 but we map to +/-32767
#define ACCEL_SCALE_FACTOR_G_PER_LSB (2.0f / 32767.0f)
#define GYRO_SCALE_FACTOR_DSP_PER_LSB (245.0f / 32767.0f)
#define MAGNET_SCALE_FACTOR_GAUSS_PER_LSB (4.0f / 32767.0f)

float get_scale_factor(sensor_t sensor) {
	if (sensor == SENSOR_ACCEL) {
		return ACCEL_SCALE_FACTOR_G_PER_LSB;
	} else if (sensor == SENSOR_GYRO) {
		return GYRO_SCALE_FACTOR_DSP_PER_LSB;
	} else if (sensor == SENSOR_MAGNET) {
		return MAGNET_SCALE_FACTOR_GAUSS_PER_LSB;
	}

	// Should be done smarter but just to display values we do it hardcoded
//	if (sensor == SENSOR_ACCEL) {
//		return ACCEL_SCALE_FACTOR_G_PER_LSB * 1000000.0f;
//	} else if (sensor == SENSOR_GYRO) {
//		return GYRO_SCALE_FACTOR_DSP_PER_LSB * 1000.0f;
//	} else if (sensor == SENSOR_MAGNET) {
//		return MAGNET_SCALE_FACTOR_GAUSS_PER_LSB * 1000000.0f;
//	}

	return 1.0f;	// Default no scale
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
