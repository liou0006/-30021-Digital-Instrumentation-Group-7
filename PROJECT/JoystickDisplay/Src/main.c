#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
//#include "interrupts.h"
#include "lcd.h"
#include "lcd_graphics.h"
#include "adc.h"
#include "fft.h"
#include "menu.h"
#include "histogram.h"

#define N 256
#define HALF_N (N/2)

complex_t fftBuffer[N];
float magnitudes[HALF_N];

// Initialize LCD and virtual buffer
//uint8_t lcdBuffer[LCD_BUFF_SIZE];
//uint8_t virtualBuffer[VIRTUAL_WIDTH_SIZE * LCD_ROWS];

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
    {49, 11, 16444, 11, -35, -3, 513, -288, 1188}//,
//    {-2, 107, 16371, 15, -10, 4, 435, -43, 1142},
//    {100, 24, 16374, 27, 11, -1, 269, -128, 1223},
//    {54, -23, 16365, 22, -14, -1, 379, -272, 1250},
//    {12, 23, 16392, 10, 5, -4, 500, -151, 1175},
//    {79, 83, 16348, 34, -41, 1, 438, -281, 1048},
//    {76, 21, 16417, 17, -28, 14, 509, -192, 1118},
//    {38, 8, 16378, -1, -32, 8, 348, -155, 1238},
//    {63, 78, 16401, 23, 1, 3, 313, -72, 1202},
//    {78, 41, 16384, 15, -12, 1, 464, -140, 1193},
//    {21, 55, 16384, 33, -27, -10, 503, -228, 1266}
};

// compute mean of real parts
static float compute_mean_real(const complex_t *buf, int len) {
    float s = 0.0f;
    for (int i = 0; i < len; ++i) s += buf[i].real;
    return s / (float)len;
}

// apply Hann window in-place to real data (imag assumed zero)
static void apply_hann_window(complex_t *buf, int len) {
    for (int n = 0; n < len; ++n) {
        float w = 0.5f * (1.0f - cosf(2.0f * M_PI * n / (len - 1)));
        buf[n].real *= w;
        // buf[n].imag *= w; // imag is zero anyway
    }
}

// normalize magnitudes by N (or by N/2 depending desired amplitude convention)
static void normalize_magnitudes(float *mag, int half_n, int samples) {
    // divide by N to convert to per-sample amplitude
    for (int k = 0; k < half_n; ++k) {
        mag[k] = mag[k] / (float)samples;
    }
}

int main(void) {
	uart_init( 9600 );	// Initialize USB serial at 9600 baud

	printf("\n");

	// Clear buffer
	for (int i = 0; i < N; i++) {
		fftBuffer[i].real = 0.0f;
		fftBuffer[i].imag = 0.0f;
	}

	// Insert only the samples we have (here 15 samples)
	const int num_samples = 15;
	for (int i = 0; i < num_samples; i++) {
		fftBuffer[i].real = (float)dummy_data[i].az;
		fftBuffer[i].imag = 0.0f;
	}

	// Debug: print first 20 input values
	printf("Input values:\n");
	for (int n = 0; n < 20; ++n) {
		printf("%2d: %.2f\n", n, fftBuffer[n].real);
	}

    // 3) Subtract mean (remove DC)
    float mean = compute_mean_real(fftBuffer, num_samples); // compute mean only of actual samples
    for (int i = 0; i < num_samples; ++i) {
        fftBuffer[i].real -= mean;
    }

    // 4) Apply Hann window to the samples (reduce leakage)
    apply_hann_window(fftBuffer, num_samples);
    // remainder up to N are zeros (window multiplication ok)

    // Compute FFT and magnitudes
	fft256(fftBuffer);
	fft256_magnitude(fftBuffer, magnitudes);

    // 6) Normalize magnitudes by N
    normalize_magnitudes(magnitudes, HALF_N, N);

	// Plot magnitudes on LCD
	for (int n = 0; n < 20; n++) {
		printf("|X[%d]| = %.6f\n", n, magnitudes[n]);
	}
//	printf("\n");

	while(1) {}
}
