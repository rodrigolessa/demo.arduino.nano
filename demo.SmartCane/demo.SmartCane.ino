/////////////////////////////////////////////////
// Bibliotecas

#include <SoftwareSerial.h>
#include <Servo.h>
#include <Ultrasonic.h>

/////////////////////////////////////////////////
// Definindo pinos (digitais) para atuadores

#define ledWarning  7  // D7
#define pinoFT      9  // D9  ~ pwm
#define pinoLDR     10 // D10 ~ pwm
#define pinServo    11 // D11 ~ pwm
#define pinBuzzer   12 // D12
#define pinBtnPanic 4 // D4
#define pinBtn      8 // D8

// Definindo pinos para sensores

#define pinTRIGGER  2  // D2 - Arduino pin tied to trigger pin on the ultrasonic sensor.
#define pinECHO     5  // D5 ~ pwm - Arduino pin tied to echo pin on the ultrasonic sensor.

// Alguns pinos têm capacidades especiais. 
// Por exemplo, os pinos digitais 3, 5, 6, 9, 10 e 11, no Arduino Nano, 
// permitem gerar saída analógica usando PWM.

/////////////////////////////////////////////////
// Parâmetros de controle

#define minDistance 5
#define maxDistance 250
// Maximum distance we want to ping for (in centimeters).
// Maximum sensor distance is rated at 400-500cm. 

#define intFrequence 10
// Wait 50ms between pings (about 20 pings/sec). 
// 29ms should be the shortest delay between pings.

// Medidas em centimetros
float longe = 100;
float medio = 60;
float perto = 30;

// Servo position in degrees
int servoAngleMax = 90;
int servoAngleInit = 40;

// BUZZER
// * frequência: a frequência da nota em Hertz 
// * duração: a duração da nota em milissegundos (parâmetro opcional)
// Nota  =  Frequência (Hz)
// Dó  - C4 = 261.63
// Ré  - D4 = 293.66
// Mi  - E4 = 329.63
// Fá  - F4 = 349.23
// Sol - G4 = 392.00
// Lá  - A4 = 440.00
// Si  - B4 = 493.88
// Dó  - C5 = 523.25

//float buzzDuration = 2000;
float buzzDuration = 200;
//float buzzFrequence = 261.63;
float buzzFrequence = 261.63;

// Armazenar o valor lido do LDR
int lum = 0;

// Valor lido pelo fototransistor do Infrared
int infraval = 0;

bool standby = false;

/////////////////////////////////////////////////
// Instânciando sensores e atuadores

// Instância o sensor de distância com pinos PWN
Ultrasonic ultrasonic(pinTRIGGER, pinECHO);

// Instância servo motor
Servo servo;

/////////////////////////////////////////////////
// Esta função "setup" roda uma vez quando a placa e ligada ou resetada

void setup() {
  
  Serial.begin(9600);

  pinMode(pinoLDR, INPUT); // declarando pino de entrada
  pinMode(ledWarning, OUTPUT); // declarando pino de saída
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinoFT, INPUT); //Pino ligado ao coletor do fototransistor
  pinMode(pinBtnPanic, INPUT);
  pinMode(pinBtn, INPUT);

  // Iniciar motor servo e reposicionar
  servo.attach(pinServo);

  moveTo(servoAngleInit);
}

/////////////////////////////////////////////////
// Função que se repete infinitamente

void loop() {

  if (standby == false) {

    digitalWrite(ledWarning, HIGH); // ligar o LED

    for(int i = servoAngleInit; i <= servoAngleMax; i++) {
      servo.write(i);
      objectWarning(medianDistance());
    }

    infraRedDistance();
  }

  handleSerial();

  int btnPanic = digitalRead(pinBtnPanic);
  if (btnPanic == 1){
    Serial.print("Don't Panic!");
    Standby(true);
    tone(pinBuzzer, buzzFrequence, buzzDuration);
    delay(2);
    noTone(pinBuzzer);
    delay(2);
    tone(pinBuzzer, buzzFrequence, buzzDuration);
    delay(2);
    noTone(pinBuzzer);
  }

  if (standby == false) {
  
    // Repeats the previous lines from 120 to 20 degrees
    for(int i = servoAngleMax; i > servoAngleInit; i--) {
      servo.write(i);
      objectWarning(medianDistance());
      //delay(intFrequence);
    }

    infraRedDistance();
  }

  handleSerial();

  int btn = digitalRead(pinBtn);
  if (btn == 1){
    Serial.print("Second button pressed!");
    Standby(false);
  }

  if (standby) {

    // For debug
    float distance = medianDistance();
    delay(intFrequence * 5);
  }
}

/////////////////////////////////////////////////
// Mover o servo motor, suavemente, para a posição desejada

void moveTo(int servoAngle) {
  
  int currentPosition = servo.read();

  Serial.print("Posição: ");
  Serial.println(currentPosition);
  
  if (currentPosition > servoAngle) {
    for (int i = currentPosition; i > servoAngle; i--) {
      servo.write(i);
      delay(intFrequence / 2);
    }
  } else if (currentPosition < servoAngle) {
    for (int i = currentPosition; i <= servoAngle; i++){
      servo.write(i);
      delay(intFrequence / 2);
    }
  }
}

/////////////////////////////////////////////////
// Usar o sensor ultrassonico para calcular a distância de objetos. Usando média de três medidas

float medianDistance() {

  float d1 = ultrasonic.distanceRead();
  //Serial.println(d1);
  delay(5);

  float d2 = ultrasonic.distanceRead();
  //Serial.println(d2);
  delay(5);

  float d3 = ultrasonic.distanceRead();
  //Serial.println(d3);
  delay(5);

  float d4 = ultrasonic.distanceRead();
  //Serial.println(d3);
  delay(5);

  float total = d1 + d2 + d3 + d4;
  float distance = total / 3;

  //Serial.print("Total: ");
  //Serial.println(total);

  //Serial.print("Distancia em cm: ");
  //Serial.println(distance);

  return distance;
}

void objectWarning(float distance) {

  // Desligar o alerta
  noTone(pinBuzzer);

  // Se zero não houve retorno. Se maior que o distância máxima controlada não é necessário avisar
  if (distance > minDistance && distance < maxDistance) {
    
    if (distance < perto) {
      
      tone(pinBuzzer, buzzFrequence, buzzDuration);

      float distanceCheck = medianDistance();
    
      if (distance > minDistance && distance < maxDistance && distance < perto) {
        
        delay(intFrequence);

        distanceCheck = medianDistance();

        if (distance > minDistance && distance < maxDistance && distance < perto) {
          delay(intFrequence);
        }
      }
      
    } else if (distance < medio) {
      
      tone(pinBuzzer, buzzFrequence, buzzDuration);

      float distanceCheck = medianDistance();
    
      if (distance > minDistance && distance < maxDistance && distance < medio) {
        
        delay(intFrequence);
      }
      
    } else if (distance < longe) {
      tone(pinBuzzer, buzzFrequence, buzzDuration / 2);
    }
  }
}

/////////////////////////////////////////////////
// Controle de entrada e saída de comandos

void infraRedDistance() {

  lum = 0;
  lum = analogRead(pinoLDR); // lendo o sensor
      
  for (int i = 0; i < 3; i = i + 1){
    lum = lum + analogRead(pinoLDR); // lendo o sensor
    delay(5);
  }
    
  lum = lum / 3;
      
  Serial.print("Luminosidade: "); // exibindo no console a resistencia do sensor LDR
  Serial.println(lum); // exibindo no console a resistencia do sensor LDR
    
  //if (lum > 800){
  //digitalWrite(pinoLED, LOW); // muito claro, desligar o LED
  //} else if (lum < 400){
  //digitalWrite(pinoLED, HIGH); // ligar o LED
  //}

  infraval = analogRead(pinoFT);

  Serial.print("Infrared: ");
  Serial.println(infraval);
  
  if (infraval > 0)
  {  
    Serial.println("Objeto : Detectado");  
  }  
  else  
  {  
    Serial.println("Objeto : Ausente !");  
  }

  delay(500);
}

/////////////////////////////////////////////////
// Controle de entrada e saída de comandos

void handleSerial() {

  while (Serial.available() > 0) {
    
    char incomingChar = Serial.read();

    switch (incomingChar) {
      case '+':
        Standby(false);
        break;
      case '-':
        Standby(true);
        break;
    }
  }
}

void Standby(bool action) {
  if (action == true){
    Serial.println("Standby...");  // exbindo somente para testes
    standby = true;
    digitalWrite(ledWarning, LOW); // desligar o LED
    // Desligar o alerta
    noTone(pinBuzzer);
    moveTo(servoAngleMax);
  }
  else if (action == false){
    Serial.println("Restarting...");  // exbindo somente para testes
    standby = false;
  }
}
