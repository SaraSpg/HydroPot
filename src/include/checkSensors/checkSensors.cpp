#include "sensors.h"

void checkSensors()
{
  valueSensor1 = analogRead(SENSOR_PIN_1);
  valueSensor2 = analogRead(SENSOR_PIN_2);
  printing();

  if (errorFlag1 || errorFlag2)
  {
    state = ERROR;
  }
  else if (valueSensor1 > HUMIDITY_THRESHOLD_1)
  {
    digitalWrite(LED_PIN_1, HIGH);
    if (valueSensor1 > HUMIDITY_THRESHOLD_1 + HUMIDITY_MARGIN_ERROR)
    {
      errorFlag1 = 1;
    }
    else if (errorFlag1 == 0)
    {
      state = WATER1;
    }
  }
  else if (valueSensor2 > HUMIDITY_THRESHOLD_2)
  {
    digitalWrite(LED_PIN_2, HIGH);
    if (valueSensor2 > HUMIDITY_THRESHOLD_2 + HUMIDITY_MARGIN_ERROR)
    {
      errorFlag2 = 1;
    }
    else if (errorFlag2 == 0)
    {
      state = WATER2;
    }
  }
  else
  {
    digitalWrite(LED_PIN_1, LOW);
    digitalWrite(LED_PIN_2, LOW);
    digitalWrite(RELAY_PIN_1, LOW);
    digitalWrite(RELAY_PIN_2, LOW);
    errorFlag1 = 0;
    errorFlag2 = 0;
    state = WAITING_MEASURE;
  }
}
