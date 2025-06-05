#include <Arduino.h>
#include "globals.h"
#include "setup.h"

void setupSystem() {
  pinMode(SENSOR_PIN_1, INPUT);
  pinMode(SENSOR_PIN_2, INPUT);

  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  digitalWrite(RELAY_PIN_1, LOW);
  digitalWrite(RELAY_PIN_2, LOW);

  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  digitalWrite(LED_PIN_1, LOW);
  digitalWrite(LED_PIN_2, LOW);

  pinMode(LED_WAIT, OUTPUT);
  digitalWrite(LED_WAIT, LOW);

  Serial.begin(9600);
  Serial.println("Sistema di irrigazione avviato.");

  state = IDLE;
}
