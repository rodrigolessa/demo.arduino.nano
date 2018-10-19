#include <Ultrasonic.h>

// Definindo pinos (digitais) para atuadores

#define ledLonge   3  // D3
#define ledMedio   7  // D7
#define ledPerto   11 // D11
#define pinBuzzer  12 // D12

// Definindo pinos para sensores

#define pinTRIGGER  2  // D2 - Arduino pin tied to trigger pin on the ultrasonic sensor.
#define pinECHO     5  // D5 ~ PWM - Arduino pin tied to echo pin on the ultrasonic sensor.

// Alguns pinos têm capacidades especiais. 
// Por exemplo, os pinos digitais 3, 5, 6, 9, 10 e 11, no Arduino Nano, 
// permitem gerar saída analógica usando PWM.

// Parâmetros de controle

#define intDistanciaMaxima 200  // Maximum distance we want to ping for (in centimeters). 
                                // Maximum sensor distance is rated at 400-500cm.
#define intFrequencia      1000 // Wait 50ms between pings (about 20 pings/sec). 
                                // 29ms should be the shortest delay between pings.

bool estaLonge = false;
bool estaMedio = false;
bool estaPerto = false;

// Medidas em centimetros
long distancia = intDistanciaMaxima;
long longe = 30;
long medio = 20;
long perto = 10;

// Instância o sensor de distância com pinos PWN

Ultrasonic ultrasonic(pinTRIGGER, pinECHO);

void setup() {
  
  Serial.begin(9600);

  pinMode(ledLonge, OUTPUT);
  pinMode(ledMedio, OUTPUT);
  pinMode(ledPerto, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
}

void loop() {

  distancia = ultrasonic.distanceRead();
  
  Serial.print("Distancia em cm: ");
  Serial.println(distancia);

  if (distancia <= perto) {

    // Avisa que vai abrir a porta
    
    digitalWrite(ledLonge, HIGH);
    digitalWrite(ledMedio, HIGH);
    digitalWrite(ledPerto, HIGH);

    estaPerto = true;
    estaMedio = false;
    estaLonge = false;

    tone(pinBuzzer, 1500);
    delay(250);
    noTone(pinBuzzer);
    delay(250);
    tone(pinBuzzer, 1500);
    delay(250);
    
  //} else if (distancia <= medio && !estaMedio) {
  } else if (distancia <= medio) {

     // Avisa que está bem próximo
     
    digitalWrite(ledLonge, HIGH);
    digitalWrite(ledMedio, HIGH);
    digitalWrite(ledPerto, LOW);
    
    estaPerto = false;
    estaMedio = true;
    estaLonge = false;

    tone(pinBuzzer, 1200);
    delay(intFrequencia);

  //} else if (distancia <= longe && !estaLonge) {
  } else if (distancia <= longe) {

    // Está se aproximando
      
    digitalWrite(ledLonge, HIGH);
    digitalWrite(ledMedio, LOW);
    digitalWrite(ledPerto, LOW);

    estaPerto = false;
    estaMedio = false;
    estaLonge = true;

    noTone(pinBuzzer);
    delay(intFrequencia);

  //} else if (distancia >= intDistanciaMaxima) {
  } else {

    // Nenhum objeto encontrado

    digitalWrite(ledLonge, LOW);
    digitalWrite(ledMedio, LOW);
    digitalWrite(ledPerto, LOW);
    
    estaPerto = false;
    estaMedio = false;
    estaLonge = false;

    noTone(pinBuzzer);
    delay(intFrequencia);
  }
  
}
