#include "Arduino.h"
#include "watering.h"
#include "../globals/globals.h" // Include global variables

void watering(int plantNumber) {
    // Handle watering logic for plant 1
    if (plantNumber == 1) {
        digitalWrite(RELAY_PIN_1, HIGH);  // Turn ON pump for plant 1
        digitalWrite(LED_PIN_1, HIGH);    // Turn ON status LED for plant 1

        // Ensure plant 2 is inactive while watering plant 1
        digitalWrite(RELAY_PIN_2, LOW);   // Turn OFF pump for plant 2
        digitalWrite(LED_PIN_2, LOW);     // Turn OFF LED for plant 2

        // Keep pump active for the configured watering duration
        delay(DELAY_WATERING);

        // Update watering statistics for plant 1
        wateringCounter1++;
        lastWateringTime1 = millis();
        
        //after the watering absorption we check and set low the error flag  
        errorFlag1 = true;

        // Stop watering and reset indicators
        digitalWrite(RELAY_PIN_1, LOW);   // Turn OFF pump for plant 1
        digitalWrite(LED_PIN_1, LOW);     // Turn OFF LED for plant 1

        // Move to waiting state to allow water absorption
        state = WAITING_WATER_ABSORPTION;
    } 

    // Handle watering logic for plant 2
    else if (plantNumber == 2) {
        digitalWrite(RELAY_PIN_2, HIGH);  // Turn ON pump for plant 2
        digitalWrite(LED_PIN_2, HIGH);    // Turn ON status LED for plant 2

        // Ensure plant 1 is inactive while watering plant 2
        digitalWrite(RELAY_PIN_1, LOW);   // Turn OFF pump for plant 1
        digitalWrite(LED_PIN_1, LOW);     // Turn OFF LED for plant 1

        // Keep pump active for the configured watering duration
        delay(DELAY_WATERING);

        // Update watering statistics for plant 2
        wateringCounter2++;
        lastWateringTime2 = millis();

        //after the watering absorption we check and set low the error flag  
        errorFlag2 = true;

        // Stop watering and reset indicators
        digitalWrite(RELAY_PIN_2, LOW);   // Turn OFF pump for plant 2
        digitalWrite(LED_PIN_2, LOW);     // Turn OFF LED for plant 2

        // Move to waiting state to allow water absorption
        state = WAITING_WATER_ABSORPTION;
    }
}
