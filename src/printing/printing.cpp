#include "printing.h"

#include "../globals/globals.h"
#include "../timeUtils/timeUtils.h"

void printing()
{
    // -------------------------
    // Sensor values
    // -------------------------

    Serial.print("Sensor 1: ");
    Serial.println(valueSensor1);

    Serial.print("Sensor 2: ");
    Serial.println(valueSensor2);

    // -------------------------
    // Plant 1
    // -------------------------

    Serial.print("Last watering time 1: ");

    if (lastWateringTime1 == 0)
    {
        Serial.println("Never watered");
    }
    else
    {
        unsigned long elapsedTime =
            (millis() - lastWateringTime1) / 1000;

        Serial.println(formatElapsedTime(elapsedTime));
    }

    // -------------------------
    // Plant 2
    // -------------------------

    Serial.print("Last watering time 2: ");

    if (lastWateringTime2 == 0)
    {
        Serial.println("Never watered");
    }
    else
    {
        unsigned long elapsedTime =
            (millis() - lastWateringTime2) / 1000;

        Serial.println(formatElapsedTime(elapsedTime));
    }
}