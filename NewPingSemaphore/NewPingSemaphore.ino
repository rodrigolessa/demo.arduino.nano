// ------------------------------------------------------------------------------------
// Exemplo para controle um sensor ultrasonico e demonstrar a distância usando 3 LEDs
// ------------------------------------------------------------------------------------

#include <NewPing.h>

#define verde         3
#define amarelo       7
#define vermelho      11

#define TRIGGER_PIN   2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN      5  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE  200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define intFrequencia 150 // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.

bool boolVerde = false;
bool boolAmarelo = false;
bool boolVermelho = false;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  
  pinMode(verde, OUTPUT);
  pinMode(amarelo, OUTPUT);
  pinMode(vermelho, OUTPUT);
}

void loop() {

  long cm;
  
  delay(intFrequencia);
  
  Serial.print("Ping: ");
  cm = sonar.ping_cm();
  Serial.print(cm);
  //Serial.print(sonar.ping_cm());
  Serial.println("cm");

  if (cm < 30 && !boolVerde) {
    digitalWrite(verde, HIGH);
    boolVerde = true;
  } else if (cm > 35 && boolVerde) {
    digitalWrite(verde, LOW);
    boolVerde = false;
  }

  if (cm < 20 && !boolAmarelo) {
    digitalWrite(amarelo, HIGH);
    boolAmarelo = true;
  } else if (cm > 25 && boolAmarelo) {
    digitalWrite(amarelo, LOW);
    boolAmarelo = false;
  }

  if (cm < 10 && !boolVermelho) {
    digitalWrite(vermelho, HIGH);
    boolVermelho = true;
  } else if (cm > 15 and boolVermelho) {
    digitalWrite(vermelho, LOW);
    boolVermelho = false;
  }

  if (cm >= MAX_DISTANCE) {
    digitalWrite(verde, LOW);
    boolVerde = false;
    digitalWrite(amarelo, LOW);
    boolAmarelo = false;
    digitalWrite(vermelho, LOW);
    boolVermelho = false;
  }
  
}
