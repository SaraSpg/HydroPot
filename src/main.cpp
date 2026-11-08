
#include "Arduino.h"
#include "globals/globals.h"
#include "setupSystem/setupSystem.h"
#include "checkSensors/checkSensors.h"
#include "watering/watering.h"
#include "errorFound/errorFound.h"
#include "printing/printing.h"
#include "telegram/telegram.h"


// ========================================================
// MANUAL WATERING REQUEST
// ========================================================

// Called by the Telegram module to request manual watering.
//
// plantNumber:
//   1 -> Plant 1 (English Ivy)
//   2 -> Plant 2 (Anthurium polyschistum)
//
// Manual watering is accepted only when the FSM is IDLE.

bool requestManualWatering(uint8_t plantNumber)
{
    // Do not interrupt an active watering operation.
    if (state == WATER1 || state == WATER2)
    {
        Serial.println(
            "\nManual watering rejected: pump already active."
        );

        return false;
    }

    // Request watering of Plant 1.
    if (plantNumber == 1)
    {
        Serial.println(
            "\nManual watering requested: Plant 1."
        );

        state = WATER1;
        return true;
    }

    // Request watering of Plant 2.
    else if (plantNumber == 2)
    {
        Serial.println(
            "\nManual watering requested: Plant 2."
        );

        state = WATER2;
        return true;
    }

    return false;
}


// ========================================================
// SETUP
// ========================================================

void setup()
{
    // Initialize ESP32 pins, Serial communication
    // and the initial system configuration.
    setupSystem();

    // Connect to Wi-Fi and initialize Telegram.
    telegramSetup();
}


// ========================================================
// MAIN LOOP
// ========================================================

void loop()
{
    // --------------------------------------------------------
    // TELEGRAM
    // --------------------------------------------------------

    // Check for new Telegram messages.
    // This also allows manual watering requests.
    telegramHandleMessages();

    // Check whether a scheduled plant status report
    // must be sent (08:00 or 20:00).
    telegramScheduledReports();


    // ========================================================
    // IRRIGATION FSM
    // ========================================================

    switch (state)
    {

    // --------------------------------------------------------
    // IDLE
    // --------------------------------------------------------
    // Normal monitoring state.
    //
    // Possible transitions:
    //   IDLE -> WATER1
    //   IDLE -> WATER2
    //   IDLE -> WAITING_MEASURE
    //   IDLE -> ERROR
    //
    // Manual Telegram requests are also accepted in this state.
    // --------------------------------------------------------

    case IDLE:

        Serial.println("\nState: IDLE");

        // Check the current soil moisture values
        // and determine the next state.
        checkSensors();

        break;


    // --------------------------------------------------------
    // WATER1
    // --------------------------------------------------------
    // Plant 1 (English Ivy) requires watering.
    //
    // The corresponding pump is activated for the
    // configured watering duration.
    //
    // After watering, the system enters the
    // WAITING_WATER_ABSORPTION state.
    // --------------------------------------------------------

    case WATER1:

        Serial.println("\nState: WATER1");

        // Activate the pump associated with Plant 1.
        watering(1);

        Serial.println("\t Plant 1 watered.");

        break;


    // --------------------------------------------------------
    // WATER2
    // --------------------------------------------------------
    // Plant 2 (Anthurium polyschistum) requires watering.
    //
    // The corresponding pump is activated for the
    // configured watering duration.
    //
    // After watering, the system enters the
    // WAITING_WATER_ABSORPTION state.
    // --------------------------------------------------------

    case WATER2:

        Serial.println("\nState: WATER2");

        // Activate the pump associated with Plant 2.
        watering(2);

        Serial.println("\t Plant 2 watered.");

        break;


    // --------------------------------------------------------
    // WAITING_MEASURE
    // --------------------------------------------------------
    // Short waiting period between sensor measurements.
    //
    // After the delay, the system returns to IDLE.
    // --------------------------------------------------------

    case WAITING_MEASURE:

        Serial.println("\nState: WAITING_MEASURE");

        // Wait before taking another sensor measurement.
        delay(DELAY_READING);

        // Return to normal monitoring.
        state = IDLE;

        break;


    // --------------------------------------------------------
    // WAITING_WATER_ABSORPTION
    // --------------------------------------------------------
    // After watering, the system waits for the water
    // to be absorbed by the soil.
    //
    // The waiting period uses millis(), allowing Telegram
    // messages and scheduled reports to be processed.
    //
    // After the waiting period, the system enters ERROR.
    // --------------------------------------------------------

    case WAITING_WATER_ABSORPTION:

        Serial.println("\nState: WAITING_WATER_ABSORPTION");

        // Activate the waiting indicator.
        digitalWrite(LED_WAIT, HIGH);

        // Store the starting time only once.
        if (waitingWaterAbsorptionStart == 0)
        {
            waitingWaterAbsorptionStart = millis();
        }

        // Check whether the absorption time has elapsed.
        if (millis() - waitingWaterAbsorptionStart >= DELAY_ONEDAY)
        {
            // Turn off the waiting indicator.
            digitalWrite(LED_WAIT, LOW);

            // Reset the timer.
            waitingWaterAbsorptionStart = 0;

            // Perform the error-checking procedure.
            state = ERROR;
        }

        break;


    // --------------------------------------------------------
    // ERROR
    // --------------------------------------------------------
    // Check whether the previous watering operation
    // was successful or whether an error was detected.
    // --------------------------------------------------------

    case ERROR:

        Serial.println("\nState: ERROR");

        // Evaluate the previous watering operation.
        errorFound();

        break;
    }


    // ========================================================
    // SERIAL MONITOR
    // ========================================================

    // Print the latest sensor readings and watering information.
    printing();
}
