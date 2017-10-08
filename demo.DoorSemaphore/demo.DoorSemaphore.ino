#include <NewPing.h>

// Definindo pinos para atuadores

#define ledLonge   3
#define ledMedio   7
#define ledPerto   11

#define pinTRIGGER  2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define pinECHO     5  // Arduino pin tied to echo pin on the ultrasonic sensor.

// Parâmetros de controle

#define intDistanciaMaxima  200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define intFrequencia       50 // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
#define intQtdTestes        6 // Quantidade de testes para fazer antes de obter uma média da distância

// Medidas em centimetros
long longe = 140;
long medio = 85;
long perto = 35;

// Instância o sensor de distância com pinos PWN

NewPing sonar(pinTRIGGER, pinECHO, intDistanciaMaxima);

void setup() {
  
  // Somente para testes
  //Serial.begin(9600); // Open serial monitor at 9600 baud to see ping results.

  pinMode(ledLonge, OUTPUT);
  pinMode(ledMedio, OUTPUT);
  pinMode(ledPerto, OUTPUT);
}

void loop() {

  long us = sonar.ping_median(intQtdTestes); // Medida em microsegundos
  long distancia = sonar.convert_cm(us); // Converte tempo para centimetros
  
  // Somente para testes
  //Serial.print("distancia: ");
  //Serial.print(distancia);
  //Serial.println("cm");

  if (distancia <= 0) {

    // Ignora, provavelmente um erro na leitura
    
  } else if (distancia <= perto) {

    // Avisa que vai abrir a porta
    
    digitalWrite(ledPerto, HIGH);
    digitalWrite(ledMedio, HIGH);
    digitalWrite(ledLonge, HIGH);

  } else if (distancia <= medio) {

    // Avisa que está bem próximo
    
    digitalWrite(ledPerto, LOW);
    digitalWrite(ledMedio, HIGH);
    digitalWrite(ledLonge, HIGH);
    
  } else if (distancia <= longe) {

    // Está se aproximando
    
    digitalWrite(ledPerto, LOW);
    digitalWrite(ledMedio, LOW);
    digitalWrite(ledLonge, HIGH);
    
  } else {

    digitalWrite(ledPerto, LOW);
    digitalWrite(ledMedio, LOW);
    digitalWrite(ledLonge, LOW);
    
  }

  // Aguarda até a próxima leitura
  
  delay(intFrequencia);
}
