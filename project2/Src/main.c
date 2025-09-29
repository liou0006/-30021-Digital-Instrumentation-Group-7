#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "joystick.h"
#include "led.h"
#include "lcd.h"
#include "interrupt.h"
#include "timer.h"
#include "window.h"
#include "flash.h"
#include "adc.h"
#include "math.h"
#include "pwm.h"

//#define VREFINT_CAL *((uint16_t*) ((uint32_t) 0x1FFFF7BA)) //calibrated at 3.3V@ 30
//static uint8_t lcdBuffer[LCD_BUFF_SIZE];

int main(void) {
	uart_init( 9600 ); // Initialize USB serial at 9600 baud

	timer16_pwm_init();
	timer17_pwm_init();


	while (1)
	{
		update_servos(); // Update servos by reading the ADC for the pots
	}

	/*
	initJoystick();
	initLed();
	iniEXTIA4();
	initTimer();
	lcd_init_and_print();
	ADC_setup_PA();
	ADC_Setup_VREFEN();
	timer16_pwm_init();

	//setting initial duty cycle
	int dutyCycle = 128;
	setDutyCycle(dutyCycle);

	while(1) {

		static uint8_t lastDeci = 255;
		uint8_t curDeci = timeData.hundredths / 10;
		if (curDeci != lastDeci) {
			lastDeci = curDeci;

			uint16_t VREF = ADC_measure_VREF();
			float V_DDA = 3.3 * VREFINT_CAL / VREF;

			uint16_t pa0 = ADC_measure_PA(1);

			// when PWM has a duty cycle of 50 it switches between
			// PA0: 0 and 4080
			// V = 0V and 2.83V

			float Vch0 = (V_DDA *pa0)/ (4095);

			float V_desired = 1.0;
			float error = V_desired - Vch0;

			float kp = 20; //increase or decrease for faster response
			float cp = kp * error; // Proportional controller / might need to make a integral if we desire absolute 1V
			int newDutyCycle = dutyCycle + cp;

			// maybe set the boundary conditions to 5 for the lower and 250 for the upper
			//so the duty cycle doesn't die out suddenly

			if(newDutyCycle <= 0){
				newDutyCycle = 0;
			} else if (newDutyCycle >= 255){
				newDutyCycle = 255;
			}

			printf("V_meas=%.2f, Error=%.2f, Duty=%d\n", Vch0, error, dutyCycle);



			setDutyCycle(newDutyCycle);
			dutyCycle = newDutyCycle;

			char line0[24], line1[24], line2[24], line3[24];
//			sprintf(line0, "VRef: %4u", VREF);
//			sprintf(line1, "VDDA: %.2fV", V_DDA);
			sprintf(line2, "PA0: %4u", pa0);
			sprintf(line3, "V_meas: %.2f", Vch0);

//			lcd_write_string((uint8_t *)line0, lcdBuffer, 0, 0);
//			lcd_write_string((uint8_t *)line1, lcdBuffer, 0, 1);
			lcd_write_string((uint8_t *)line2, lcdBuffer, 0, 2);
			lcd_write_string((uint8_t *)line3, lcdBuffer, 0, 3);
			lcd_push_buffer(lcdBuffer);

			// for(uint32_t i = 0; i<5000000;i++); //delay function but not good if you want the system to use the software feedback

		}
	}
	*/
}
