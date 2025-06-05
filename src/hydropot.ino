// Project: Irrigation System for two plants using Arduino Uno

//----------------------- INCLUDES -----------------------//
#include <Arduino.h>
#include "globals.h"
#include "checkSensors.h"
#include "watering.h"
#include "errorHandling.h"
#include "printing.h"
#include "setupSystem.h"

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
    watering(1); // Pass plant number for watering
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

  // Optional: print sensor readings and watering times periodically
  printing();
}
