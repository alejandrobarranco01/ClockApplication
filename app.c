/**
 * @file app.c
 * @brief Main application for the Clock Application.
 *
 * @author Alejandro Barranco-Leyte
 */

//#include <unistd.h>
#include "app.h"
#include "hardware.h"

volatile AppState current_state = OFF;

int main(int argc, char **argv) {

	printf("Welcome Clock Application\n\n");

	// If hardware pointers fail to initialize exit
	if (setUpPointers() == -1)
		exit(1);

	if (setUpClock())
		exit(1);

	startSecondCounter();

	pthread_t state_thread;
	if (pthread_create(&state_thread, NULL, stateCheckThread, NULL)) {
		fprintf(stderr, "Error creating state thread\n");
		exit(1);
	}

	// Main application loop
	while (1) {
		if (current_state == VIEW_MODE) {
			viewMode(time);
		} else if (current_state == MODIFY_MODE) {
			modifyMode(time);
		} else if (current_state == OFF) {
			turnOff();
		}
	}

	stopSecondCounter();

	// Cleanup (though we never get here in this example)
	pthread_join(state_thread, NULL);
	free(time);
	return 0;
}

void viewMode() {
	// Display time continuously
	while (current_state == VIEW_MODE) {
		writeTime();
	}
}

void modifyMode() {
	printf("Entered Modify Mode\n");

	static int display_index = 2;
	int button = -1;

	while (current_state == MODIFY_MODE) {
		button = checkButtons();
		if (button != -1) {
			//printf("Button %d pressed\n", button);

			if (button == 0) {  // KEY0 - Change display selection
				display_index++;
				if (display_index > 5)
					display_index = 2;
				printf("Now editing display %d\n", display_index);
			} else if (button == 1) {
				incrementDisplay(display_index);
			} else if (button == 2)
				decrementDisplay(display_index);
			else if (button == 3)
				resetSeconds();
			// Wait for button release
			while (checkButtons() == button) {
				usleep(10000);
			}
		}
		usleep(200000);  // Main loop delay
	}
	printf("Exited Modify Mode\n");
}

void* stateCheckThread(void *arg) {
	(void) arg;

	while (1) {
		int readState = checkState();
		switch (readState) {
		case -1:
			current_state = OFF;
			break;
		case 0:
			current_state = VIEW_MODE;
			break;
		case 1:
			current_state = MODIFY_MODE;
			break;
		}
		usleep(100000);
	}
	return NULL;
}

