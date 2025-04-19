#define RELAY_PIN 12 // Pin for the first relay (pump 1)
#define LED_PIN 4    // Pin for the first LED (plant 1)

void setup()
{
   pinMode(RELAY_PIN, OUTPUT); // Imposta il pin come uscita
   pinMode(LED_PIN, OUTPUT);   // Imposta il pin come uscita
   Serial.begin(9600);         // Inizializza la comunicazione seriale
}

void loop()
{
   digitalWrite(RELAY_PIN, HIGH);   // Accende il relè (attiva il motore)
   digitalWrite(LED_PIN, HIGH);     // Accende il LED (indicatore di funzionamento)
   Serial.println("Motore acceso"); // Stampa un messaggio sulla seriale
   delay(1000);                     // Aspetta 1 secondo

   digitalWrite(RELAY_PIN, LOW);    // Spegne il relè (ferma il motore)
   digitalWrite(LED_PIN, LOW);      // Spegne il LED (indicatore di funzionamento)
   Serial.println("Motore spento"); // Stampa un messaggio sulla seriale
   delay(2000);                     // Aspetta 2 secondi
}
