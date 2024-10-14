/**
 * @file app.c
 * @brief Main application for the Clock Application.
 *
 * @author Alejandro Barranco-Leyte
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "app.h"
#include "hardware.h"


int main(int argc, char **argv) {

	printf("Welcome Clock Application\n\n");

	Time* savedTime = readTimeFromMemory(); // Read time from memory

	while (1) {
		displayTime(savedTime); // Display Time
		// int input = readButtons();
		int input;
		scanf("%d", &input);
		if (input == 4) changeTimeMode(savedTime); // 4 = KEY0
	}

	free(savedTime);
	return 0;
}

/**
 * This function will serve as a "Change Time" mode in which
 * a user can modify and save changes to the clock.
 */
int changeTimeMode(Time* time) {
	printf("You are in changeTimeMode(Time* time)\n");
	int currIndex = 0; // Initialize index for displays

	while (1) {
		flashDigit(currIndex); // Make current digit flash

		// int input = readButtons();
		int input;
		scanf("%d", &input);
		switch(input) {
		case 4:
			saveChanges(time);
			return 0; // Return to main application
		case 3:
			printf("Increment index\n");
			currIndex = (currIndex + 1) % 4; // To stay in bounds
			printf("New index: %d\n", currIndex);
			break;
		case 2:
			printf("Increment value\n");
			increment7Seg(time, currIndex);
			displayTime(time); // Show new time (for now)
			break;
		case 1:
			printf("Decrement value\n");
			decrement7Seg(time, currIndex);
			displayTime(time); // Show new time (for now)
			break;
		default:
			printf("Incorrect input\n");
		};
	}

	return 0;
}
