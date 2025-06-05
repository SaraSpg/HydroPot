#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

// Dichiariamo la funzione (prototipo)
void checkSensors();

// Variabili e stato esterni che servono alla funzione
extern int valueSensor1;
extern int valueSensor2;
extern int errorFlag1;
extern int errorFlag2;

extern const int SENSOR_PIN_1;
extern const int SENSOR_PIN_2;
extern const int LED_PIN_1;
extern const int LED_PIN_2;
extern const int RELAY_PIN_1;
extern const int RELAY_PIN_2;

extern const int HUMIDITY_THRESHOLD_1;
extern const int HUMIDITY_THRESHOLD_2;
extern const int HUMIDITY_MARGIN_ERROR;

extern void printing();

enum state_type;
extern state_type state;

extern const state_type IDLE;
extern const state_type WATER1;
extern const state_type WATER2;
extern const state_type WAITING_MEASURE;
extern const state_type ERROR;

#endif
