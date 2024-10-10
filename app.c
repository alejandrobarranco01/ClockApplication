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

	int input;
	scanf("%d\n", &input);
	if (input == 4) changeTimeMode(savedTime);

	free(savedTime); // Free up the dynamically allocated memory for savedTime

	return 0;
}

int changeTimeMode(Time* time) {
	printf("You are in changeTimeMode(Time* time)\n");
	int currIndex = 0;

	return 0;
}
