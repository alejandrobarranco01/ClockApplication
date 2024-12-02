/**
 * @file utility.h
 * @brief Header file for other misc functions.
 *
 * @author Alejandro Barranco-Leyte
 */

#ifndef UTILITY_H_
#define UTILITY_H_

int isValidHex(unsigned int val);
unsigned int bcd2sevenSegmentDecoder(int digit);
unsigned char getHexForLetter(char letter);

#endif /* UTILITY_H_ */
