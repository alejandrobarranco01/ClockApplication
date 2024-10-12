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
		writeTo7Seg(time, 1, 5); // Second hour digit
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

/**
 * This function will read inputs from the push buttons
 */
int readButtons() {
	// Will read hardware push buttons states
	printf("readButtons()\n");

	// Return some status code
	return 0;
}

/**
 * This function will create a flashing effect of the 7-segment display
 * at the current index.
 */
void flashDigit(int currIndex) {
	// Implement logic to make digit at current index flash
	printf("flashDigit(int currIndex) -> Current digit flashing: %d\n", currIndex);
}


/**
 * This function will save current time into memory.
 */
int saveChanges(const Time* time) {
	// Implement saving time to memory
	printf("saveChanges(const Time* time) -> Saving changes...\n");

	// Return some status code
	return 0;
}

/**
 * This function will increment the value at the current
 * display, making sure that bounds are taken into account.
 */
int increment7Seg(Time* time, int currIndex) {
	// Save value of current index into a temp value
	int temp = readFrom7Seg(time, currIndex);

	int firstSeg = readFrom7Seg(time, 0);
	int secondSeg = readFrom7Seg(time, 1);

	switch(currIndex) {

	// HEX5 / 7Seg[0]
	case 0:

		// If the second display is equal to 9
		// keep the first display between 0 and 2
		// and set the second display equal to 0
		if (secondSeg == 9) {
			temp = (temp + 1) % 3;
			writeTo7Seg(time, 1, 0);
		}
		// If the second display is greater than 4,
		// keep the first display between 0 and 1
		else if (secondSeg > 4) temp = (temp + 1) % 2;
		// Otherwise keep it between 0 and 2
		else temp = (temp + 1) % 3;
		break;

		// HEX4 / 7Seg[1]
	case 1:
		// If the first display is greater than 1 AND
		// the second display is currently 3, set current display (first)
		// and the first display equal to 0 (clock reset)
		if (firstSeg > 1 && temp == 3) {
			temp = 0;
			writeTo7Seg(time, 0, 0);
		}
		// If the first display is greater than 1,
		// keep the second display between 0 and 4
		else if (firstSeg > 1) temp = (temp + 1) % 5;
		// Otherwise keep it between 0 and 11
		else {
			temp = (temp + 1) % 11;
			// But if it's equal to 10, set the current display
			// equal to 0 and call increment7Seg() on the previous display
			if (temp == 10) {
				temp = 0;
				increment7Seg(time, currIndex - 1);
			}
		}
		break;

		// HEX3 / 7Seg[2]
	case 2:
		// Keep the display between 0 and 6
		temp = (temp + 1) % 7;

		// But if it's equal to 6, set the current display
		// equal to 0 and call increment7Seg() on the previous display
		if (temp == 6) {
			temp = 0;
			increment7Seg(time, currIndex - 1);
		}
		break;

		// HEX4 / 7Seg[3]
	case 3:
		// Keep the display between 0 and 10
		temp = (temp + 1) % 11;

		// But if it's equal to 6, set the current display
		// equal to 0 and call increment7Seg() on the previous display
		if (temp == 10) {
			temp = 0;
			increment7Seg(time, currIndex - 1);
		}
	}

	// Write the new value
	writeTo7Seg(time, currIndex, temp);

	// Return some status code
	return 0;
}

/**
 * This function will write to a single 7-segment display.
 */
int writeTo7Seg(Time* time, int currIndex, int value) {
	// Implement some logic to write to 7 segment display
	time->sevenSeg[currIndex] = value;

	// Return some status code
	return 0;
}


/**
 * This function will read the value at a specified 7-segment
 * display and return a decimal representation of the value.
 */
int readFrom7Seg(const Time* time, int currIndex) {
	// Return decimal value of the current 7 segment display
	return time->sevenSeg[currIndex];
}

/**
 * This function will decrement the value at the current
 * display, making sure that bounds are taken into account.
 */
int decrement7Seg(Time* time, int currIndex) {
	// Save value of current index into a temp value
	int temp = readFrom7Seg(time, currIndex);

	int firstSeg = readFrom7Seg(time, 0);
	int secondSeg = readFrom7Seg(time, 1);

	switch(currIndex) {

		// HEX5 / 7Seg[0]
	case 0:
		// If the first display is at 0, add three to
		// keep it in bounds, and if the second display
		// is greater than 4, set the second display equal
		// to zero
		if (temp == 0) {
			temp += 3;
			if (secondSeg > 4) writeTo7Seg(time, 1, 0);
		}
		break;

		// HEX4 / 7Seg[1]
	case 1:
		// If the second display is at 0 decrement the previous display
		// and the first display is greater than zero, add ten to the
		// current display to keep it in bounds Otherwise if the first
		// display is at 0, and add four to the current display
		if (temp == 0) {
			decrement7Seg(time, currIndex - 1);
			if (firstSeg > 0) temp += 10;
			else if (firstSeg == 0) temp += 4;
		}

		break;

		// HEX3 / 7Seg[2]
	case 2:
		// If the third display is at 0, add six to keep
		// it in bounds when we decrement it
		// If the second display greater than 0, decrement it
		// If the second display and first display are at 0,
		// set the first to two and the second to three (clock loop)
		// All other cases, decrement the first display and set
		// the second display to be at 9
		if (temp == 0) {
			temp += 6;
			if (secondSeg > 0) decrement7Seg(time, 1);
			else if (secondSeg == 0 && firstSeg == 0) {
				writeTo7Seg(time, 0, 2);
				writeTo7Seg(time, 1, 3);
			} else {
				decrement7Seg(time, 0);
				writeTo7Seg(time, 1, 9);
			}
		}
		break;

		// HEX2 / 7Seg[3]
	case 3:
		// If the fourth display is at 0, add ten to keep
		// it in bounds when we decrement it and decrement the previous
		// display
		if (temp == 0) {
			temp += 10;
			decrement7Seg(time, currIndex - 1);
		}
		break;

	};

	// Write the new value to the display after decrementing it
	writeTo7Seg(time, currIndex, --temp);

	// Return some status code
	return 0;
}
