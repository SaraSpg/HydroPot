
#include "errorFound.h"
#include "../globals/globals.h"
#include "../telegram/telegram.h"

void errorFound()
{
    // Visual indication of error.
    digitalWrite(LED_PIN_1, HIGH);
    digitalWrite(LED_PIN_2, HIGH);

    // Safety: ensure both pumps are OFF.
    digitalWrite(RELAY_PIN_1, LOW);
    digitalWrite(RELAY_PIN_2, LOW);

    // Read both sensors independently.
    int sensorValue1 = analogRead(SENSOR_PIN_1);
    int sensorValue2 = analogRead(SENSOR_PIN_2);

    // =====================================================
    // PLANT 1 — ENGLISH IVY
    // =====================================================

    if (errorFlag1)
    {
        if (sensorValue1 < HUMIDITY_THRESHOLD_1)
        {
            // Plant 1 has recovered successfully.
            errorFlag1 = false;

            Serial.println("Plant 1: watering successful.");

            // Notify recovery (only if an error was reported).
            sendPlantRecoveryNotification(1);
        }
        else
        {
            // Plant 1 has not recovered.
            Serial.println("ERROR: Plant 1 has not recovered.");

            // This function prevents repeated notifications.
            sendPlantErrorNotification(1);
        }
    }

    // =====================================================
    // PLANT 2 — ANTHURIUM POLYSCHISTUM
    // =====================================================

    if (errorFlag2)
    {
        if (sensorValue2 < HUMIDITY_THRESHOLD_2)
        {
            // Plant 2 has recovered successfully.
            errorFlag2 = false;

            Serial.println("Plant 2: watering successful.");

            // Notify recovery (only if an error was reported).
            sendPlantRecoveryNotification(2);
        }
        else
        {
            // Plant 2 has not recovered.
            Serial.println("ERROR: Plant 2 has not recovered.");

            // This function prevents repeated notifications.
            sendPlantErrorNotification(2);
        }
    }

    // =====================================================
    // FSM STATE MANAGEMENT
    // =====================================================

    // Return to IDLE only when both plants have recovered.
    if (!errorFlag1 && !errorFlag2)
    {
        Serial.println("All plants have recovered. Returning to IDLE.");
        state = IDLE;
    }
    else
    {
        // At least one plant still needs attention.
        state = ERROR;
    }
}
