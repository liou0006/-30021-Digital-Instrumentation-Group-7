#ifndef WINDOW_H_
#define WINDOW_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>
#include <string.h>

/* Exported functions ------------------------------------------------------- */
void clrscr(void);
void initStopWatchWindow();
void printTitleBar(const char *title, int width);

#endif /* WINDOW_H_ */
