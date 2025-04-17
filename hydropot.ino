// Project Idea: Irrigation System with Arduino for two plants.
// Material: Arduino Uno, 2 pumps, 2 LEDs, 2 sensors (moisture or similar).

// Organization of the code: use of a FSM to manage the states of the system.
// IDLE:    no watering, both pumps off, both LEDs off, checking sensors.
// WATER1:  watering plant 1, pump 1 on, LED 1 on, pump 2 off, LED 2 off, pausing sensors.
// WATER2:  watering plant 2, pump 1 off, LED 1 off, pump 2 on, LED 2 on, pausing sensors.
// WAITING: waiting for a certain time before checking the sensors again, both pumps off, both LEDs off.
// ERROR:   error state, both pumps off, both LEDs off, checking sensors.
enum state_type
{
  IDLE,
  WATER1,
  WATER2,
  WAITING,
  ERROR
};
state_type state = IDLE;

//----------------------- PIN DEFINITIONS -----------------------//
#define SENSOR_PIN_1 A1 // Pin for the first sensor
#define SENSOR_PIN_2 A2 // Pin for the second sensor
#define RELALY_PIN_1 13 // Pin for the first relay (pump 1)
#define RELALY_PIN_2 12 // Pin for the second relay (pump 2)
#define LED_PIN_1 11    // Pin for the first LED (plant 1)
#define LED_PIN_2 10    // Pin for the second LED (plant 2)
//--------------------------------------------------------------//

//----------------------- CONSTANT DEFINITIONS -----------------//
#define SENSOR_READ_INTERVAL 500 // Interval for reading sensors (in milliseconds)
#define HUMIDITY_THRESHOLD_1 370 // Threshold for the first sensor (moisture level)
#define HUMIDITY_THRESHOLD_2 370 // Threshold for the second sensor (moisture level)
// the two sensors may need a different calibration, so we can set different thresholds for each one. Above the threshold, the pump will be activated.
#define HUMIDITY_MARGIN_ERROR 50 //  If the sensor value continues to rise without watering, it is considered an error.
#define ONE_DAY_MILLIS 86400000  // 24 hours in milliseconds. If the sensor value does not drop after 24 hours from the irrigation, it is considered an error.
//--------------------------------------------------------------//

//----------------------- GLOBAL VARIABLES ---------------------//
int wateringCounter = 0; // Counter for the number of waterings
int errorFlag1 = 0;      // Error flag for the system
int errorFlag2 = 0;      // Error flag for the system
//--------------------------------------------------------------//

//----------------------- FUNCTION DEFINITIONS -----------------//

//--------------------------------------------------------------//

void setup()
{
  // Initialize the pins
  pinMode(SENSOR_PIN_1, INPUT);
  pinMode(SENSOR_PIN_2, INPUT);
  pinMode(RELALY_PIN_1, OUTPUT);
  pinMode(RELALY_PIN_2, OUTPUT);
  digitalWrite(RELALY_PIN_1, LOW);
  digitalWrite(RELALY_PIN_2, LOW);

  Serial.begin(9600);
}

void loop()
{
  switch (state)
  {
  //-------------------------------- IDLE -----------------------------------//
  // IDLE: no watering, both pumps off, checking sensors.
  case IDLE:
    Serial.println("Stato: IDLE");
    if (errorFlag)
    {
      state = ERROR;
    }
    else if (analogRead(SENSOR_PIN_1) > HUMIDITY_THRESHOLD_1)
    {
      digitalWrite(LED_PIN_1, HIGH); // Turn on LED 1 if the soil is dry
      if (analogRead(SENSOR_PIN_1) > HUMIDITY_THRESHOLD_1 + HUMIDITY_MARGIN_ERROR)
      {
        errorFlag1 = 1; // Set error flag if the sensor value continues to rise without watering
      }
      else if (errorFlag1 == 0)
      {
        state = WATER1; // Change state to WATER1 if the sensor value is above the threshold
      }
    }
    else if (analogRead(SENSOR_PIN_2) > HUMIDITY_THRESHOLD_2)
    {
      digitalWrite(LED_PIN_2, HIGH); // Turn on LED 2 if the soil is dry
      if (analogRead(SENSOR_PIN_2) > HUMIDITY_THRESHOLD_2 + HUMIDITY_MARGIN_ERROR)
      {
        errorFlag2 = 1; // Set error flag if the sensor value continues to rise without watering
      }
      else if (errorFlag2 == 0)
      {
        state = WATER2; // Change state to WATER2 if the sensor value is above the threshold
      }
    }
    else
    {
      // If both sensors are below the threshold, turn off both LEDs and reset the error flag
      digitalWrite(LED_PIN_1, LOW);
      digitalWrite(LED_PIN_2, LOW);
      errorFlag = 0;
      state = WAITING;
    }

    break;

    //-------------------------------- WATER1 -----------------------------------//
    // WATER1: watering plant 1, pump 1 on, pump 2 off, LED 2 off, pausing sensors.
  case WATER1:
    Serial.println("Stato: WATER1");

    break;

    //-------------------------------- WATER2 -----------------------------------//
    // WATER2: watering plant 2, pump 1 off, pump 2 on, LED 2 on, pausing sensors.
  case WATER2:
    Serial.println("Stato: WATER2");

    break;

    //-------------------------------- WAITING -----------------------------------//
    // WAITING: waiting for a certain time before checking the sensors again, both pumps off
  case WAITING:
    Serial.println("Stato: WAITING");
    break;

    //-------------------------------- ERROR -----------------------------------//
    // ERROR: error state, both pumps off, both LEDs on, checking sensors.
  case ERROR:
    Serial.println("Stato: ERROR");
    digitalWrite(LED_PIN_1, HIGH);   // Turn on LED 1 to indicate error
    digitalWrite(LED_PIN_2, HIGH);   // Turn on LED 2 to indicate error
    digitalWrite(RELALY_PIN_1, LOW); // Turn off pump 1
    digitalWrite(RELALY_PIN_2, LOW); // Turn off pump 2
    if (analogRead(SENSOR_PIN_1) < HUMIDITY_THRESHOLD_1)
    {
      errorFlag1 = 0; // Reset error flag if the sensor value is below the threshold
      state = IDLE;
    }
    else if (analogRead(SENSOR_PIN_2) < HUMIDITY_THRESHOLD_2)
    {
      errorFlag2 = 0; // Reset error flag if the sensor value is below the threshold
      state = IDLE;
    }
    break;
  }
}
