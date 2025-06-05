# Arduino Irrigation System for Two Plants

## Overview

This project implements an automated irrigation system using Arduino Uno, designed to water two plants based on soil moisture sensor readings. The system manages two pumps and two LEDs to indicate watering status.

## Features

- Measures soil moisture using two sensors.
- Waters each plant independently when moisture drops below a threshold.
- Uses a finite state machine (FSM) to manage system states: IDLE, WATER1, WATER2, WAITING, and ERROR.
- Includes error detection if watering does not improve soil moisture within a given time.
- Provides serial output for monitoring sensor values, watering events, and system states.

## Hardware Components

- Arduino Uno
- 2 Soil moisture sensors (analog)
- 2 Water pumps controlled via relays
- 2 LEDs for status indication
- Relay modules to switch pumps on/off

## Pin Configuration

| Component         | Pin Number |
| ----------------- | ---------- |
| Sensor 1          | A0         |
| Sensor 2          | A1         |
| Relay Pump 1      | 8          |
| Relay Pump 2      | 9          |
| LED 1             | 4          |
| LED 2             | 5          |
| LED Waiting State | 8          |

## Thresholds and Timing

- Moisture Threshold Sensor 1: 500
- Moisture Threshold Sensor 2: 500
- Watering Duration: 3000 ms
- Reading Interval: 3000 ms
- Error Margin and Water Absorption Wait times implemented for robustness.

## Software Structure

- `globals.h` / `globals.cpp`: Contains all global variables, constants, and system state enumeration.
- `watering.cpp`: Manages the watering process including pump and LED control.
- `checkSensors.h`: Checks sensor values and updates system states accordingly.
- `errorFound.cpp`: Handles error states when watering is not successful.
- `printing.cpp`: Prints sensor values and watering logs to Serial Monitor.
- `hydropot.ino`: The main Arduino sketch implementing the FSM and calling the appropriate functions.

Enjoy your automated plant watering system! 🌱💧
