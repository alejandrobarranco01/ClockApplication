#
TARGET = ClockApplication

ALT_DEVICE_FAMILY ?= soc_cv_av
SOCEDS_ROOT ?= $(SOCEDS_DEST_ROOT)
HWLIBS_ROOT = $(SOCEDS_ROOT)/ip/altera/hps/altera_hps/hwlib
CROSS_COMPILE = arm-linux-gnueabihf-
CFLAGS = -g -Wall -std=c99 -D$(ALT_DEVICE_FAMILY) -I$(HWLIBS_ROOT)/include/$(ALT_DEVICE_FAMILY)   -I$(HWLIBS_ROOT)/include/
LDFLAGS =  -g -Wall 
CC = $(CROSS_COMPILE)gcc
ARCH= arm

# List of source files
SRCS = app.c hardware.c   # Include all relevant source files here

# Object files generated from source files
OBJS = $(SRCS:.c=.o)

build: $(TARGET)

# Link object files to create the final executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) *.a *.o *~
	
.PHONY: all
all: $(TARGET)