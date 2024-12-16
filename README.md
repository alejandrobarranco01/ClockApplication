# Clock Application

This is my Clock Application project for SWE-350. This project will run on the DE10-Standard development board. The application is implemented in C and will utilize the ARM HPS and FPGA on the board to create a simple clock application.

## Author

Alejandro Barranco-Leyte

Demo Video
Check out the video demo of the project here.

## Video Link

[Video Link](https://youtu.be/rhM_VRLb7cc)

## GitHub Link

[Github Link](https://github.com/alejandrobarranco01/ClockApplication)

## Project Description

The application simulates a simple clock using military time format, displayed on four seven-segment displays (HEX2 through HEX5). Users can view and modify the time, including hours, minutes, and seconds, using the board's switches and buttons. The FPGA manages the seven-segment displays via a BCD decoder.

### Features

- Displays the time in 24-hour military format.
- Uses buttons on the DE10-Standard board for user input:
  - **KEY0**: Toggle between "Display Time" and "Change Time" mode.
  - **KEY1**: Move on to the next digit in time change mode.
  - **KEY2**: Increment the current digit in time change mode.
  - **KEY3**: Decrement the current digit in time change mode.
- **SW9** turns the clock on and off.
- **SW0** resets the seconds while in time change mode.
- Flashes the current digit being modified for user clarity.
- The clock reads and saves the current time to a text file for persistence between sessions.

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

### FPGA Configuration

To configure the FPGA for the clock application:

1. **Synthesize the DE10_Standard_Computer QPF File:**
   - Open the Quartus Prime software.
   - Load the project file for your DE10-Standard board.
   - Synthesize the design by clicking on **Processing** > **Start Compilation**. This will generate the necessary configuration files.

2. **Use the Quartus Programmer Tool:**
   - Open the **Quartus Programmer** tool (you can find it under the **Tools** menu).
   - Connect the DE10-Standard board to your computer via the USB-Blaster or USB cable.
   - In the Quartus Programmer, select the appropriate hardware device (DE10-Standard).
   - Click on **Add File** and select the compiled output file (e.g., `.sof` file) generated from the synthesis step.
   - Click **Start** to upload the configuration to the FPGA.

3. **Verify the Configuration:**
   - After the FPGA configuration is complete, the clock application should now run on the DE10-Standard board, utilizing the FPGA resources for the seven-segment display and BCD decoder.
