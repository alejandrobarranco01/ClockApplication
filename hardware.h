/**
 * @file hardware.h
 * @brief Header file for hardware-related functions.
 *
 * @author Alejandro Barranco-Leyte
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>

#include "address_map_arm.h"

typedef struct {
	int sevenSeg[6];
} Time;

typedef struct {
	unsigned int firstdisp :8;
	unsigned int seconddisp :8;
	unsigned int thirddisp :8;
	unsigned int fourthdisp :8;
} HEX_Registers1;

typedef struct {
	unsigned int fifthdisp :8;
	unsigned int sixthdisp :8;
	unsigned int :16;
} HEX_Registers2;

typedef struct {
	unsigned int sw0 :1;
	unsigned int sw1 :1;
	unsigned int sw2 :1;
	unsigned int sw3 :1;
	unsigned int sw4 :1;
	unsigned int sw5 :1;
	unsigned int sw6 :1;
	unsigned int sw7 :1;
	unsigned int sw8 :1;
	unsigned int sw9 :1;
	unsigned int :22;
} SWRegister;

typedef struct {
	unsigned int key0 :1;
	unsigned int key1 :1;
	unsigned int key2 :1;
	unsigned int key3 :1;
	unsigned int :28;
} KeyRegister;

volatile unsigned int *HEX_ptr1;
volatile unsigned int *HEX_ptr2;
volatile unsigned int *KEY_ptr;
volatile unsigned int *SW_ptr;

Time* readTimeFromMemory();
int displayTime(const Time *time);
int readButtons(Time *time, int currIndex);
void flashDigit(Time *time, int currIndex);
int saveChanges(const Time *time);
int increment7Seg(Time *time, int currIndex);
int writeTo7Seg(Time *time, int currIndex, int value);
int readFrom7Seg(const Time *time, int currIndex);
int decrement7Seg(Time *time, int currIndex);
void updateAll(Time *time, HEX_Registers1 *firstFour, HEX_Registers2 *secondTwo);
int isValidHex(unsigned int val);

// Set up functions
int open_physical(int);
void* map_physical(int, unsigned int, unsigned int);
void close_physical(int);
int unmap_physical(void*, unsigned int);
int setUpPointers();
unsigned int bcd2sevenSegmentDecoder(int digit);
void* handleTimeAndSwitches(void *arg);

#endif /* HARDWARE_H_ */
