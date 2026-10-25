#include <Arduino.h>
#include "../globals/globals.h"
#include "setupSystem.h"

void setupSystem() {
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  digitalWrite(RELAY_PIN_1, LOW);
  digitalWrite(RELAY_PIN_2, LOW);

  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_WAIT, OUTPUT);
  digitalWrite(LED_PIN_1, LOW);
  digitalWrite(LED_PIN_2, LOW);
  digitalWrite(LED_WAIT, LOW);

  Serial.begin(115200);  
  Serial.println("Sistema di irrigazione (ESP32) avviato.");

  state = IDLE;
}
