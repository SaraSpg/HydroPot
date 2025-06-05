#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

// ----------- Pin Definitions ----------- //
extern const int SENSOR_PIN_1;
extern const int SENSOR_PIN_2;
extern const int RELAY_PIN_1;
extern const int RELAY_PIN_2;
extern const int LED_PIN_1;
extern const int LED_PIN_2;

// ----------- Thresholds ----------- //
extern const int HUMIDITY_THRESHOLD_1;
extern const int HUMIDITY_THRESHOLD_2;
extern const int HUMIDITY_MARGIN_ERROR;
extern const unsigned long DELAY_WATERING;

// ----------- State Machine ----------- //
enum SystemState
{
  IDLE,
  WAITING_MEASURE,
  WATER1,
  WATER2,
  WAITING_WATER_ABSORPTION,
  ERROR
};
extern SystemState state;

// ----------- Sensor and Control Variables ----------- //
extern int valueSensor1;
extern int valueSensor2;
extern bool errorFlag1;
extern bool errorFlag2;

extern int wateringCounter1;
extern int wateringCounter2;
extern unsigned long lastWateringTime1;
extern unsigned long lastWateringTime2;

// ------------ Functions Declarations ------------ //
void checkSensors();
void watering();
void errorFound();
void printing();
void printingDate(unsigned long time);
