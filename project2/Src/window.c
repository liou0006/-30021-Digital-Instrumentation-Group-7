/*
 * window.c
 *
 *  Created on: Sep 9, 2025
 *      Author: emma
 */

#include "window.h"

void clrscr(void) {
	/* ANSI escape sequence:
	 * 	\033[2J clears screen
	 * 	\033[H moves cursor to top left
	 */
	printf("\033[2J\033[H");
	fflush(stdout);
}

void initStopWatchWindow() {
	clrscr();

	printf("\033[37;40m");	// set terminal to white text on black background

	// Print title bar of width 30
	printTitleBar("Stop watch", 30);
}

void printTitleBar(const char *title, int width) {
	int padding = width - strlen(title);

	// Title with inverted colours
	printf("\033[7m %s", title);

	// Fill rest with spaces
	for (int i = 0; i < padding - 1; i++) {
		printf(" ");
	}

	// Reset the color (white text on black background)
	printf("\033[0m\n");
}
