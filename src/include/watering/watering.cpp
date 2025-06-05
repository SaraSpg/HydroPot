#include "watering/watering.h"
#include "globals.h"

void watering(int plantNumber) {
    if (plantNumber == 1) {
        digitalWrite(RELAY_PIN_1, HIGH);  // Accendi pompa 1
        digitalWrite(LED_PIN_1, HIGH);    // Accendi LED 1
        digitalWrite(RELAY_PIN_2, LOW);   // Spegni pompa 2
        digitalWrite(LED_PIN_2, LOW);     // Spegni LED 2
        delay(DELAY_WATERING);
        wateringCounter1++;
        lastWateringTime1 = millis();
        digitalWrite(RELAY_PIN_1, LOW);
        digitalWrite(LED_PIN_1, LOW);
        state = WAITING_WATER_ABSORPTION;
    } 
    else if (plantNumber == 2) {
        digitalWrite(RELAY_PIN_2, HIGH);  // Accendi pompa 2
        digitalWrite(LED_PIN_2, HIGH);    // Accendi LED 2
        digitalWrite(RELAY_PIN_1, LOW);   // Spegni pompa 1
        digitalWrite(LED_PIN_1, LOW);     // Spegni LED 1
        delay(DELAY_WATERING);
        wateringCounter2++;
        lastWateringTime2 = millis();
        digitalWrite(RELAY_PIN_2, LOW);
        digitalWrite(LED_PIN_2, LOW);
        state = WAITING_WATER_ABSORPTION;
    }
}
