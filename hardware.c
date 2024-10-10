/**
 * @file hardware.c
 * @brief Hardware interface for the Clock Application.
 *
 * @author Alejandro Barranco-Leyte
 */

#include <stdio.h>
#include <stdlib.h>

#include "hardware.h"

/**
 * This function dynamically allocates memory for a Time structure
 * and initializes it with an initial value representing the time.
 */
Time* readTimeFromMemory() {
	Time* time = (Time*)malloc(sizeof(Time));
	if (time != NULL) {
		time->sevenSeg[0] = 1; // First hour digit
		time->sevenSeg[1] = 2; // Second hour digit
		time->sevenSeg[2] = 0; // First minute digit
		time->sevenSeg[3] = 0; // Second minute digit
	}
	return time; // Return pointer to Time
}

/**
 * This function prints the time to the console.
 */
int displayTime(const Time* time) {
	printf("displayTime(const Time* time) -> %d%d:%d%d\n",
			time->sevenSeg[0], time->sevenSeg[1], time->sevenSeg[2], time->sevenSeg[3]);

	return 0;
}

int readButtons() {
	// Will read hardware push buttons states
	printf("readButtons()\n");
	return 0;
}

void flashDigit(int currIndex) {
	printf("flashDigit(int currIndex) -> Current digit flashing: %d\n", currIndex);
}
