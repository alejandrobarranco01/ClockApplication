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
int readButtons();
void flashDigit(int currIndex);
int saveChanges(const Time* time);
int increment7Seg(Time* time, int currIndex);
int writeTo7Seg(Time* time, int currIndex, int value);
int readFrom7Seg(const Time* time, int currIndex);


#endif /* HARDWARE_H_ */
