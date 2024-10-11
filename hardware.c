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
		writeTo7Seg(time, 0, 1); // First hour digit
		writeTo7Seg(time, 1, 2); // Second hour digit
		writeTo7Seg(time, 2, 0); // First minute digit
		writeTo7Seg(time, 3, 0); // Second minute digit
	}

	// Return pointer to Time
	return time;
}

/**
 * This function prints the time to the console.
 */
int displayTime(const Time* time) {
	printf("displayTime(const Time* time) -> %d%d:%d%d\n",
			readFrom7Seg(time, 0), readFrom7Seg(time, 1),
			readFrom7Seg(time, 2), readFrom7Seg(time, 3));

	// Return some status code
	return 0;
}

int readButtons() {
	// Will read hardware push buttons states
	printf("readButtons()\n");

	// Return some status code
	return 0;
}

void flashDigit(int currIndex) {
	// Implement logic to make digit at current index flash
	printf("flashDigit(int currIndex) -> Current digit flashing: %d\n", currIndex);
}

int saveChanges(const Time* time) {
	// Implement saving time to memory
	printf("saveChanges(const Time* time) -> Saving changes...\n");

	// Return some status code
	return 0;
}

int increment7Seg(Time* time, int currIndex) {

	// Return some status code
	return 0;
}

int writeTo7Seg(Time* time, int currIndex, int value) {
	// Implement some logic to write to 7 segment display
	time->sevenSeg[currIndex] = value;

	// Return some status code
	return 0;
}

int readFrom7Seg(const Time* time, int currIndex) {
	// Return decimal value of the current 7 segment display
	return time->sevenSeg[currIndex];
}
