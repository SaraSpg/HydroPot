# ESP32 Irrigation System for Two Plants

## Overview

This project implements an automated irrigation system based on an ESP32, designed to water two plants independently according to real‑time soil moisture measurements.  
The system reads two capacitive moisture sensors, controls two submersible pumps through relay modules, and uses LEDs to indicate system activity and waiting states.  
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
- **2 capacitive soil moisture sensors (3.3 V)** for stable, corrosion‑free humidity measurement
- **2 RUNCCI‑YUN DC 3–5 V mini submersible brushless pumps**
- **3 m flexible transparent PVC tubing** for water delivery
- **2 ARCELI 5 V single‑channel relay modules** to switch the pumps safely
- **Status LEDs** for plant watering indication and waiting state feedback
- External power supply suitable for pumps and relays

## Pin Configuration (ESP32)

| Component         | ESP32 Pin |
|-------------------|-----------|
| Sensor 1          | 34        |
| Sensor 2          | 35        |
| Relay Pump 1      | 25        |
| Relay Pump 2      | 26        |
| LED Plant 1       | 18        |
| LED Plant 2       | 19        |
| LED Waiting State | 21        |

## Thresholds and Timing

- **Moisture Threshold Sensor 1:** 500  
- **Moisture Threshold Sensor 2:** 500  
- **Error Margin:** 100  
- **Watering Duration:** 3000 ms  
- **Sensor Reading Interval:** 2000 ms  
- **Water Absorption Delay:** 24 h (86 400 000 ms)

These parameters define the main operating conditions of the irrigation system. 
The moisture thresholds determine when watering is required, while the error margin is used to detect abnormal sensor readings after watering (if the humindity has decreased after watering, raise an error). 
The watering duration controls how long each pump remains active, and the reading interval determines how frequently the soil moisture is checked. 
After watering, the system waits 24 hours to allow the water to be absorbed by the soil before continuing the monitoring process.
**Note:** The moisture thresholds are defined separately for the two sensors because they are different sensor versions and may produce different readings under the same conditions.

## Software Structure

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

Enjoy your automated plant watering system 🌱💧