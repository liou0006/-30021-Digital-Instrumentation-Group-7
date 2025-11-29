#include "menu.h"

#include "joystick.h"
#include "lcd.h"
#include "lcd_graphics.h"

// Current menu
static menu_state_t currentMenu = MENU_MAIN;
static uint8_t sel = 0;			// Selected line
static uint8_t joystick = 0;	// Joystick state (default is none)

// Remember cursor position for each menu
static uint8_t sel_main = 0;
static uint8_t sel_sensor = 0;
static uint8_t sel_axis = 0;

// Remember menu info
static sensor_t currentSensor;
static axis_t currentAxis;
static uint8_t FFTmode = 0;		// 1 = FFT, 0 = Histogram


uint8_t rxBufferSize = 20;
uint8_t rxBuffer[20]; // should be rxBufferSize
int16_t dataArray[10];
uint16_t sampleIndex = 0;
uint16_t maxData = 256; // ændre det til 256  (det kan ændres til NUM_SAMPLES som er defineret til 256)
lsm9ds1_raw_data_t lsmdata[256]; // should be maxData
uint8_t packet[20]; // should be IMU_PACKET_SIZE


void menu_init() {
	lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
	lcd_write_string((uint8_t *)"Main Menu:", lcdBuffer, 0, 0);
	lcd_write_string((uint8_t*)" FFT", lcdBuffer, 0, 1);
	lcd_write_string((uint8_t*)" Histogram", lcdBuffer, 0, 2);
}

void menu_update() {
	static uint8_t wait = 0;
	joystick = readJoystickState();

	if (wait) {
		if (joystick == 0) wait = 0;
		joystick = 0;
	}

	switch (currentMenu) {
	case MENU_MAIN:
		sel_main = 0;
		sel_sensor = 0;
		sel_axis = 0;

		if (joystick == UP && sel > 0) {
			sel--;
			wait = 1;
		} else if (joystick == DOWN && sel < 2) {
			sel++;
			wait = 1;
		} else if (joystick == CENTER) {
			sel_main = sel;		// Save main menu cursor

			if (sel == 0) {
				currentMenu = MENU_COLLECT;
			} else if (sel == 1) {
				currentMenu = MENU_FFT;
				FFTmode = 1;
				sel = sel_sensor;	// Restore previous cursor in sensor menu
			} else {
				currentMenu = MENU_HIST;
				FFTmode = 0;
				sel = sel_sensor;	// Restore previous cursor in sensor menu
			}
			wait = 1;
		}
		lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
		lcd_write_string((uint8_t *)"Main Menu:", lcdBuffer, 0, 0);
		lcd_write_string((uint8_t*)(sel == 0 ? ">Collect" : " Collect"), lcdBuffer, 0, 1);
		lcd_write_string((uint8_t*)(sel == 1 ? ">FFT" : " FFT"), lcdBuffer, 0, 2);
		lcd_write_string((uint8_t*)(sel == 2 ? ">Histogram" : " Histogram"), lcdBuffer, 0, 3);
		break;

	case MENU_COLLECT:
		lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
		lcd_write_string((uint8_t *)"Collecting data...", lcdBuffer, 0, 0);
		// checks if CS pin is low
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_SET);
		//throws away bad signal
		while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == SET) {
			SPI_ReceiveData8(SPI3);
		}
		for (int i = 0; i < rxBufferSize; i++) {
			while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
			rxBuffer[i] = SPI_ReceiveData8(SPI3);
		}
		//checks if CS pin is high
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_RESET);



		// update code
		for (int i = 0; i < rxBufferSize / 2; i++) {

			uint8_t highByte = rxBuffer[i * 2];
			uint8_t lowByte  = rxBuffer[i * 2 + 1];

			dataArray[i] = (int16_t)((highByte << 8) | lowByte);
		}

		if (sampleIndex < maxData){
			lsmdata[sampleIndex].gx = dataArray[0];
			lsmdata[sampleIndex].gy = dataArray[1];
			lsmdata[sampleIndex].gz = dataArray[2];
			lsmdata[sampleIndex].ax = dataArray[3];
			lsmdata[sampleIndex].ay = dataArray[4];
			lsmdata[sampleIndex].az = dataArray[5];
			lsmdata[sampleIndex].mx = dataArray[6];
			lsmdata[sampleIndex].my = dataArray[7];
			lsmdata[sampleIndex].mz = dataArray[8];
			lsmdata[sampleIndex].T = dataArray[9];
			//printf("dataArray [0] %d\n", dataArray[0]);
			//printf("lsmData gx: %d\n\n", lsmdata[sampleIndex].gx);
			sensors_pack_raw(&lsmdata[sampleIndex], packet);
			//printf("Packet [0][1] %d , %d", packet[0], packet[1]);
			// Write to SD
			//				openlog_writebytes(packet, IMU_PACKET_SIZE);
			openlog_writebytes(packet, 20);
			sampleIndex++;
		} else if (sampleIndex == maxData){
			printf("Data sent\n");
			currentMenu = MENU_MAIN;
			sampleIndex = 0; // should be 0
			//reset_openlog();
		} else {
			printf("Error");
		}


		//		print on PuTTY
		//		printf("Rx: ");
		//		for (int i = 0; i < rxBufferSize / 2; i++) {
		//			printf("%d	", dataArray[i]);
		//		}
		//		printf("\n");
		break;

	case MENU_FFT:
	case MENU_HIST:
		sel_axis = 0;

		if (joystick == LEFT) {
			currentMenu = MENU_MAIN;
			sel = sel_main;		// Restore main menu cursor
			wait = 1;
		} else if (joystick == UP && sel > 0) {
			sel--;
			wait = 1;
		} else if (joystick == DOWN && sel < 2) {
			sel++;
			wait = 1;
		} else if (joystick == CENTER) {
			sel_sensor = sel;	// Save sensor selection cursor

			if (sel == 0) currentSensor = SENSOR_ACCEL;
			else if (sel == 1) currentSensor = SENSOR_GYRO;
			else currentSensor = SENSOR_MAGNET;

			currentMenu = MENU_AXIS;
			sel = sel_axis;		// Restore previous axis cursor
			wait = 1;
		}

		lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
		lcd_write_string((uint8_t*)(FFTmode ? "FFT: Select Sensor" : "Hist.: Select Sensor"), lcdBuffer, 0, 0);
		lcd_write_string((uint8_t *)(sel == 0 ? ">Accelerometer" : " Accelerometer"), lcdBuffer, 0, 1);
		lcd_write_string((uint8_t *)(sel == 1 ? ">Gyroscope" : " Gyroscope"), lcdBuffer, 0, 2);
		lcd_write_string((uint8_t *)(sel == 2 ? ">Magnetometer" : " Magnetometer"), lcdBuffer, 0, 3);
		break;

	case MENU_AXIS:
		if (joystick == LEFT) {
			currentMenu = (FFTmode ? MENU_FFT : MENU_HIST);
			sel = sel_sensor;	// Restore sensor cursor
			wait = 1;
		} else if (joystick == UP && sel > 0) {
			sel--;
			wait = 1;
		} else if (joystick == DOWN && sel < 2) {
			sel++;
			wait = 1;
		} else if (joystick == CENTER) {
			sel_axis = sel;	// Save axis cursor

			if (sel == 0) currentAxis = AXIS_X;
			else if (sel == 1) currentAxis = AXIS_Y;
			else currentAxis = AXIS_Z;

			currentMenu = MENU_PLOT;
			wait = 1;
		}

		lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);
		lcd_write_string((uint8_t*)(FFTmode ? "FFT: Select Axis" : "Hist.: Select Axis"), lcdBuffer, 0, 0);
		lcd_write_string((uint8_t*)(sel == 0 ? ">X" : " X"), lcdBuffer, 0, 1);
		lcd_write_string((uint8_t*)(sel == 1 ? ">Y" : " Y"), lcdBuffer, 0, 2);
		lcd_write_string((uint8_t*)(sel == 2 ? ">Z" : " Z"), lcdBuffer, 0, 3);
		break;

	case MENU_PLOT:
		lcd_clear_buffer(lcdBuffer, LCD_BUFF_SIZE);

		if (joystick == LEFT) {
			currentMenu = MENU_AXIS;
			sel = sel_axis;		// Restore axis cursor
			wait = 1;
		}

		lcd_clear_buffer(virtualBuffer, LCD_ROWS * VIRTUAL_WIDTH_SIZE);
		//		draw_new_axis();

		if (FFTmode) {
			//			plot_fft(currentSensor, currentAxis);
		} else {
			// y-axis labels not working debugging
//			lsm9ds1_raw_data_t samples[NUM_SAMPLES]; // changes
//			sensors_read_samples(samples, 49);
//			plot_histogram(samples, currentSensor, currentAxis);

			plot_histogram(lsmdata, currentSensor, currentAxis);
		}

		update_lcdBuffer();		// Copy visible window to physical LCD buffer
		break;
	}
}

