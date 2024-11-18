/**
 * @file app.h
 * @brief Header file for the Clock Application.
 *
 * @author Alejandro Barranco-Leyte
 */

#ifndef APP_H_
#define APP_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>

#include "hardware.h"

int changeTimeMode(Time *time);

#endif /* APP_H_ */
