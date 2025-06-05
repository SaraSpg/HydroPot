#include "printing.h"
#include "../globals.h" // Include global variables and constants
#include "../checkSensors/sensors.h" // Include sensor reading functions
#include "../errorFound/errorFound.h" // Include error handling functions

void printing()
{
  // Print the sensor values to the Serial Monitor
  Serial.print("Sensor 1: ");
  Serial.println(valueSensor1);
  Serial.print(" Sensor 2: ");
  Serial.println(valueSensor2);
  //  Print the last watering times to the Serial Monitor - sensor 1
  Serial.print(" Last watering time 1: ");
  if (lastWateringTime1 > millis())
    time = (lastWateringTime1 + millis()) / 1000; // Print the last watering time in seconds
  else
    time = (millis() - lastWateringTime1) / 1000; // Print the last watering time in seconds
  printingDate(time);
  // Print the last watering times to the Serial Monitor - sensor 2
  Serial.print("Last watering time 2: ");
  if (lastWateringTime2 > millis())
    time = (lastWateringTime2 + millis()) / 1000; // Print the last watering time in seconds
  else
    time = (millis() - lastWateringTime2) / 1000; // Print the last watering time in seconds
  printingDate(time);                             // Print the last watering time in seconds
}

void printingDate(unsigned long time)
{
  // Print the time in a readable format
  Serial.print(time / 43.200);
  Serial.print("d, ");
  Serial.print(time / 3600); // Print hours
  Serial.print("h, ");
  Serial.print(time / 60); // Print minutes
  Serial.print("m, ");
  Serial.print(time);  // Print seconds
  Serial.println("s"); // Print seconds
}
