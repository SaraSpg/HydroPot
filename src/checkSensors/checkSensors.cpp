#include "checkSensors.h"

void checkSensors()
{
  // Read raw analog values from the two soil moisture sensors
  valueSensor1 = analogRead(SENSOR_PIN_1);
  valueSensor2 = analogRead(SENSOR_PIN_2);

  // Check if sensor 1 is out of the valid range (THRESHOLD - 1000 to THRESHOLD + 500)
  if (valueSensor1 < (HUMIDITY_THRESHOLD_1 - 1000) || valueSensor1 > (HUMIDITY_THRESHOLD_1 + 500)) 
    errorFlag1 = 1;

  // Check if sensor 2 is out of the valid range (THRESHOLD - 1000 to THRESHOLD + 500)
  if (valueSensor2 < (HUMIDITY_THRESHOLD_2 - 1000) || valueSensor2 > (HUMIDITY_THRESHOLD_2 + 500)) 
    errorFlag2 = 1;

  // Highest priority: if an error was detected on any sensor,
  // force the system into the ERROR state
  if (errorFlag1 || errorFlag2) 
    state = ERROR;

  // ---- SENSOR 1 CHECK (Plant 1 has implicit priority) ----
  // If sensor 1 indicates soil is drier than the configured threshold
  else if (valueSensor1 > HUMIDITY_THRESHOLD_1)
  {
    // Turn on LED to indicate plant 1 needs attention
    digitalWrite(LED_PIN_1, HIGH);

    // If no error is detected, request watering for plant 1
    if (errorFlag1 == 0) state = WATER1;
  }

  // ---- SENSOR 2 CHECK (executed only if plant 1 does not need water) ----
  // Same logic as sensor 1, applied to plant 2
  else if (valueSensor2 > HUMIDITY_THRESHOLD_2)
  {
    // Turn on LED to indicate plant 2 needs attention
    digitalWrite(LED_PIN_2, HIGH);

    // If sensor is valid, request watering for plant 2
    if (errorFlag2 == 0) state = WATER2;
  }

  // ---- NORMAL CONDITION ----
  // No errors and no plant requires watering
  else
  {
    // Ensure all LEDs and relays are turned off (safe idle state)
    digitalWrite(LED_PIN_1, LOW);
    digitalWrite(LED_PIN_2, LOW);
    digitalWrite(RELAY_PIN_1, LOW);
    digitalWrite(RELAY_PIN_2, LOW);

    // Reset error flags since conditions are back to normal
    errorFlag1 = 0;
    errorFlag2 = 0;

    // Move to a waiting state to avoid continuous sensor polling
    state = WAITING_MEASURE;
  }
}

void updateSensorReadings()
{
  valueSensor1 = analogRead(SENSOR_PIN_1);
  valueSensor2 = analogRead(SENSOR_PIN_2);
}