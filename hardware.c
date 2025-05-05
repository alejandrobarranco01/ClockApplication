/**
 * @file hardware.c
 * @brief Hardware interface for the Clock Application.
 *
 * @author Alejandro Barranco-Leyte
 */

#include <time.h>
#include <pthread.h>

#include "hardware.h"

volatile ClockTime *clockTime = NULL;

static pthread_t sec_thread;
static volatile int sec_thread_run = 0;

/*
 * This function helps set up the clock
 */
int setUpClock() {
	clockTime = (ClockTime*) malloc(sizeof(ClockTime));
	if (clockTime == NULL) {
		return -1;
	}

	// Initialize time to noon
	clockTime->hours = 12;
	clockTime->minutes = 0;
	clockTime->seconds = 0;

	// printf("Before writeTime()\n");

	return writeTime();

}

/*
 *	This function helps constantly write the time
 */
int writeTime() {
	HEX_Registers *displays = (HEX_Registers*) (JP1_ptr + 0);
	*(JP1_ptr + 1) = 0x00FFFFFF;

	displays->hex0 = clockTime->seconds % 10;
	displays->hex1 = clockTime->seconds / 10;
	displays->hex2 = clockTime->minutes % 10;
	displays->hex3 = clockTime->minutes / 10;
	displays->hex4 = clockTime->hours % 10;
	displays->hex5 = clockTime->hours / 10;

	//printf("Just printed time!\n");
	return 0;
}

/*
 * This function checks the state of switches 9 and 0,
 * which are used to determine the mode
 */
int checkState() {
	SWRegister *mySwitches = (SWRegister*) SW_ptr;

	return (mySwitches->sw9 == 0) ? -1 : (mySwitches->sw0 == 1) ? 1 : 0;

}

/*
 * This function returns which push button was pressed
 */
int checkButtons() {
	KeyRegister *keys = (KeyRegister*) KEY_ptr;
	if (keys->key0)
		return 0;
	if (keys->key1)
		return 1;
	if (keys->key2)
		return 2;
	if (keys->key3)
		return 3;
	return -1;
}

/*
 * This function contains the logic for decrementing
 * the value of a display at a certain index
 */
int decrementDisplay(int display_index) {
	switch (display_index) {
	case 2: {
		int minutes = clockTime->minutes;
		minutes--;
		clockTime->minutes = (minutes < 0) ? 59 : minutes;
		break;
	}
	case 3: {
		int minutes = clockTime->minutes;
		minutes -= 10;
		clockTime->minutes = (minutes < 0) ? 59 : minutes;
		break;
	}
	case 4: {
		int hours = clockTime->hours;
		hours--;
		clockTime->hours = (hours < 0) ? 23 : hours;
		break;
	}
	case 5: {
		int hours = clockTime->hours;  // Fixed: was clockTime->minutes
		hours -= 10;
		clockTime->hours = (hours < 0) ? 23 : hours;
		break;
	}
	}
	return writeTime();
}

/*
 * This function contains the logic for incrementing
 * the value of a display at a certain index
 */
int incrementDisplay(int display_index) {
	switch (display_index) {
	case 2: {
		int minutes = clockTime->minutes;
		minutes++;
		clockTime->minutes = (minutes > 59) ? 0 : minutes;
		break;
	}
	case 3: {
		int minutes = clockTime->minutes;
		minutes += 10;
		clockTime->minutes = (minutes > 59) ? 0 : minutes;
		break;
	}
	case 4: {
		int hours = clockTime->hours;
		hours++;
		clockTime->hours = (hours > 23) ? 0 : hours;
		break;
	}
	case 5: {
		int hours = clockTime->hours;
		hours += 10;
		clockTime->hours = (hours > 23) ? 0 : hours;
		break;
	}
	}

	/* This check helps prevent weird glitching
	 * while the seconds update in the background
	 * when the system is OFF
	 */
	if (checkState() != -1)
		return writeTime();
	return 0;
}

/*
 * This function is a simple second resetter
 */
int resetSeconds() {
	clockTime->seconds = 0;
	return writeTime();
}

/*
 * This function turns off all displays
 * when in OFF mode and constantly checks to
 * see if OFF mode is still the case
 */
int turnOff() {
	HEX_Registers *displays = (HEX_Registers*) (JP1_ptr + 0);

	*(JP1_ptr + 1) = 0x00FFFFFF;

	displays->hex0 = 15;
	displays->hex1 = 15;
	displays->hex2 = 15;
	displays->hex3 = 15;
	displays->hex4 = 15;
	displays->hex5 = 15;

	while (checkState() == -1) {
		usleep(10000);
	}

	return writeTime();
}

void* secondCounterThread(void *arg) {
	(void) arg;
	struct timespec ts;
	ts.tv_sec = 1;
	ts.tv_nsec = 0;

	while (sec_thread_run) {
		nanosleep(&ts, NULL); // Wait exactly 1 second

		int new_seconds = clockTime->seconds + 1;
		if (new_seconds > 59) {
			clockTime->seconds = 0;
			incrementDisplay(2);
		} else
			clockTime->seconds = new_seconds;

		if (checkState() != -1)
			writeTime();
	}

	return NULL;
}

void startSecondCounter() {
	sec_thread_run = 1;
	pthread_create(&sec_thread, NULL, secondCounterThread, NULL);
}

void stopSecondCounter() {
	sec_thread_run = 0;
	pthread_join(sec_thread, NULL);
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
	void *HPS_virtual = NULL;  // Separate mapping for HPS peripherals

	// Create virtual memory access to the FPGA light-weight bridge
	if ((fd = open_physical(fd)) == -1) {
		return -1;
	}
	if ((LW_virtual = map_physical(fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL) {
		return -1;
	}

	HPS_virtual = map_physical(fd, HPS_BRIDGE_BASE, HPS_BRIDGE_SPAN);
	if (!HPS_virtual) {
		close_physical(fd);
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


