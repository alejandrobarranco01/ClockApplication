/**
 * @file hardware.h
 * @brief Header file for hardware-related functions.
 *
 * @author Alejandro Barranco-Leyte
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

typedef struct {
	int sevenSeg[4];
} Time;

Time* readTimeFromMemory();
int displayTime(const Time* time);


#endif /* HARDWARE_H_ */
