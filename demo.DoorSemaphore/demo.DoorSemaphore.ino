#include <Ultrasonic.h>

// Definindo pinos para atuadores

#define ledLonge   3
#define ledMedio   7
#define ledPerto   11

#define pinTRIGGER  2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define pinECHO     5  // Arduino pin tied to echo pin on the ultrasonic sensor.

// Parâmetros de controle

#define intDistanciaMaxima  200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define intFrequencia       700 // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.

int estaLonge = 0;
int estaMedio = 0;
int estaPerto = 0;

// Medidas em centimetros
long distancia = intDistanciaMaxima;
long longe = 140;
long medio = 70;
long perto = 25;

// Instância o sensor de distância com pinos PWN

Ultrasonic ultrasonic(pinTRIGGER, pinECHO);

void setup() {
  
  Serial.begin(9600);

  pinMode(ledLonge, OUTPUT);
  pinMode(ledMedio, OUTPUT);
  pinMode(ledPerto, OUTPUT);
}

void loop() {

  distancia = ultrasonic.distanceRead();
  
  Serial.print("distancia: ");
  Serial.print(distancia);
  Serial.println("cm");

  // Avisa sobre proximidade
  // Está se aproximando

  if (distancia <= longe && estaLonge == 0) {
    digitalWrite(ledLonge, HIGH);
    estaLonge ++;
  } else if (distancia > longe && estaLonge == 1) {
    estaLonge ++;
  } else if (distancia > (longe + 2) && estaLonge > 0) {
    // Apaga o led
    digitalWrite(ledLonge, LOW);
    estaLonge = 0;
  }

  // Avisa que está bem próximo

  if (distancia <= medio && estaMedio == 0) {
    digitalWrite(ledMedio, HIGH);
    estaMedio ++;
  } else if (distancia > medio && estaMedio == 1) {
    estaMedio ++;
  } else if (distancia > (medio + 2) && estaMedio > 0) {
    // Apaga o led
    digitalWrite(ledMedio, LOW);
    estaMedio = 0;
  }

  // Avisa que vai abrir a porta

  if (distancia <= perto && estaPerto == 0) {
    digitalWrite(ledPerto, HIGH);
    estaPerto ++;
  } else if (distancia > perto && estaPerto == 1) {
    estaPerto ++;
  } else if (distancia > (perto + 2) && estaPerto > 0) {
    // Apaga o led
    digitalWrite(ledPerto, LOW);
    estaPerto = 0;
  }

  // Aguarda até a próxima leitura
  
  delay(intFrequencia);
}
