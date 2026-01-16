//==============================================================//
//  Author: Sara Spagnoli
//  Project: Automated Irrigation System for Two Plants (ESP32)
//
//  Description:
//  This project implements a fully automated irrigation system
//  designed to monitor soil moisture and water two independent
//  plants using an ESP32 microcontroller. The system periodically
//  reads humidity sensors, evaluates the moisture level, and
//  activates the corresponding water pump when irrigation is needed.
//
//  The control logic is implemented as a finite state machine (FSM)
//  with clearly defined states such as IDLE, WATER1, WATER2,
//  WAITING_MEASURE, WAITING_WATER_ABSORPTION, and ERROR.
//  Each state handles a specific part of the workflow, ensuring
//  predictable behavior and easier debugging.
//
//
//  This modular structure (globals, setup, sensors, watering,
//  error handling, printing) allows the system to be easily extended
//  or adapted for additional plants, different sensors, or more
//  advanced irrigation strategies.
//
//  Hardware Components:
//  - ESP32 development board for control, sensing, and state management
//  - Two RUNCCI-YUN DC 3–5 V mini submersible brushless water pumps
//  - Two ARCELI 5 V single-channel relay modules used to switch the pumps
//    safely and isolate the ESP32 from the load
//  - Two capacitive soil moisture sensors (3.3 V compatible) for
//    non-corrosive, stable humidity measurement
//  - Status LEDs for system feedback (e.g., WAIT state indication).
//==============================================================//


//----------------------- INCLUDES -----------------------//
#include <Arduino.h>
#include "src/globals/globals.h"
#include "src/setupSystem/setupSystem.h"
#include "src/checkSensors/checkSensors.h"
#include "src/watering/watering.h"
#include "src/errorFound/errorFound.h"
#include "src/printing/printing.h"

//----------------------- SETUP -----------------------//
void setup()
{
  setupSystem();
}

//----------------------- MAIN LOOP -----------------------//
void loop()
{
  switch (state)
  {
  case IDLE:
    Serial.println("\nState: IDLE");
    checkSensors();
    break;

  case WATER1:
    Serial.println("\nState: WATER1");
    watering(1);
    Serial.println("\t Plant 1 watered.");
    break;

  case WATER2:
    Serial.println("\nState: WATER2");
    watering(2);
    Serial.println("\t Plant 2 watered.");
    break;

  case WAITING_MEASURE:
    Serial.println("\nState: WAITING_MEASURE");
    delay(DELAY_READING);
    state = IDLE;
    break;

  case WAITING_WATER_ABSORPTION:
    Serial.println("\nState: WAITING_WATER_ABSORPTION");
    digitalWrite(LED_WAIT, HIGH);
    delay(DELAY_ONEDAY); // Normally 24h delay for water absorption
    digitalWrite(LED_WAIT, LOW);
    state = ERROR;
    break;

  case ERROR:
    Serial.println("\nState: ERROR");
    errorFound();
    break;
  }

  // Printing last measured humidity values and last watering times
  printing();
}
