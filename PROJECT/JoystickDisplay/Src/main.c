#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "interrupts.h"
#include "timer.h"
#include "lcd.h"
#include "lcd_graphics.h"
#include "adc.h"

int main(void) {
	uart_init( 9600 );	// Initialize USB serial at 9600 baud

	initJoystick();		// Enabling GPIO pins for joystick
	initLed();			// Enabling GPIO pins for LED
	initTimer();
	init_spi_lcd();		// Initialize SPI for LCD
	ADC_setup_PA();		// Enabling GPIO pins for ADC

	// Initialize and clear buffer
	static uint8_t lcdBuffer[LCD_BUFF_SIZE];
	lcd_clear_buffer(lcdBuffer);

	while(1) {
		static float pa0_f = 0, pa1_f = 0;
		const float alpha = 0.1;	// Smoothing factor, smaller means smoother

		static uint8_t last_deci = 255;
		uint8_t current_deci = timeData.hundredths / 10;	// Update every 0.1 s
		if (current_deci != last_deci) {
			last_deci = current_deci;

			pa0_f = pa0_f * (1 - alpha) + alpha * ADC_measure_PA(1);
			pa1_f = pa1_f * (1 - alpha) + alpha * ADC_measure_PA(2);

			// Convert read ADC value to string
			char line0[24], line1[24];
			sprintf(line0, "PA0: %4u", (uint16_t)pa0_f);
			sprintf(line1, "PA1: %4u", (uint16_t)pa1_f);

			// Write strings to LCD buffer
			lcd_write_string((uint8_t *)line0, lcdBuffer, 0, 1);
			lcd_write_string((uint8_t *)line1, lcdBuffer, 0, 2);

			// Push buffer to LCD
			lcd_push_buffer(lcdBuffer);
		}
	}





//	initJoystick();		// Enabling GPIO pins for joystick
//	initLed();			// Enabling GPIO pins for LED
//	initTimer();
//	init_spi_lcd();		// Initialize SPI for LCD
//	ADC_setup_PA();		// Enabling GPIO pins for ADC
//
//	// Initialize and clear buffer
//	static uint8_t lcdBuffer[LCD_BUFF_SIZE];
//	lcd_clear_buffer(lcdBuffer);
//
//	while(1) {
//		static float pa0_f = 0, pa1_f = 0;
//		const float alpha = 0.1;	// Smoothing factor, smaller means smoother
//
//		static uint8_t last_deci = 255;
//		uint8_t current_deci = timeData.hundredths / 10;	// Update every 0.1 s
////		uint8_t current_deci = timeData.hundredths / 50;	// Update every 0.5 s
//		if (current_deci != last_deci) {
//			last_deci = current_deci;
//
//			uint16_t pa0 = ADC_measure_PA(1);
//			uint16_t pa1 = ADC_measure_PA(2);
////			uint16_t pa0 = ADC_measure_PA_avg(1, 8);
////			uint16_t pa1 = ADC_measure_PA_avg(2, 8);
//
//			pa0_f = pa0_f * (1 - alpha) + alpha * pa0;
//			pa1_f = pa1_f * (1 - alpha) + alpha * pa1;
//
//			char line0[24], line1[24];
//
//			sprintf(line0, "PA0: %4u", (uint16_t)pa0_f);
//			sprintf(line1, "PA1: %4u", (uint16_t)pa1_f);
//
//			lcd_write_string((uint8_t *)line0, lcdBuffer, 0, 1);
//			lcd_write_string((uint8_t *)line1, lcdBuffer, 0, 2);
//
//			// Push buffer to LCD
//			lcd_push_buffer(lcdBuffer);
//		}
//	}


//	float value;	// sensor reading
//	float min = -2.0;
//	float max = -2.0;
//
//
//	// Initialize SPI for LCD
//	init_spi_lcd();
//
//	// Initialize and clear buffer
//	static uint8_t buffer[LCD_BUFF_SIZE];	// 512 bytes
//	lcd_clear_buffer(buffer);
//
//	lcd_push_buffer(buffer);
//
//	while(1) {
//		int bin_index = (int)((value - min) / (max - min) * NUM_BINS);
//		if (bin_index < 0) bin_index = 0;
//		if (bin_index >= NUM_BINS) bin_index = NUM_BINS - 1;
//
//		histogram[bin_index]++;
//	}


	// ===== BASIC JOYSTICK FUNCTIONALITY WITH LED =====
//	initLed();
//	initJoystick();
//
//	setLed('d');
//
//	while(1) {
//		handleJoystick();
//	}

	// ================== LCD DISPLAY ==================
//	static uint8_t lcdBuffer [ LCD_BUFF_SIZE ];	// Frame buffer for LCD
//
//	// Initialize SPI + GPIOs and reset LCD
//	init_spi_lcd();
//
//	// Clear buffer
//	memset(lcdBuffer, 0x00, LCD_BUFF_SIZE);
//
//	// Write "Hello" at x=0, y=0 on LCD
//	lcd_write_string((uint8_t *)"Hello", lcdBuffer, 0, 0);
//
//	// Push buffer content to LCD
//	lcd_push_buffer(lcdBuffer);
//
//	while(1);


	// ================== LCD GRAPHICS =================

//	// Initialize SPI for LCD
//	init_spi_lcd();
//
//	// Initialize and clear buffer
//	static uint8_t buffer[LCD_BUFF_SIZE];	// 512 bytes
//	lcd_clear_buffer(buffer);

//	lcd_test_cosine_wave();

//	lcd_draw_horizontal_line(buffer, 2, 29, 10);
//	lcd_draw_axis(buffer);

//	lcd_draw_char3x5(buffer, 8, 27, '1');
//	lcd_draw_char3x5(buffer, 12, 27, '0');
//
//	lcd_draw_char3x5(buffer, 16, 25, '1');
//	lcd_draw_char3x5(buffer, 20, 25, '0');
//
//	lcd_draw_char3x5(buffer, 24, 22, '1');
//	lcd_draw_char3x5(buffer, 28, 22, '0');

//	lcd_draw_char3x5(buffer, 0, 27, '0');
//	lcd_draw_char3x5(buffer, 4, 27, '1');
//	lcd_draw_char3x5(buffer, 8, 27, '2');
//	lcd_draw_char3x5(buffer, 12, 27, '3');
//	lcd_draw_char3x5(buffer, 16, 27, '4');
//	lcd_draw_char3x5(buffer, 20, 27, '5');
//	lcd_draw_char3x5(buffer, 24, 27, '6');
//	lcd_draw_char3x5(buffer, 28, 27, '7');
//	lcd_draw_char3x5(buffer, 32, 27, '8');
//	lcd_draw_char3x5(buffer, 36, 27, '9');
//
//	lcd_push_buffer(buffer);

//	while(1);
}
