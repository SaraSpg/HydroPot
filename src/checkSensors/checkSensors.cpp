#include "checkSensors.h"

void checkSensors()
{
  // Read raw analog values from the two soil moisture sensors
  valueSensor1 = analogRead(SENSOR_PIN_1);
  valueSensor2 = analogRead(SENSOR_PIN_2);

  // Highest priority: if an error was already detected on any sensor,
  // force the system into the ERROR state
  if (errorFlag1 || errorFlag2)
  {
    state = ERROR;
  }

  // ---- SENSOR 1 CHECK (Plant 1 has implicit priority) ----
  // If sensor 1 indicates soil is drier than the configured threshold
  else if (valueSensor1 > HUMIDITY_THRESHOLD_1)
  {
    // Turn on LED to indicate plant 1 needs attention
    digitalWrite(LED_PIN_1, HIGH);

    // If the value exceeds the threshold plus a safety margin,
    // this is considered an abnormal condition (sensor error)
    if (valueSensor1 > HUMIDITY_THRESHOLD_1 + HUMIDITY_MARGIN_ERROR)
    {
      errorFlag1 = 1;
    }
    // If no error is detected, request watering for plant 1
    else if (errorFlag1 == 0)
    {
      state = WATER1;
    }
  }

  // ---- SENSOR 2 CHECK (executed only if plant 1 does not need water) ----
  // Same logic as sensor 1, applied to plant 2
  else if (valueSensor2 > HUMIDITY_THRESHOLD_2)
  {
    // Turn on LED to indicate plant 2 needs attention
    digitalWrite(LED_PIN_2, HIGH);

    // Check for abnormal sensor readings beyond allowed margin
    if (valueSensor2 > HUMIDITY_THRESHOLD_2 + HUMIDITY_MARGIN_ERROR)
    {
      errorFlag2 = 1;
    }
    // If sensor is valid, request watering for plant 2
    else if (errorFlag2 == 0)
    {
      state = WATER2;
    }
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
