/**
 * @file app.c
 * @brief Main application for the Clock Application.
 *
 * @author Alejandro Barranco-Leyte
 */

#include "app.h"
#include "hardware.h"

/**
 * @brief Main function for the Clock Application
 *
 * This function helps do the basic set up of the application,
 * such as calling hardware initialization functions, reading saved time
 * from memory and creates a thread to handle input.
 *
 */
int main(int argc, char **argv) {

	printf("Welcome Clock Application\n\n");

	// If hardware pointers fail to initialize exit
	if (setUpPointers() == -1)
		exit(1);

	Time *savedTime = readTimeFromMemory(); // Read time from memory

	// Thread to handle inputs as well as seconds
	pthread_t handleTimeAndSwitchesThread;

	if (pthread_create(&handleTimeAndSwitchesThread, NULL,
			handleTimeAndSwitches, (void*) savedTime)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	while (1) {
		displayTime(savedTime); // Display Time

		int button = readButtons(savedTime, -2); // Wait for button pressed
		if (button == 4) {

			changeTimeMode(savedTime);  // Enter change time mode

		} else
			continue;
	}

	pthread_join(handleTimeAndSwitchesThread, NULL);

	free(savedTime);

	return 0;
}

/**
 * @brief Enters the "Change Time" mode where a user can
 * adjust the time to their needs/wants and also save their
 * changes
 *
 * This function will serve as a "Change Time" mode in which
 * a user can modify and save changes to the clock.
 */
int changeTimeMode(Time *time) {
	printf("You are in changeTimeMode(Time* time)\n");
	int currIndex = 0; // Initialize index for displays

	while (1) {

		int button = readButtons(time, currIndex);

		switch (button) {
		case 4:
			saveChanges(time);
			return 0;
			break;
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
		};
	}

	return 0;
}



