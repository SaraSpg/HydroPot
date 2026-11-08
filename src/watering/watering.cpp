
#include "Arduino.h"
#include "watering.h"
#include "../globals/globals.h"
#include "../telegram/telegram.h"


// =====================================================
// WATERING
// =====================================================

void watering(int plantNumber)
{
    // =================================================
    // PLANT 1 — ENGLISH IVY
    // =================================================

    if (plantNumber == 1)
    {
        Serial.println("\nWatering Plant 1 — English Ivy");

        // Activate pump 1 and its status LED.
        digitalWrite(RELAY_PIN_1, HIGH);
        digitalWrite(LED_PIN_1, HIGH);

        // Safety: ensure pump 2 is OFF.
        digitalWrite(RELAY_PIN_2, LOW);
        digitalWrite(LED_PIN_2, LOW);

        // Keep pump active for the configured duration.
        delay(DELAY_WATERING_1);

        // Record watering statistics.
        bool firstWatering = (wateringCounter1 == 0);

        unsigned long currentTime = millis();

        unsigned long elapsedTime = 0;

        if (!firstWatering)
        {
            elapsedTime = currentTime - lastWateringTime1;
        }

        wateringCounter1++;

        lastWateringTime1 = currentTime;

        // Send Telegram watering notification.
        sendWateringNotification(
            1,
            elapsedTime,
            firstWatering
        );

        // Set error flag:
        // Plant 1 must be checked after water absorption.
        errorFlag1 = true;

        // Deactivate pump 1 and its LED.
        digitalWrite(RELAY_PIN_1, LOW);
        digitalWrite(LED_PIN_1, LOW);
    }


    // =================================================
    // PLANT 2 — ANTHURIUM POLYSCHISTUM
    // =================================================

    else if (plantNumber == 2)
    {
        Serial.println("\nWatering Plant 2 — Anthurium polyschistum");

        // Activate pump 2 and its status LED.
        digitalWrite(RELAY_PIN_2, HIGH);
        digitalWrite(LED_PIN_2, HIGH);

        // Safety: ensure pump 1 is OFF.
        digitalWrite(RELAY_PIN_1, LOW);
        digitalWrite(LED_PIN_1, LOW);

        // Keep pump active for the configured duration.
        delay(DELAY_WATERING_2);

        // Record watering statistics.
        bool firstWatering = (wateringCounter2 == 0);

        unsigned long currentTime = millis();

        unsigned long elapsedTime = 0;

        if (!firstWatering)
        {
            elapsedTime = currentTime - lastWateringTime2;
        }

        wateringCounter2++;

        lastWateringTime2 = currentTime;

        // Send Telegram watering notification.
        sendWateringNotification(
            2,
            elapsedTime,
            firstWatering
        );

        // Set error flag:
        // Plant 2 must be checked after water absorption.
        errorFlag2 = true;

        // Deactivate pump 2 and its LED.
        digitalWrite(RELAY_PIN_2, LOW);
        digitalWrite(LED_PIN_2, LOW);
    }


    // =================================================
    // WATER ABSORPTION WAITING PERIOD
    // =================================================

    else
    {
        Serial.println("\nInvalid plant number. Watering aborted.");
        return;
    }

    // Reset the absorption timer.
    // This is important when a manual watering request
    // occurs during an existing absorption period.
    waitingWaterAbsorptionStart = 0;

    // Enter the waiting state after every watering operation.
    state = WAITING_WATER_ABSORPTION;

    Serial.println("\t Watering completed.");
    Serial.println("\t Waiting for water absorption.");
}
