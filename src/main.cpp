#include "Arduino.h"
#include "globals/globals.h"
#include "setupSystem/setupSystem.h"
#include "checkSensors/checkSensors.h"
#include "watering/watering.h"
#include "errorFound/errorFound.h"
#include "printing/printing.h"
#include "telegram/telegram.h"


// ============================================================
// SETUP
// ============================================================

void setup()
{
    // Initialize the ESP32 pins, Serial communication,
    // and the initial system configuration.
    setupSystem();

    // Connect to Wi-Fi and initialize the Telegram bot.
    telegramSetup();
}


// ============================================================
// MAIN LOOP
// ============================================================

void loop()
{
    // Check for new Telegram messages at every loop iteration.
    // Telegram monitoring runs independently from the FSM.
    telegramHandleMessages();


    // ========================================================
    // FINITE STATE MACHINE
    // ========================================================

    switch (state)
    {
    // --------------------------------------------------------
    // IDLE
    // --------------------------------------------------------
    // Normal monitoring state.
    // The two moisture sensors are read and their values
    // are compared with the configured thresholds.
    //
    // Possible transitions:
    //   IDLE -> WATER1
    //   IDLE -> WATER2
    //   IDLE -> WAITING_MEASURE
    //   IDLE -> ERROR
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
    // Plant 1 requires watering.
    // The corresponding pump is activated for the configured
    // watering duration.
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
    // Plant 2 requires watering.
    // The corresponding pump is activated for the configured
    // watering duration.
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
    // This prevents the sensors from being read continuously
    // and gives the system time before starting a new cycle.
    //
    // After the delay, the system returns to IDLE.
    // --------------------------------------------------------

    case WAITING_MEASURE:

        Serial.println("\nState: WAITING_MEASURE");

        // Wait before taking another sensor measurement.
        delay(DELAY_READING);

        // Return to the normal monitoring state.
        state = IDLE;

        break;


    // --------------------------------------------------------
    // WAITING_WATER_ABSORPTION
    // --------------------------------------------------------
    // After watering, the system waits for the water to be
    // absorbed by the soil before evaluating the moisture again.
    //
    // The 24-hour waiting period is implemented using millis()
    // instead of delay(), so the ESP32 can continue handling
    // Telegram messages during this period.
    //
    // After the waiting period, the system enters ERROR.
    // --------------------------------------------------------

    case WAITING_WATER_ABSORPTION:

        Serial.println("\nState: WAITING_WATER_ABSORPTION");

        // Indicate that the system is currently in the
        // water absorption waiting period.
        digitalWrite(LED_WAIT, HIGH);

        // Store the starting time only once when entering
        // the waiting period.
        if (waitingWaterAbsorptionStart == 0)
        {
            waitingWaterAbsorptionStart = millis();
        }

        // Check whether the configured absorption time has elapsed.
        if (millis() - waitingWaterAbsorptionStart >= DELAY_ONEDAY)
        {
            // Turn off the waiting indicator.
            digitalWrite(LED_WAIT, LOW);

            // Reset the timer so it can be used again
            // during the next watering cycle.
            waitingWaterAbsorptionStart = 0;

            // After the absorption period, the system performs
            // the error-checking procedure.
            state = ERROR;
        }

        break;


    // --------------------------------------------------------
    // ERROR
    // --------------------------------------------------------
    // Error checking state.
    // The system verifies whether the watering operation
    // produced the expected change in soil moisture.
    //
    // The errorFound() function determines how to handle
    // the result and whether the system can return to normal
    // operation.
    // --------------------------------------------------------

    case ERROR:

        Serial.println("\nState: ERROR");

        // Check whether the previous watering operation
        // was successful or whether an error was detected.
        errorFound();

        break;
    }


    // ========================================================
    // SERIAL MONITOR
    // ========================================================

    // Print the latest sensor readings and watering information
    // after every FSM iteration.
    printing();
}