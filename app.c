/**
 * @file app.c
 * @brief Main application for the Clock Application.
 *
 * @author Alejandro Barranco-Leyte
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "app.h"
#include "hardware.h"


int main(int argc, char **argv) {

	printf("Welcome Clock Application\n\n");

	Time* savedTime = readTimeFromMemory(); // Read time from memory
	displayTime(savedTime); // Display Time



	return( 0 );
}
