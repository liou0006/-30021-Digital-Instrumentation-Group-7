# 9-DOF motion monitoring system with integrated accelerometer, gyroscope, and magnetometer and integrated data processing with FFT and histogram plotting using STM32 along with microSD data logging capabilities

This repository sub-directory contains the C code used for a project in the course 30021 Digital Instrumentation at DTU in Fall 2025. The C code uses the STM32CubeIDE and base project provided in the course along with two STM32 microcontrollers, one MBED expansion board, one LSM9DS1 IMU, and one OpenLog datalogger with corresponding micro SD card.

## Description

*The following description is taken from our project report.*

The goal of this project is to design and implement a motion monitoring instrument capable of measuring acceleration, angular velocity, and magnetic field using a 9-DOF IMU sensor. The system will be based on an STM32 microcontroller, featuring high-speed data acquisition, signal processing (FFT and histogram), and data logging capabilities.

The device will:
* Acquire data from a 3-axis accelerometer, 3-axis gyroscope, and 3-axis magnetometer when prompted by the user.
* Perform signal processing (e.g., data scaling corresponding to chosen sensor, along with computing FFT and histogram on the data of the chosen sensor's axis).
* Display results (FFT and histogram) on a the MBED expansion board's LCD.
* Log all measurements and processed data on an SD card for offline analysis.
* Include timing and interrupt-based sampling to ensure precise and synchronized measurements.
* This project will demonstrate a complete embedded measurement system, including sensing, digital signal processing, data logging, and user display.

## Getting Started

### Dependencies

* STM32CubeIDE

### Executing program

Each microcontroller has it own project. STM-A has ```SPIMCU``` and STM-B (with MBED expansion board) has ```JoystickDisplay```. You need two computers both with STM32CubeIDE to run the project. You run both projects by clicking run inside the STM32CubeIDE.

## Authors

* Mark Calum Malloy (s153679)
* Emma Louise Blair (s214680)
* Liou Xia (s224028)
* Guilherme Henriques de Almeida Terca (s243020)
* Francisco Cadima Almeida (s243042)

