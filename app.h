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

typedef enum {
	OFF, VIEW_MODE, MODIFY_MODE
} AppState;

#include "hardware.h"

extern volatile AppState current_state;

void viewMode();
void modifyMode();
void* stateCheckThread(void *arg);

#endif /* APP_H_ */
