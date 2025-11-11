/*
 * openlog_sd.h
 *
 *  Created on: 6 Oct 2025
 *      Author: markh
 */

#ifndef OPENLOG_SD_H_
#define OPENLOG_SD_H_


#include "stm32f30x.h"




void init_uart(uint32_t baud);
void reset_datalogger(void);
void openlog_writechar(char c);
void openlog_writestring(char *s);
void openlog_writeline(char *s);

void reset(void);
void commandmode(void);
void openlog_append(const char* filename);
void log_data(uint8_t sample_start_nr,float data, const char* unit);
void delay(uint32_t ms);
#endif /* OPENLOG_SD_H_ */
