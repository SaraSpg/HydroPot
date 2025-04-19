// Project Idea: Irrigation System with Arduino for two plants.
// Material: Arduino Uno, 2 pumps, 2 LEDs, 2 sensors (moisture or similar).

// Organization of the code: use of a FSM to manage the states of the system.
// IDLE:    no watering, both pumps off, both LEDs off, checking sensors.
// WATER1:  watering plant 1, pump 1 on, LED 1 on, pump 2 off, LED 2 off, pausing sensors.
// WATER2:  watering plant 2, pump 1 off, LED 1 off, pump 2 on, LED 2 on, pausing sensors.
// WAITING: waiting for a certain time before checking the sensors again.
// ERROR:   error state, both pumps off, both LEDs off, checking sensors.
enum state_type
{
  IDLE,
  WATER1,
  WATER2,
  WAITING_MEASURE,
  WAITING_WATER_ABSORPTION,
  ERROR
};
state_type state = IDLE;

//----------------------- PIN DEFINITIONS -----------------------//
#define SENSOR_PIN_1 A1 // Pin for the first sensor
#define SENSOR_PIN_2 A0 // Pin for the second sensor
#define RELAY_PIN_1 2   // Pin for the first relay (pump 1)
#define RELAY_PIN_2 13  // Pin for the second relay (pump 2)
#define LED_PIN_1 13    // Pin for the first LED (plant 1)
#define LED_PIN_2 12    // Pin for the second LED (plant 2)
#define LED_WAIT 8      // Pin for the waiting
//--------------------------------------------------------------//

//----------------------- CONSTANT DEFINITIONS -----------------//
#define HUMIDITY_THRESHOLD_1 370 // Threshold for the first sensor (moisture level)
#define HUMIDITY_THRESHOLD_2 370 // Threshold for the second sensor (moisture level)
// the two sensors may need a different calibration, so we can set different thresholds for each one. Above the threshold, the pump will be activated.
#define HUMIDITY_MARGIN_ERROR 50 //  If the sensor value continues to rise without watering, it is considered an error.
#define DELAY_ONEDAY 86400000    // 24 hours in milliseconds. If the sensor value does not drop after 24 hours from the irrigation, it is considered an error.
#define DELAY_WATERING 5000      // Duration of watering (in milliseconds). This is the time that the pump will be activated to water the plant.
#define DELAY_READING 3000       // Interval for reading sensors (in milliseconds)
//--------------------------------------------------------------//

//----------------------- GLOBAL VARIABLES ---------------------//
int wateringCounter1 = 0; // Counter for the number of waterings
int wateringCounter2 = 0; // Counter for the number of waterings
int errorFlag1 = 0;       // Error flag for the system
int errorFlag2 = 0;       // Error flag for the system
int valueSensor1 = 0;
int valueSensor2 = 0;
unsigned long lastWateringTime1 = 0;
unsigned long lastWateringTime2 = 0;
unsigned long time = 0; // Variable to store the time of the last watering
//--------------------------------------------------------------//

//----------------------- FUNCTION DEFINITIONS -----------------//
void checkSensors();
void watering();
void errorFound();
void printing();
void printingDate(unsigned long);
//--------------------------------------------------------------//

void setup()
{
  // Initialize the pins
  pinMode(SENSOR_PIN_1, INPUT);
  pinMode(SENSOR_PIN_2, INPUT);
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_WAIT, OUTPUT);
  // Set the initial state of the pumps and LEDs
  digitalWrite(LED_PIN_1, LOW);
  digitalWrite(LED_PIN_2, LOW);
  digitalWrite(LED_WAIT, LOW);
  digitalWrite(RELAY_PIN_1, LOW);
  digitalWrite(RELAY_PIN_2, LOW);

  Serial.begin(9600);
}

//------------------------------------------------------------------- LOOP ---------------------------------------------------------------------//

void loop()
{
  // Serial.print(analogRead(SENSOR_PIN_1));
  // delay (500);

  switch (state)
  {
  //-------------------------------- IDLE -----------------------------------//
  // IDLE: no watering, both pumps off, checking sensors.
  case IDLE:
    Serial.println("\nStato: IDLE");
    checkSensors(); // Check the sensors to see if watering is needed
    break;

    //-------------------------------- WATER1 -----------------------------------//
    // WATER1: watering plant 1, pump 1 on, pump 2 off, LED 2 off, pausing sensors.
  case WATER1:
    Serial.print("\nStato: WATER1");
    watering(); // Call the watering function to water plant 1
    Serial.println("\t\t ~°w°~ Grootino è stato annaffiato ~°w°~ ");
    break;

    //-------------------------------- WATER2 -----------------------------------//
    // WATER2: watering plant 2, pump 1 off, pump 2 on, LED 2 on, pausing sensors.
  case WATER2:
    Serial.println("\nStato: WATER2");
    watering(); // Call the watering function to water plant 2
    break;

    //-------------------------------- WAITING -----------------------------------//
    // WAITING_MEASURE: waiting for a certain time before checking the sensors again, both pumps off (the values vary slowly)
  case WAITING_MEASURE:
    Serial.println("\nStato: WAITING");
    delay(DELAY_READING); // Wait for a certain time before checking the sensors again
    state = IDLE;         // Change state to IDLE after waiting
    break;

    // WAITING_WATER_ABSORPTION: waiting for a certain time before checking the sensors again, both pumps off (the water needs to be absorbed by the soil).
  case WAITING_WATER_ABSORPTION:
    Serial.println("\nStato: WAITING WATERING ABSORPTION");
    digitalWrite(LED_WAIT, HIGH); // Turn on the waiting LED
    // DA CAMBIARE
    delay(10000);
    // delay(DELAY_ONEDAY); // Wait for a certain time before checking the sensors again
    digitalWrite(LED_WAIT, LOW); // Turn off the waiting LED
    state = ERROR;
    break;

    //-------------------------------- ERROR -----------------------------------//
    // ERROR: error state, both pumps off, both LEDs on, checking sensors.
  case ERROR:
    Serial.print("\nStato: ERROR? ");
    errorFound(); // Call the error function to handle the error state
    break;
  }
}

//------------------------------------------------------------------- FUNCTIONS ------------------------------------------------------------------------//

//-------------------------- CHECK SENSORS FUNCTION ------------------------------//
void checkSensors()
{
  valueSensor1 = analogRead(SENSOR_PIN_1);
  // valueSensor2 = analogRead(SENSOR_PIN_2);
  printing();

  if (errorFlag1 or errorFlag2)
  {
    state = ERROR;
  }
  else if (valueSensor1 > HUMIDITY_THRESHOLD_1)
  {
    digitalWrite(LED_PIN_1, HIGH); // Turn on LED 1 if the soil is dry
    if (valueSensor1 > HUMIDITY_THRESHOLD_1 + HUMIDITY_MARGIN_ERROR)
    {
      errorFlag1 = 1; // Set error flag if the sensor value continues to rise without watering
    }
    else if (errorFlag1 == 0)
    {
      state = WATER1; // Change state to WATER1 if the sensor value is above the threshold
    }
  }
  // The two sensors are checked with else if because the technology is power saving and can only activate one pump at a time.
  // else if (valueSensor2 > HUMIDITY_THRESHOLD_2)
  // {
  //   digitalWrite(LED_PIN_2, HIGH); // Turn on LED 2 if the soil is dry
  //   if (valueSensor2 > HUMIDITY_THRESHOLD_2 + HUMIDITY_MARGIN_ERROR)
  //   {
  //     errorFlag2 = 1; // Set error flag if the sensor value continues to rise without watering
  //   }
  //   else if (errorFlag2 == 0)
  //   {
  //     state = WATER2; // Change state to WATER2 if the sensor value is above the threshold
  //   }
  // }
  else
  {
    // If both sensors are below the threshold, turn off both LEDs and reset the error flag
    digitalWrite(LED_PIN_1, LOW);
    digitalWrite(LED_PIN_2, LOW);
    digitalWrite(RELAY_PIN_1, LOW); // Turn off pump 1
    // digitalWrite(RELAY_PIN_2, LOW); // Turn off pump 2
    errorFlag1 = 0;
    errorFlag2 = 0;
    state = WAITING_MEASURE; // Change state to WAITING_MEASURE if both sensors are below the threshold
  }
}

//-------------------------- WATERING FUNCTION ------------------------------//
void watering()
{
  if (state == WATER1)
  {
    digitalWrite(RELAY_PIN_1, HIGH);  // Turn on pump 1
    digitalWrite(LED_PIN_1, HIGH);    // Turn on LED 1
    delay(DELAY_WATERING);            // Watering duration (in milliseconds)
    digitalWrite(RELAY_PIN_1, LOW);   // Turn off pump 1
    digitalWrite(LED_PIN_1, LOW);     // Turn off LED 1
    wateringCounter1++;               // Increment watering counter
    lastWateringTime1 = millis();     // Record the time of watering
    state = WAITING_WATER_ABSORPTION; // Change state to WAITING after watering
  }
  else if (state == WATER2)
  {
    digitalWrite(RELAY_PIN_2, HIGH);  // Turn on pump 2
    digitalWrite(LED_PIN_2, HIGH);    // Turn on LED 2
    delay(DELAY_WATERING);            // Watering duration (in milliseconds)
    digitalWrite(RELAY_PIN_2, LOW);   // Turn off pump 2
    digitalWrite(LED_PIN_2, LOW);     // Turn off LED 2
    wateringCounter2++;               // Increment watering counter
    lastWateringTime2 = millis();     // Record the time of watering
    state = WAITING_WATER_ABSORPTION; // Change state to WAITING after watering
  }
  else
    state = IDLE;
}

//-------------------------- ERROR FUNCTION ------------------------------//
void errorFound()
{
  digitalWrite(LED_PIN_1, HIGH);  // Turn on LED 1 to indicate error
  digitalWrite(LED_PIN_2, HIGH);  // Turn on LED 2 to indicate error
  digitalWrite(RELAY_PIN_1, LOW); // Turn off pump 1
  // digitalWrite(RELAY_PIN_2, LOW; // Turn off pump 2
  if (analogRead(SENSOR_PIN_1) < HUMIDITY_THRESHOLD_1)
  {
    errorFlag1 = 0; // Reset error flag if the sensor value is below the threshold
    Serial.print(" NOPE.");
    Serial.println("\t Plant 1 has been watered correctly.");
    state = IDLE;
  }
  // else if (analogRead(SENSOR_PIN_2) < HUMIDITY_THRESHOLD_2)
  // {
  //   errorFlag2 = 0; // Reset error flag if the sensor value is below the threshold
  //   state = IDLE;
  // }
  else
  {
    Serial.println(" ERROR.");
  }
}

//-------------------------- PRINTING FUNCTIONS ------------------------------//
void printing()
{
  // Print the sensor values to the Serial Monitor
  Serial.print("Sensor 1: ");
  Serial.println(valueSensor1);
  // Serial.print(" Sensor 2: ");
  // Serial.println(valueSensor2);
  //  Print the last watering times to the Serial Monitor - sensor 1
  Serial.print(" Last watering time 1: ");
  if (lastWateringTime1 > millis())
    time = (lastWateringTime1 + millis()) / 1000; // Print the last watering time in seconds
  else
    time = (millis() - lastWateringTime1) / 1000; // Print the last watering time in seconds
  printingDate(time);
  // Print the last watering times to the Serial Monitor - sensor 2
  // Serial.print("Last watering time 2: ");
  // if (lastWateringTime2 > millis())
  //   time = (lastWateringTime2 + millis()) / 1000; // Print the last watering time in seconds
  // else
  //   time = (millis() - lastWateringTime2) / 1000; // Print the last watering time in seconds
  // printingDate(time);                             // Print the last watering time in seconds
}

void printingDate(unsigned long time)
{
  // Print the time in a readable format
  Serial.print(time / 43.200);
  Serial.print("d, ");
  Serial.print(time / 3600); // Print hours
  Serial.print("h, ");
  Serial.print(time / 60); // Print minutes
  Serial.print("m, ");
  Serial.print(time);  // Print seconds
  Serial.println("s"); // Print seconds
}
