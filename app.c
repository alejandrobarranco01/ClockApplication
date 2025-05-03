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

	AppState current_state = VIEW_MODE;
	while (1) {
		if (current_state == VIEW_MODE) {
			// -2 is a flag to signify that we are in display time mode
			// so that the flash digit function isn't triggered
			int button = readButtons(savedTime, -2); // Wait for button pressed
			// If KEY0 is pressed then enter change time mode
			if (button == 4)
				current_state = MODIFY_MODE;		// Enter change time mode
		} else if (current_state == MODIFY_MODE) {
			changeTimeMode(savedTime);  // Enters modify mode
			current_state = VIEW_MODE;  // Auto-return to view after exit
		}
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

	// We will start on HEX2 (minutes ones)
	int currIndex = 2;

	// Main change time mode loop
	while (1) {

		// Wait for button to be pressed, pass the current index as a flag
		// so that digit flashes
		int button = readButtons(time, currIndex);

		switch (button) {
		// KEY0 returns back to main mode
		case 4:
			saveChanges(time);
			return 0;
			break;
			// KEY1 increments in the index
		case 3:
			printf("Increment index\n");
			currIndex = (currIndex + 1) % 4 + 2; // To stay in bounds
			printf("New index: %d\n", currIndex);
			break;
			// KEY2 increments the value at the current index
		case 2:
			printf("Increment value\n");
			increment7Seg(time, currIndex);
			break;
			// KEY3 decrements the value at the current index
		case 1:
			printf("Decrement value\n");
			decrement7Seg(time, currIndex);
			break;
		};
	}

	return 0;
}
