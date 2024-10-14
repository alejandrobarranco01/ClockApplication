# Clock Application

This is my Clock Application projectfor SWE-350. This project will run on the DE10-Standard development board. The application is implemented in C and will utilize the ARM HPS and FPGA on the board to create a simple clock application.

## Author

Alejandro Barranco-Leyte

## Project Description

The application simulates a simple clock with military time format, displayed on four seven-segment displays (HEX2 through HEX5). Users can view and modify the time using the board's buttons. The FPGA will be used to control the displays via a BCD decoder.

### Features

- Displays the time in 24-hour military format.
- Uses buttons on the DE10-Standard board for user input:
  - **KEY0**: Toggle between "Display Time" and "Change Time" mode.
  - **KEY1**: Move on to the next digit in time change mode.
  - **KEY2**: Increment the current digit in time change mode.
  - **KEY3**: Decrement the current digit in time change mode.
- Flashes the current digit being modified for user clarity.
- Initial default time displayed is `12:00`.

## Code Structure

- `app.c`: Main application logic handling the display and user interaction.
- `hardware.c`: Interface with the hardware components, including reading inputs and controlling the displays.
- `hardware.h` and `app.h`: Header files for function declarations and structure definitions.

## Building the Project

The project is compiled using a Makefile optimized for the DE10-Standard development board. Ensure that you have the necessary cross-compilation tools installed.

### Prerequisites

- ARM cross-compilation tools (`arm-linux-gnueabihf-gcc`).

### Build Instructions

1. Clone the repository to your development environment.
2. Run `make` to build the application:

   ```bash
   make
   ```
3. The executable ClockApplication will be generated.

### Clean Built files

1. To clean up build artifacts, run:
    ```bash
    make clean
    ```
