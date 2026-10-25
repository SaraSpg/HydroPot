# ESP32 Irrigation System for Two Plants

## Overview

This project implements an automated irrigation system based on an ESP32, designed to water two plants independently according to real‑time soil moisture measurements.  
The system reads two capacitive moisture sensors, controls two pumps through relay modules, and uses LEDs to indicate system activity and waiting states.  
A finite state machine (FSM) ensures predictable behavior, modularity, and robust error handling.
A Telegram bot is also implemented for remote monitoring.

## Features

- Continuous soil moisture monitoring using two capacitive sensors
- Independent automatic irrigation for each plant
- Configurable moisture thresholds
- Finite State Machine (FSM) with the following states:
  **IDLE**, **WATER1**, **WATER2**, **WAITING_MEASURE**, **WAITING_WATER_ABSORPTION**, **ERROR**
- Watering error detection
- Pump and LED control
- Serial monitoring and system logging
- Telegram remote monitoring
- Modular code structure for easy maintenance and expansion

## Hardware Components

- **ESP32 development board**
- **2 capacitive soil moisture sensors** 
- **2 pumps**
- **2 relay modules**
- **flexible transparent PVC tubing** 
- **Status LEDs** 
- External power supply (5V)

## Pin Configuration (ESP32)

| Component         | ESP32 Pin |
|-------------------|-----------|
| Sensor 1          | 34        |
| Sensor 2          | 35        |
| Relay Pump 1      | 25        |
| Relay Pump 2      | 26        |
| LED Plant 1       | 17        |
| LED Plant 2       | 2        |
| LED Waiting State | 21        |

## Thresholds and Timing

- **Moisture Threshold Sensor 1:** 2700  
- **Moisture Threshold Sensor 2:** 2200  
- **Error Margin:** 500  
- **Watering Duration:** 2000 ms  
- **Sensor Reading Interval:** 2000 ms  
- **Water Absorption Delay:** 24 h (86 400 000 ms)

## Software Structure

- **`globals.h` / `globals.cpp`**  
  Defines the ESP32 pins, system thresholds, timing parameters, global variables, and FSM states.

- **`setupSystem.h` / `setupSystem.cpp`**  
  Initializes the ESP32, GPIO pins, Serial communication, and initial system variables.

- **`checkSensors.h` / `checkSensors.cpp`**  
  Reads the two moisture sensors, compares their values with the configured thresholds, and determines whether watering is required.

- **`watering.h` / `watering.cpp`**  
  Controls the pumps and corresponding LEDs, manages the watering duration, and records watering events and timestamps.

- **`errorFound.h` / `errorFound.cpp`**  
  Handles error conditions and checks whether watering has produced the expected change in soil moisture.

- **`printing.h` / `printing.cpp`**  
  Displays sensor readings and watering information on the Serial Monitor.

- **`telegram.h` / `telegram.cpp`**  
  Manages the Wi-Fi connection and Telegram bot for remote monitoring and watering notifications.

- **`timeUtils.h` / `timeUtils.cpp`**  
  Provides utility functions for formatting elapsed time.

- **`main.cpp`**  
  Contains the main `setup()` and `loop()` functions and manages the FSM by calling the appropriate modules.

---