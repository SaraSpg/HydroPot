# ESP32 Irrigation System for Two Plants

## Overview

This project implements an automated irrigation system based on an ESP32, designed to water two plants independently according to real‑time soil moisture measurements.  
The system reads two capacitive moisture sensors, controls two submersible pumps through relay modules, and uses LEDs to indicate system activity and waiting states.  
A finite state machine (FSM) ensures predictable behavior, modularity, and robust error handling.

## Features

- Continuous monitoring of soil moisture using two capacitive sensors (3.3 V compatible)
- Independent irrigation of each plant when moisture drops below a configurable threshold
- Finite State Machine (FSM) with states:  
  **IDLE**, **WATER1**, **WATER2**, **WAITING_MEASURE**, **WAITING_WATER_ABSORPTION**, **ERROR**
- Error detection when watering does not sufficiently increase soil moisture
- Serial logging of sensor readings, watering events, and system state transitions
- Modular code structure for easy maintenance and scalability

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
|------------------|-----------|
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

These parameters ensure stable operation, prevent over‑watering, and allow the soil to absorb water before re‑evaluation.

## Software Structure

- **`globals.h` / `globals.cpp`**  
  Pin definitions, thresholds, global variables, and FSM state enumeration.

- **`setupSystem.cpp`**  
  Initializes pins, serial communication, and system variables.

- **`checkSensors.cpp`**  
  Reads moisture sensors, evaluates thresholds, and updates the FSM state.

- **`watering.cpp`**  
  Controls pumps and LEDs, manages watering duration, and updates counters and timestamps.

- **`errorFound.cpp`**  
  Handles error conditions when watering does not improve soil moisture.

- **`printing.cpp`**  
  Outputs sensor readings, system state, and watering logs to the Serial Monitor.

- **Main sketch (`.ino`)**  
  Implements the FSM loop and orchestrates the system by calling the appropriate modules.

---

Enjoy your automated plant watering system 🌱💧