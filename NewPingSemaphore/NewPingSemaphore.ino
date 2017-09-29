// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------

#include <NewPing.h>

#define verde  3
#define amarelo  7
#define vermelho  11

#define TRIGGER_PIN  2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     5  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(1152000); // Open serial monitor at 115200 baud to see ping results.
  pinMode(verde, OUTPUT);
  pinMode(amarelo, OUTPUT);
  pinMode(vermelho, OUTPUT);
}

void loop() {

  long duration, inches, cm;
  
  digitalWrite(verde, LOW);
  digitalWrite(amarelo, LOW);
  digitalWrite(vermelho, LOW);
  delay(1000);
  //delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  //Serial.print("Ping: ");
  //Serial.print(sonar.ping_cm());
  //Serial.println("cm");

  cm = sonar.ping_cm();

  if (cm < 35) {
    digitalWrite(verde, HIGH);
    delay(500);
  }

  if (cm < 20) {
    digitalWrite(amarelo, HIGH);
    delay(500);
  }

  if (cm < 10) {
    digitalWrite(vermelho, HIGH);
    delay(500);
  }
  
}
