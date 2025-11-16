#include "plot.h"

#include "fft.h"
#include "histogram.h"
//#include "adc.h"
#include "lcd_graphics.h"
#include "lcd.h"

static float rawSamples[256];
static float fftOut[256];
static float magnitude[128];

void plot_fft(sensor_t sensor, axis_t axis) {
	//
}

void plot_histogram(sensor_t sensor, axis_t axis) {
	//
}
