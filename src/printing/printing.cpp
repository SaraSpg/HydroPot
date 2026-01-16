#include "printing.h"
#include "../globals/globals.h" // Include global variables
#include "../checkSensors/checkSensors.h" // Include sensor checking functions
#include "../errorFound/errorFound.h" // Include error handling functions

void printing()
{
  Serial.print("Sensor 1: ");
  Serial.println(valueSensor1);
  Serial.print(" Sensor 2: ");
  Serial.println(valueSensor2);

  // Calcolo tempo trascorso per il primo sensore
  Serial.print(" Last watering time 1: ");
  unsigned long elapsedTime = 0;
  if (lastWateringTime1 > millis())
    elapsedTime = (lastWateringTime1 + millis()) / 1000;
  else
    elapsedTime = (millis() - lastWateringTime1) / 1000;
  printingDate(elapsedTime);

  // Calcolo tempo trascorso per il secondo sensore
  Serial.print("Last watering time 2: ");
  if (lastWateringTime2 > millis())
    elapsedTime = (lastWateringTime2 + millis()) / 1000;
  else
    elapsedTime = (millis() - lastWateringTime2) / 1000;
  printingDate(elapsedTime);
}

void printingDate(unsigned long seconds)
{
  Serial.print(seconds / 43200); // giorni (approssimato: 12h)
  Serial.print("d, ");
  Serial.print(seconds / 3600); // ore
  Serial.print("h, ");
  Serial.print(seconds / 60); // minuti
  Serial.print("m, ");
  Serial.print(seconds);  // secondi
  Serial.println("s");
}
