#include "Arduino.h"

const int RELAY_PIN_1 = 25;
const int RELAY_PIN_2 = 26;
const int LED_PIN_1   = 17;
const int LED_PIN_2   = 2;

// Active duration for each pump during testing (in milliseconds, e.g., 3 seconds)
const unsigned long TEST_INTERVAL = 3000; 

// Safety delay between shutting off one pump and turning on the next
const unsigned long PAUSE_BETWEEN_TESTS = 5000;

void setup() {
    // Initialize serial communication for debugging output
    Serial.begin(115200);
    while (!Serial); // Wait for serial port initialization (useful for native USB boards)
    
    // Configure relay and status LED pins as OUTPUTs
    pinMode(RELAY_PIN_1, OUTPUT);
    pinMode(LED_PIN_1, OUTPUT);
    pinMode(RELAY_PIN_2, OUTPUT);
    pinMode(LED_PIN_2, OUTPUT);
    
    // Ensure all pumps and LEDs are turned OFF by default at start
    digitalWrite(RELAY_PIN_1, LOW);
    digitalWrite(LED_PIN_1, LOW);
    digitalWrite(RELAY_PIN_2, LOW);
    digitalWrite(LED_PIN_2, LOW);
    
    Serial.println("=== Starting Motor and LED Test ===");
    delay(1000); // Wait 1 sec

}

void loop() {
    // --- PLANT 1 TEST ---
    Serial.println("Testing Plant 1: Turning ON Pump 1 and LED 1...");
    digitalWrite(RELAY_PIN_1, HIGH);
    digitalWrite(LED_PIN_1, HIGH);
    
    delay(TEST_INTERVAL); // Keep Motor 1 active
    
    Serial.println("Plant 1 Test complete: Turning OFF.");
    digitalWrite(RELAY_PIN_1, LOW);
    digitalWrite(LED_PIN_1, LOW);
    
    // Inter-test pause
    delay(PAUSE_BETWEEN_TESTS);

    // --- PLANT 2 TEST ---
    Serial.println("Testing Plant 2: Turning ON Pump 2 and LED 2...");
    digitalWrite(RELAY_PIN_2, HIGH);
    digitalWrite(LED_PIN_2, HIGH);
    
    delay(TEST_INTERVAL); // Keep Motor 2 active
    
    Serial.println("Plant 2 Test complete: Turning OFF.");
    digitalWrite(RELAY_PIN_2, LOW);
    digitalWrite(LED_PIN_2, LOW);
    
    // Pause before restarting the test loop
    Serial.println("Test sequence finished. Waiting 5 seconds before repeating...\n");
    delay(5000);
}


