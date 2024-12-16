/**
 * @file hardware.c
 * @brief Hardware interface for the Clock Application.
 *
 * @author Alejandro Barranco-Leyte
 */

#include "hardware.h"

/**
 * This function dynamically allocates memory for a Time structure
 * and initializes it with an initial value representing the time.
 */
Time* readTimeFromMemory() {
	// Dynamically allocate memory for a pointer to a Time structure
	Time *time = (Time*) malloc(sizeof(Time));

	// If Time is a null pointer return an error
	if (time == NULL) {
		printf("Memory allocation failed\n");
		return NULL;
	}

	// Open the text file that contains the saved time
	FILE *file = fopen("/home/root/time/time.txt", "r");
	// If there's an error opening the time text file, throw an error
	if (file == NULL) {
		perror("Error opening time.txt");
		free(time);
		return NULL;
	}

	// Read the time values from the file and populate the Time structure
	if (fscanf(file, "%d\n%d\n%d\n%d\n%d\n%d", &time->hoursTens,
			&time->hoursOnes, &time->minutesTens, &time->minutesOnes,
			&time->secondsTens, &time->secondsOnes) != 6) {
		printf("Error reading time from file\n");
		fclose(file);
		free(time);
		return NULL;
	}

	// Close the file
	fclose(file);

	// Display the time on the 7-segment displays
	HEX_Registers *displays = (HEX_Registers*) (JP1_ptr + 0);
	*(JP1_ptr + 1) = 0x00FFFFFF;

	// Set up the displays based on the loaded time
	updateAll(time, displays);

	return time;
}

/**
 * This function makes a flashing like functionality
 * on a given 7-segment display by writing the real value
 * and then writing a value of 15 which triggers a blank screen
 * based on the BCD decoder I made
 */
void flashDigit(Time *time, int currIndex) {
	// Save the value of the current 7-segment display in a temp variable
	int temp = readFrom7Seg(time, currIndex);
	writeTo7Seg(time, currIndex, 15); // Make the display go blank
	usleep(100000);
	writeTo7Seg(time, currIndex, temp); // Display the regular value again
	usleep(100000);
}

/**
 * This function will read inputs from the push buttons
 * and returns which button was pushed
 */
int readButtons(Time *time, int currIndex) {
	// Create pointers to the Switch and Key Registers
	KeyRegister *myButtons = (KeyRegister*) KEY_ptr;
	SWRegister *mySwitches = (SWRegister*) SW_ptr;

	// Use this as a flag to signal button press
	int buttonPressed = -1;

	// Run loop while buttons remain unpressed
	while (buttonPressed == -1) {

		// If the flag is not -2 and the S9 switch is up (on state) flash the digit
		if (currIndex != -2 && mySwitches->sw9)
			flashDigit(time, currIndex);

		if (myButtons->key0)
			buttonPressed = 4; // Key0 corresponds to 4
		else if (myButtons->key1)
			buttonPressed = 3; // Key1 corresponds to 3
		else if (myButtons->key2)
			buttonPressed = 2; // Key2 corresponds to 2
		else if (myButtons->key3)
			buttonPressed = 1; // Key3 corresponds to 1

	}

	// Wait until the button is released to avoid re-triggering
	while (myButtons->key0 || myButtons->key1 || myButtons->key2
			|| myButtons->key3) {
		usleep(1000); // Sleep for a short time
	}

	return buttonPressed; // Return the button that was pressed

}

/**
 * This function will save current time into memory.
 */
int saveChanges(const Time *time) {
	// Implement saving time to memory
	printf("saveChanges(const Time* time) -> Saving changes...\n");

	// Open the text file and return an error if there's an issue opening the file
	FILE *file = fopen("/home/root/time/time.txt", "w");
	if (file == NULL) {
		perror("Error opening time.txt");
		return -1;
	}

	// Write to the file the current values from the time structure
	if (fprintf(file, "%d\n%d\n%d\n%d\n%d\n%d\n", time->hoursTens,
			time->hoursOnes, time->minutesTens, time->minutesOnes,
			time->secondsTens, time->secondsOnes) < 0) {
		perror("Error writing to time.txt");
		fclose(file);
		return -2;
	}

	// Close the file after writing
	fclose(file);

	// Return 0 for success
	return 0;
}

/**
 * This function will increment the value at the current
 * display, making sure that bounds are taken into account.
 */
int increment7Seg(Time *time, int currIndex) {
	// Save value of current index into a temp value
	int temp = readFrom7Seg(time, currIndex);

	int hex5Value = readFrom7Seg(time, 5);
	int hex4Value = readFrom7Seg(time, 4);

	switch (currIndex) {

	// HEX5 / 7Seg[0]
	case 5:

		// If the second display is equal to 9
		// keep the first display between 0 and 2
		// and set the second display equal to 0
		if (hex4Value == 9) {
			temp = (temp + 1) % 3;
			writeTo7Seg(time, 4, 0);
		}
		// If the second display is greater than 3,
		// keep the first display between 0 and 1
		else if (hex4Value > 3)
			temp = (temp + 1) % 2;
		// Otherwise keep it between 0 and 2
		else
			temp = (temp + 1) % 3;
		break;

		// HEX4 / 7Seg[1]
	case 4:
		// If the first display is greater than 1 AND
		// the second display is currently 3, set current display (first)
		// and the first display equal to 0 (clock reset)
		if (hex5Value > 1 && temp == 3) {
			temp = 0;
			writeTo7Seg(time, 5, 0);
		}
		// If the first display is greater than 1,
		// keep the second display between 0 and 4
		else if (hex5Value > 1)
			temp = (temp + 1) % 5;
		// Otherwise keep it between 0 and 11
		else {
			temp = (temp + 1) % 11;
			// But if it's equal to 10, set the current display
			// equal to 0 and call increment7Seg() on the previous display
			if (temp == 10) {
				temp = 0;
				increment7Seg(time, currIndex + 1);
			}
		}
		break;

		// HEX3 / 7Seg[2]
	case 3:
		// Keep the display between 0 and 6
		temp = (temp + 1) % 7;

		// But if it's equal to 6, set the current display
		// equal to 0 and call increment7Seg() on the previous display
		if (temp == 6) {
			temp = 0;
			increment7Seg(time, currIndex + 1);
		}
		break;

		// HEX2 / 7Seg[3]
	case 2:
		// Keep the display between 0 and 10
		temp = (temp + 1) % 11;

		// But if it's equal to 6, set the current display
		// equal to 0 and call increment7Seg() on the previous display
		if (temp == 10) {
			temp = 0;
			increment7Seg(time, currIndex + 1);
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
int writeTo7Seg(Time *time, int currIndex, int value) {
	// Create pointer to HEX displays and set them as output
	HEX_Registers *displays = (HEX_Registers*) (JP1_ptr + 0);
	*(JP1_ptr + 1) = 0x00FFFFFF;

	switch (currIndex) {
	// Update HEX0 (Seconds ones places)
	case 0:
		time->secondsOnes = value;
		displays->firstdisp = time->secondsOnes;
		break;
		// Update HEX1 (Seconds tens places)
	case 1:
		time->secondsTens = value;
		displays->seconddisp = time->secondsTens;
		break;
		// Update HEX2 (Minutes ones places)
	case 2:
		time->minutesOnes = value;
		displays->thirddisp = time->minutesOnes;
		break;
		// Update HEX3 (Minutes tens places)
	case 3:
		time->minutesTens = value;
		displays->fourthdisp = time->minutesTens;
		break;
		// Update HEX4 (Hours ones places)
	case 4:
		time->hoursOnes = value;
		displays->fifthdisp = time->hoursOnes;
		break;
		// Update HEX5 (Hours tens places)
	case 5:
		time->hoursTens = value;
		displays->sixthdisp = time->hoursTens;
		break;

	}
	// Update all the displays for consistency
	updateAll(time, displays);

	// Return some status code
	return 0;
}

/**
 * This function will read the value at a specified 7-segment
 * display and return a decimal representation of the value.
 */
int readFrom7Seg(const Time *time, int currIndex) {
	// Create pointer to HEX displays
	HEX_Registers *displays = (HEX_Registers*) (JP1_ptr + 0);
	*(JP1_ptr + 1) = 0x00FFFFFF;

	// Return the value of a given display, casted as an int
	switch (currIndex) {
	case 2:
		return (int) displays->thirddisp;
		break;
	case 3:
		return (int) displays->fourthdisp;
		break;
	case 4:
		return (int) displays->fifthdisp;
		break;
	case 5:
		return (int) displays->sixthdisp;
		break;
	}
	return -1;

}

/**
 * This function will decrement the value at the current
 * display, making sure that bounds are taken into account.
 */
int decrement7Seg(Time *time, int currIndex) {
	// Save value of current index into a temp value
	int temp = readFrom7Seg(time, currIndex);

	int hex5Value = readFrom7Seg(time, 5); // Get the value for HEX5 (Hours tens place)
	int hex4Value = readFrom7Seg(time, 4); // Get the value for HEX4 (Hours ones place)

	switch (currIndex) {

	// HEX5 (Hours tens place)
	case 5:
		// If the first display is at 0, add three to
		// keep it in bounds, and if the second display
		// is greater than 4, set the second display equal
		// to zero
		if (temp == 0) {
			temp += 3;
			if (hex4Value > 4)
				writeTo7Seg(time, 4, 0);
		}
		break;

		// HEX4 (Hours ones place)
	case 4:
		// If the second display is at 0 decrement the previous display
		// and the first display is greater than zero, add ten to the
		// current display to keep it in bounds Otherwise if the first
		// display is at 0, and add four to the current display
		if (temp == 0) {
			decrement7Seg(time, currIndex + 1);
			if (hex5Value > 0)
				temp += 10;
			else if (hex5Value == 0)
				temp += 4;
		}

		break;
		// HEX3 (Minutes tens place)
	case 3:
		// If the third display is at 0, add six to keep
		// it in bounds when we decrement it
		// If the second display greater than 0, decrement it
		// If the second display and first display are at 0,
		// set the first to two and the second to three (clock loop)
		// All other cases, decrement the first display and set
		// the second display to be at 9
		if (temp == 0) {
			temp += 6;
			if (hex4Value > 0)
				decrement7Seg(time, 4);
			else if (hex4Value == 0 && hex5Value == 0) {
				writeTo7Seg(time, 5, 2);
				writeTo7Seg(time, 4, 3);
			} else {
				decrement7Seg(time, 5);
				writeTo7Seg(time, 4, 9);
			}
		}
		break;

		// HEX2 (Minutes ones place)
	case 2:
		// If the fourth display is at 0, add ten to keep
		// it in bounds when we decrement it and decrement the previous
		// display
		if (temp == 0) {
			temp += 10;
			decrement7Seg(time, currIndex + 1);
		}
		break;

	};

	// Write the new value to the display after decrementing it
	writeTo7Seg(time, currIndex, --temp);

	// Return some status code
	return 0;
}

/**
 * This function helps update all seven-segment displays with the current values
 *
 */
void updateAll(Time *time, HEX_Registers *displays) {
	displays->firstdisp = time->secondsOnes;
	displays->seconddisp = time->secondsTens;
	displays->thirddisp = time->minutesOnes;
	displays->fourthdisp = time->minutesTens;
	displays->fifthdisp = time->hoursOnes;
	displays->sixthdisp = time->hoursTens;
}

// Open /dev/mem to give access to physical addresses
int open_physical(int fd) {
	if (fd == -1)
		if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
			printf("ERROR: could not open \"/dev/mem\"...\n");
			return -1;
		}
	return fd;
}

// Close /dev/mem to release access to physical addresses
void close_physical(int fd) {
	close(fd);
}

/*
 * Establish a virtual address mapping for the physical addresses starting at base,
 * extending by span bytes.
 */
void* map_physical(int fd, unsigned int base, unsigned int span) {
	void *virtual_base;

	// Get a mapping from physical addresses to virtual addresses
	virtual_base = mmap(NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd,
			base);
	if (virtual_base == MAP_FAILED) {
		printf("ERROR: mmap() failed...\n");
		close(fd);
		return NULL;
	}
	return virtual_base;
}

/*
 * Close the previously-opened virtual address mapping
 */
int unmap_physical(void *virtual_base, unsigned int span) {
	if (munmap(virtual_base, span) != 0) {
		printf("ERROR: munmap() failed...\n");
		return -1;
	}
	return 0;
}

/**
 * This function sets up pointers for accessing the hex displays,
 * buttons and the switches
 */

int setUpPointers() {
	int fd = -1;       // used to open /dev/mem for access to physical addresses
	void *LW_virtual = NULL; // used to map physical addresses for the light-weight bridge

	// Create virtual memory access to the FPGA light-weight bridge
	if ((fd = open_physical(fd)) == -1) {
		return -1;
	}
	if ((LW_virtual = map_physical(fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL) {
		return -1;
	}

	// Set up the pointer to the switches
	SW_ptr = NULL;
	SW_ptr = (volatile unsigned int*) (LW_virtual + SW_BASE);
	if (!SW_ptr)
		return -1;

	// Set up the pointer to the push buttons
	KEY_ptr = NULL;
	KEY_ptr = (volatile unsigned int*) (LW_virtual + KEY_BASE);
	if (!KEY_ptr)
		return -1;

	// Set up pointer to the JP1 expansion port (for the HEX displays)
	JP1_ptr = NULL;
	JP1_ptr = (volatile unsigned int*) (LW_virtual + JP1_BASE);
	if (!JP1_ptr)
		return -1;

	return 0;
}

/**
 * This function handles time and switches in a continuous loop
 */

void* handleTimeAndSwitches(void *arg) {
	SWRegister *mySwitches = (SWRegister*) SW_ptr; // Pointer to switch register
	HEX_Registers *displays = (HEX_Registers*) (JP1_ptr + 0);
	*(JP1_ptr + 1) = 0x00FFFFFF;

	Time *time = (Time*) arg;  // Cast the argument to Time* type

	Time blankTime = { 15, 15, 15, 15, 15, 15 }; //Create a blank time to clear display when flashing
	int timeSaved = 1; // Flag to track whether time has been saved yet

	int seconds = time->secondsTens * 10 + time->secondsOnes;

	// Initialize some sort of current state and alt state
	// in order to toggle
	int currState = mySwitches->sw0;
	int altState = (currState == 1) ? 0 : 1;

	while (1) {
		// Check if switch 0 has been toggled
		if (mySwitches->sw0 == altState) {
			seconds = 0; // Reset the seconds
			currState = altState; // Update current state

			altState = (currState == 1) ? 0 : 1; // Toggle the alt state

			// Update the display with the new seconds
			time->secondsTens = 0;
			time->secondsOnes = 0;

			updateAll(time, displays);

			continue;
		}

		// For now, artificially simulate time with 900 ms
		usleep(900000);

		// Increment the seconds
		seconds++;
		time->secondsTens = seconds / 10;
		time->secondsOnes = seconds % 10;

		// If we reach 60 seconds, handle time accordingly
		if (seconds == 60) {
			seconds = 0;
			time->secondsTens = seconds / 10;
			time->secondsOnes = seconds % 10;
			increment7Seg(time, 3);
		}

		// If at anytime switch 9 is on, update all displays and mark time as unsaved
		if (mySwitches->sw9 == 1) {
			updateAll(time, displays);

			timeSaved = 0;
		}
		// IF switch 9 goes down
		else {
			// Check to see if the time is saved
			if (!timeSaved) {
				if (saveChanges(time) < 0) {
					perror("Error saving time");
				}

				timeSaved = 1;  // Mark that time has been saved
			}

			// Blank the display if sw9 is OFF
			updateAll(&blankTime, displays);
		}

		usleep(100000);
	}
}

