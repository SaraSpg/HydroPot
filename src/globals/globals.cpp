#include "globals.h"

// ----------- Pin Definitions ----------- //
const int SENSOR_PIN_1 = 34;
const int SENSOR_PIN_2 = 35;
const int RELAY_PIN_1 = 25;
const int RELAY_PIN_2 = 26;
const int LED_PIN_1   = 18;
const int LED_PIN_2   = 19;
const int LED_WAIT    = 21;

// ----------- Thresholds ----------- //
const int HUMIDITY_THRESHOLD_1 = 150; //TODO: Check thresholds
const int HUMIDITY_THRESHOLD_2 = 100;
const int HUMIDITY_MARGIN_ERROR = 100;
const unsigned long DELAY_WATERING = 3000;
const unsigned long DELAY_READING  = 2000;
const unsigned long DELAY_ONEDAY   = 86400000UL;

// ----------- Stato ----------- //
SystemState state = IDLE;

// ----------- Variabili globali ----------- //
int valueSensor1 = 0;
int valueSensor2 = 0;
bool errorFlag1 = false;
bool errorFlag2 = false;

int wateringCounter1 = 0;
int wateringCounter2 = 0;
unsigned long lastWateringTime1 = 0;
unsigned long lastWateringTime2 = 0;
unsigned long waitingWaterAbsorptionStart = 0;
