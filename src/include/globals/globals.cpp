#include "globals.h"

// ----------- Pin Definitions ----------- //
const int SENSOR_PIN_1 = A0;
const int SENSOR_PIN_2 = A1;
const int RELAY_PIN_1 = 8;
const int RELAY_PIN_2 = 9;
const int LED_PIN_1 = 4;
const int LED_PIN_2 = 5;

// ----------- Thresholds ----------- //
const int HUMIDITY_THRESHOLD_1 = 500;
const int HUMIDITY_THRESHOLD_2 = 500;
const int HUMIDITY_MARGIN_ERROR = 100;
const unsigned long DELAY_WATERING = 3000;

// ----------- State Machine ----------- //
SystemState state = IDLE;

// ----------- Sensor and Control Variables ----------- //
int valueSensor1 = 0;
int valueSensor2 = 0;
bool errorFlag1 = false;
bool errorFlag2 = false;

int wateringCounter1 = 0;
int wateringCounter2 = 0;
unsigned long lastWateringTime1 = 0;
unsigned long lastWateringTime2 = 0;
