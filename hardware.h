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
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>
#include <pthread.h>

#include "address_map_arm.h"

typedef struct {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} ClockTime;

typedef struct {
	unsigned int hex0 :4;
	unsigned int hex1 :4;
	unsigned int hex2 :4;
	unsigned int hex3 :4;
	unsigned int hex4 :4;
	unsigned int hex5 :4;
	unsigned int hex6 :8;
} HEX_Registers;

typedef struct {
	uint32_t sw0 :1;
	uint32_t sw1 :1;
	uint32_t sw2 :1;
	uint32_t sw3 :1;
	uint32_t sw4 :1;
	uint32_t sw5 :1;
	uint32_t sw6 :1;
	uint32_t sw7 :1;
	uint32_t sw8 :1;
	uint32_t sw9 :1;
	uint32_t :22;
} SWRegister;

typedef struct {
	uint32_t key0 :1;
	uint32_t key1 :1;
	uint32_t key2 :1;
	uint32_t key3 :1;
	uint32_t :28;
} KeyRegister;

volatile unsigned int *KEY_ptr;
volatile unsigned int *SW_ptr;
volatile unsigned int *JP1_ptr;

extern volatile ClockTime *clockTime;

int setUpClock();
int checkState(void);
int checkButtons();
int writeTime();
int incrementDisplay(int display_index);
int decrementDisplay(int display_index);
int resetSeconds();
int turnOff();

void startSecondCounter();
void* secondCounterThread(void *arg);
void stopSecondCounter();

// Set up functions
int open_physical(int);
void* map_physical(int, unsigned int, unsigned int);
void close_physical(int);
int unmap_physical(void*, unsigned int);
int setUpPointers();

#endif /* HARDWARE_H_ */
