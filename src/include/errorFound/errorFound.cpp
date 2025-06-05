#include "errorFound.h"
#include "../globals.h"  // Per usare pin, variabili globali e stato

void errorFound()
{
  digitalWrite(LED_PIN_1, HIGH);  // Accendi LED 1 per indicare errore
  digitalWrite(LED_PIN_2, HIGH);  // Accendi LED 2 per indicare errore
  digitalWrite(RELAY_PIN_1, LOW); // Spegni pompa 1
  digitalWrite(RELAY_PIN_2, LOW); // Spegni pompa 2

  // Nota: usare && (AND logico) non & (AND bit a bit)
  if (analogRead(SENSOR_PIN_1) < HUMIDITY_THRESHOLD_1 && errorFlag1)
  {
    errorFlag1 = false; // Resetta flag errore
    Serial.print(" NOPE.");
    Serial.println("\t Plant 1 has been watered correctly.");
    state = IDLE;
  }
  else if (analogRead(SENSOR_PIN_2) < HUMIDITY_THRESHOLD_2 && errorFlag2)
  {
    errorFlag2 = false; // Resetta flag errore
    Serial.print(" NOPE.");
    Serial.println("\t Plant 2 has been watered correctly.");
    state = IDLE;
  }
  else
  {
    Serial.println(" ERROR.");
  }
}
