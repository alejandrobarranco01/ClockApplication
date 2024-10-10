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
	displayTime(savedTime); // Display Time

	while (1) {
		// int input = readButtons();
		int input;
		scanf("%d", &input);
		if (input == 4) changeTimeMode(savedTime); // 4 = KEY0
	}

	free(savedTime); // Free up the dynamically allocated memory for savedTime

	return 0;
}

int changeTimeMode(Time* time) {
	printf("You are in changeTimeMode(Time* time)\n");
	int currIndex = 0; // Initialize index for displays

	flashDigit(currIndex); // Make current digit flash

	while (1) {
		// int input = readButtons();
		int input;
		scanf("%d", &input);
		switch(input) {
		case 4:
			printf("Save changes\n");
			break;
		case 3:
			printf("Increment index\n");
			break;
		case 2:
			printf("Increment value\n");
			break;
		case 1:
			printf("Decrement value\n");
		default:
			printf("Incorrect input\n");
		};
	}

	return 0;
}
