# Clock Application

Welcome to the Clock Application project! This project is developed as an embedded systems application running on the DE10-Standard development board. The application is implemented in C and utilizes the ARM HPS and FPGA on the board to create a simple clock application.

## Project Description

The Clock Application simulates a simple clock with military time format, displayed on four seven-segment displays (HEX2 through HEX5). Users can view and modify the time using the board's buttons. The FPGA is used to control the displays via a BCD decoder, which will be implemented as taught in class.

### Features

- Displays the time in 24-hour military format.
- Uses buttons on the DE10-Standard board for user input:
  - **KEY0**: Toggle between time display and time modification modes.
  - **KEY1**: Move to the next digit for modification in time change mode.
  - **KEY2**: Increment the current digit in time change mode.
  - **KEY3**: Decrement the current digit in time change mode.
- Flashes the current digit being modified for user clarity.
- Initial default time displayed is `12:00`.

## Functional Requirements

- **Display Time**: As a user, you should be able to view the displayed time.
- **Change Time Mode**: As a user, you should be able to enter the “change time” mode using KEY0.
- **Modify Digits**: In the “change time” mode, use KEY2 to increase and KEY3 to decrease the current digit value.
- **Navigate Digits**: Use KEY1 to move between digits in the “change time” mode.
- **Save Changes**: Return to the “display time” mode, and any changes should be saved and reflected.

## Code Structure

- `app.c`: Main application logic handling the display and user interaction.
- `hardware.c`: Interface with the hardware components, including reading inputs and controlling the displays.
- `hardware.h` and `app.h`: Header files for function declarations and structure definitions.

## Building the Project

The project is compiled using a Makefile optimized for the DE10-Standard development board. Ensure that you have the necessary cross-compilation tools installed.

### Prerequisites

- ARM cross-compilation tools (`arm-linux-gnueabihf-gcc`).
- SoC EDS installed with proper path links (check `SOCEDS_DEST_ROOT`).

### Build Instructions

1. Clone the repository to your development environment.
2. Ensure that the `SOCEDS_DEST_ROOT` environment variable is set correctly.
3. Run `make` to build the application:

   ```bash
   make
   ```
4. The executable ClockApplication will be generated.

### Clean Built files

1. To clean up build artifacts, run:
    ```bash
    make clean
    ```
