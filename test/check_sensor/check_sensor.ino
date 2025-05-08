void setup() {
   Serial.begin(9600); // Inizializza la comunicazione seriale
   pinMode(A0, INPUT);
 }
 
 void loop() {
   int valore = analogRead(A0); // Legge il valore analogico dal pin A1
   Serial.println(valore);      // Stampa il valore sulla seriale
   delay(500);                  // Aspetta 500 millisecondi
 }
 