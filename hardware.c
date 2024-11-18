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
	Time *time = (Time*) malloc(sizeof(Time));
	if (time == NULL) {
		printf("Memory allocation failed\n");
		return NULL;
	}

	FILE *file = fopen("/home/root/time/time.txt", "r");
	if (file == NULL) {
		perror("Error opening time.txt");
		free(time);
		return NULL;
	}

	// Read the time values from the file and populate the Time structure
	if (fscanf(file, "%d\n%d\n%d\n%d\n%d\n%d", &time->sevenSeg[0],
			&time->sevenSeg[1], &time->sevenSeg[2], &time->sevenSeg[3],
			&time->sevenSeg[4], &time->sevenSeg[5]) != 6) {
		printf("Error reading time from file\n");
		fclose(file);
		free(time);
		return NULL;
	}

	fclose(file);

	// Display the time on the 7-segment displays
	writeTo7Seg(time, 0, time->sevenSeg[0]);
	writeTo7Seg(time, 1, time->sevenSeg[1]);
	writeTo7Seg(time, 2, time->sevenSeg[2]);
	writeTo7Seg(time, 3, time->sevenSeg[3]);

	writeTo7Seg(time, 4, time->sevenSeg[4]);
	writeTo7Seg(time, 5, time->sevenSeg[5]);

	return time;
}

/**
 * This function prints the time to the console.
 */
int displayTime(const Time *time) {
	printf("displayTime(const Time* time) -> %d%d:%d%d\n",
			readFrom7Seg(time, 0), readFrom7Seg(time, 1), readFrom7Seg(time, 2),
			readFrom7Seg(time, 3));

	// Return some status code
	return 0;
}

void flashDigit(Time *time, int currIndex) {
	int temp = readFrom7Seg(time, currIndex);
	writeTo7Seg(time, currIndex, -1);
	usleep(100000);
	writeTo7Seg(time, currIndex, temp);
	usleep(100000);
}

/**
 * This function will read inputs from the push buttons
 */
int readButtons(Time *time, int currIndex) {
	// Will read hardware push buttons states
	printf("readButtons()\n");

	// Create a pointer to HEX0_HEX3Register mapped to HEX_ptr
	KeyRegister *myButtons = (KeyRegister*) KEY_ptr;

	SWRegister *mySwitches = (SWRegister*) SW_ptr;

	int buttonPressed = -1;

	while (buttonPressed == -1) {

		if (currIndex != -2 && mySwitches->sw9)
			flashDigit(time, currIndex);

		if (myButtons->key0) {
			printf("KEY0 pressed\n");
			buttonPressed = 4; // Key 0 corresponds to 4
		} else if (myButtons->key1) {
			printf("KEY1 pressed\n");
			buttonPressed = 3; // Key 1 corresponds to 3
		} else if (myButtons->key2) {
			printf("KEY2 pressed\n");
			buttonPressed = 2; // Key 2 corresponds to 2
		} else if (myButtons->key3) {
			printf("KEY3 pressed\n");
			buttonPressed = 1; // Key 3 corresponds to 1
		}
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

	FILE *file = fopen("/home/root/time/time.txt", "w");
	if (file == NULL) {
		perror("Error opening time.txt");
		return -1;
	}

	if (fprintf(file, "%d\n%d\n%d\n%d\n%d\n%d\n", time->sevenSeg[0],
			time->sevenSeg[1], time->sevenSeg[2], time->sevenSeg[3],
			time->sevenSeg[4], time->sevenSeg[5]) < 0) {
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

	int firstSeg = readFrom7Seg(time, 0);
	int secondSeg = readFrom7Seg(time, 1);

	switch (currIndex) {

	// HEX5 / 7Seg[0]
	case 0:

		// If the second display is equal to 9
		// keep the first display between 0 and 2
		// and set the second display equal to 0
		if (secondSeg == 9) {
			temp = (temp + 1) % 3;
			writeTo7Seg(time, 1, 0);
		}
		// If the second display is greater than 3,
		// keep the first display between 0 and 1
		else if (secondSeg > 3)
			temp = (temp + 1) % 2;
		// Otherwise keep it between 0 and 2
		else
			temp = (temp + 1) % 3;
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
		else if (firstSeg > 1)
			temp = (temp + 1) % 5;
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
int writeTo7Seg(Time *time, int currIndex, int value) {
	// Implement some logic to write to 7 segment display
	time->sevenSeg[currIndex] = value;

	// Create a pointer to HEX0_HEX3Register mapped to HEX_ptr1
	HEX_Registers1 *firstFour = (HEX_Registers1*) HEX_ptr1;
	HEX_Registers2 *secondTwo = (HEX_Registers2*) HEX_ptr2;

	updateAll(time, firstFour, secondTwo);

	// Return some status code
	return 0;
}

/**
 * This function will read the value at a specified 7-segment
 * display and return a decimal representation of the value.
 */
int readFrom7Seg(const Time *time, int currIndex) {
	// Return decimal value of the current 7 segment display
	return time->sevenSeg[currIndex];

	// Create a pointer to HEX0_HEX3Register mapped to HEX_ptr1
	HEX_Registers1 *firstFour = (HEX_Registers1*) HEX_ptr1;
	HEX_Registers2 *secondTwo = (HEX_Registers2*) HEX_ptr2;

	switch (currIndex) {

	case 3:
		return (int) firstFour->thirddisp;
		break;
	case 2:
		return (int) firstFour->fourthdisp;
		break;
	case 1:
		return (int) secondTwo->fifthdisp;
		break;
	case 0:
		return (int) secondTwo->sixthdisp;
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

	int firstSeg = readFrom7Seg(time, 0);
	int secondSeg = readFrom7Seg(time, 1);

	switch (currIndex) {

	// HEX5 / 7Seg[0]
	case 0:
		// If the first display is at 0, add three to
		// keep it in bounds, and if the second display
		// is greater than 4, set the second display equal
		// to zero
		if (temp == 0) {
			temp += 3;
			if (secondSeg > 4)
				writeTo7Seg(time, 1, 0);
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
			if (firstSeg > 0)
				temp += 10;
			else if (firstSeg == 0)
				temp += 4;
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
			if (secondSeg > 0)
				decrement7Seg(time, 1);
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

void updateAll(Time *time, HEX_Registers1 *firstFour, HEX_Registers2 *secondTwo) {
	firstFour->firstdisp = bcd2sevenSegmentDecoder(time->sevenSeg[5]);
	firstFour->seconddisp = bcd2sevenSegmentDecoder(time->sevenSeg[4]);
	firstFour->thirddisp = bcd2sevenSegmentDecoder(time->sevenSeg[3]);
	firstFour->fourthdisp = bcd2sevenSegmentDecoder(time->sevenSeg[2]);
	secondTwo->fifthdisp = bcd2sevenSegmentDecoder(time->sevenSeg[1]);
	secondTwo->sixthdisp = bcd2sevenSegmentDecoder(time->sevenSeg[0]);

	// Prevent incorrect digits by reading and fixing any mistakes
	while (!isValidHex(firstFour->firstdisp)) {
		firstFour->firstdisp = bcd2sevenSegmentDecoder(time->sevenSeg[5]);
	}
	while (!isValidHex(firstFour->seconddisp)) {
		firstFour->seconddisp = bcd2sevenSegmentDecoder(time->sevenSeg[4]);
	}

	while (!isValidHex(firstFour->thirddisp)) {
		firstFour->thirddisp = bcd2sevenSegmentDecoder(time->sevenSeg[3]);
	}
	while (!isValidHex(firstFour->fourthdisp)) {
		firstFour->fourthdisp = bcd2sevenSegmentDecoder(time->sevenSeg[2]);
	}
	while (!isValidHex(secondTwo->fifthdisp)) {
		secondTwo->fifthdisp = bcd2sevenSegmentDecoder(time->sevenSeg[1]);
	}
	while (!isValidHex(secondTwo->sixthdisp)) {
		secondTwo->sixthdisp = bcd2sevenSegmentDecoder(time->sevenSeg[0]);
	}

}

int isValidHex(unsigned int val) {
	return (val == 0x3F || val == 0x06 || val == 0x5B || val == 0x4F
			|| val == 0x66 || val == 0x6D || val == 0x7D || val == 0x07
			|| val == 0x7F || val == 0x6F || val == 0x00);
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

int setUpPointers() {
	int fd = -1;       // used to open /dev/mem for access to physical addresses
	void *LW_virtual = NULL; // used to map physical addresses for the light-weight bridge
	void *HPS_virtual = NULL;

	// Create virtual memory access to the FPGA light-weight bridge
	if ((fd = open_physical(fd)) == -1) {
		return -1;
	}
	if ((LW_virtual = map_physical(fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL) {
		return -1;
	}
	if ((HPS_virtual = map_physical(fd, HPS_BRIDGE_BASE, HPS_BRIDGE_SPAN))
			== NULL) {
		return -1;
	}

	HEX_ptr1 = NULL;
	HEX_ptr1 = (volatile unsigned int*) (LW_virtual + HEX3_HEX0_BASE);
	if (!HEX_ptr1)
		return -1;

	HEX_ptr2 = NULL;
	HEX_ptr2 = (volatile unsigned int*) (LW_virtual + HEX5_HEX4_BASE);
	if (!HEX_ptr2)
		return -1;

	SW_ptr = NULL;
	SW_ptr = (volatile unsigned int*) (LW_virtual + SW_BASE);
	if (!SW_ptr)
		return -1;

	KEY_ptr = NULL;
	KEY_ptr = (volatile unsigned int*) (LW_virtual + KEY_BASE);
	if (!KEY_ptr)
		return -1;

	return 0;
}

// Function to convert decimal to 7-segment value
unsigned int bcd2sevenSegmentDecoder(int digit) {
	switch (digit) {
	case 0:
		return 0x3F; // 00111111 -> Display '0'
	case 1:
		return 0x06; // 00000110 -> Display '1'
	case 2:
		return 0x5B; // 01011011 -> Display '2'
	case 3:
		return 0x4F; // 01001111 -> Display '3'
	case 4:
		return 0x66; // 01100110 -> Display '4'
	case 5:
		return 0x6D; // 01101101 -> Display '5'
	case 6:
		return 0x7D; // 01111101 -> Display '6'
	case 7:
		return 0x07; // 00000111 -> Display '7'
	case 8:
		return 0x7F; // 01111111 -> Display '8'
	case 9:
		return 0x6F; // 01101111 -> Display '9'
	default:
		return 0x00; // Blank or off
	}
}

void* handleTimeAndSwitches(void *arg) {
	SWRegister *mySwitches = (SWRegister*) SW_ptr; // Pointer to switch register

	HEX_Registers1 *firstFour = (HEX_Registers1*) HEX_ptr1; // First four 7-segment display registers
	HEX_Registers2 *secondTwo = (HEX_Registers2*) HEX_ptr2; // Last two 7-segment display registers

	Time *time = (Time*) arg;  // Cast the argument to Time* type

	Time blankTime = { .sevenSeg = { -1, -1, -1, -1, -1, -1 } };
	int timeSaved = 1;

	int seconds = time->sevenSeg[4] * 10 + time->sevenSeg[5];

	int currState = mySwitches->sw0;
	int altState = (currState == 1) ? 0 : 1;

	while (1) {
		if (mySwitches->sw0 == altState) {
			seconds = -1;
			currState = altState;

			altState = (currState == 1) ? 0 : 1;

			// Update the display with the new seconds
			time->sevenSeg[4] = seconds / 10;
			time->sevenSeg[5] = seconds % 10;

			continue;
		}

		usleep(900000);

		// Increment the seconds
		seconds++;
		time->sevenSeg[4] = seconds / 10;
		time->sevenSeg[5] = seconds % 10;

		if (seconds == 60) {
			seconds = 0;
			time->sevenSeg[4] = seconds / 10;
			time->sevenSeg[5] = seconds % 10;
			increment7Seg(time, 3);
		}

		if (mySwitches->sw9 == 1) {
			updateAll(time, firstFour, secondTwo);
			timeSaved = 0;
		} else {
			if (!timeSaved) {
				if (saveChanges(time) < 0) {
					perror("Error saving time");
				}
				timeSaved = 1;  // Mark that time has been saved
			}

			// Blank the display if sw9 is OFF
			updateAll(&blankTime, firstFour, secondTwo);
		}

		usleep(100000);
	}
}


