#include "errorFound.h"
#include "../globals/globals.h"

void errorFound()
{
  // Visual indication of error: turn ON both LEDs
  digitalWrite(LED_PIN_1, HIGH); // Turn ON LED for plant 1 (error state)
  digitalWrite(LED_PIN_2, HIGH); // Turn ON LED for plant 2 (error state)

  // Safety action: ensure both pumps are turned OFF
  digitalWrite(RELAY_PIN_1, LOW); // Turn OFF pump for plant 1
  digitalWrite(RELAY_PIN_2, LOW); // Turn OFF pump for plant 2

  // Check if sensor values have returned to a normal range
  // and if an error flag was previously set
  if (!errorFlag1 && !errorFlag2)
  {
    state = IDLE;
  }
  else if (analogRead(SENSOR_PIN_1) < HUMIDITY_THRESHOLD_1 && errorFlag1)
  {
    // Reset error flag for plant 1
    errorFlag1 = false;

    // Debug message: error condition cleared
    Serial.print(" NOPE.");
    Serial.println("\t Plant 1 has been watered correctly.");

    // Return to idle state after error recovery
    state = IDLE;
  }
  else if (analogRead(SENSOR_PIN_2) < HUMIDITY_THRESHOLD_2 && errorFlag2)
  {
    // Reset error flag for plant 2
    errorFlag2 = false;

    // Debug message: error condition cleared
    Serial.print(" NOPE.");
    Serial.println("\t Plant 2 has been watered correctly.");

    // Return to idle state after error recovery
    state = IDLE;
  }
  else
  {
    // Error condition still present
    Serial.println(" ERROR.");
  }
}
