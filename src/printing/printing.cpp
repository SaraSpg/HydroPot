#include "printing.h"

#include "../globals/globals.h"
#include "../timeUtils/timeUtils.h"

void printing()
{
    // -------------------------
    // Sensor values & Humidity
    // -------------------------

    // Calculate humidity percentage: higher analog readings mean drier soil.
    // When the sensor value reaches the threshold, moisture drops to 0% (triggering watering).
    int humidityPercent1 = constrain(map(valueSensor1, HUMIDITY_THRESHOLD_1, HUMIDITY_THRESHOLD_1 - 1000, 0, 100), 0, 100);
    int humidityPercent2 = constrain(map(valueSensor2, HUMIDITY_THRESHOLD_2, HUMIDITY_THRESHOLD_2 - 1000, 0, 100), 0, 100);

    Serial.print("Sensor 1: ");
    Serial.print(valueSensor1);
    Serial.print(" | Humidity: ");
    Serial.print(humidityPercent1);
    Serial.println("%");

    Serial.print("Sensor 2: ");
    Serial.print(valueSensor2);
    Serial.print(" | Humidity: ");
    Serial.print(humidityPercent2);
    Serial.println("%");

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