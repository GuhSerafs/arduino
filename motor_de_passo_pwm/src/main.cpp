#include <Arduino.h>

#define PWM_OUT 6

String buffer;
void passo_posicao(int i);

void setup() {
  // put your setup code here, to run once:
  pinMode(PWM_OUT, OUTPUT);
  Serial.begin(9600);
  Serial.println("Arduino Conectado"); 
}

void loop() {
  // put your main code here, to run repeatedly:
  int16_t pos = 0;
  if(Serial.available()){
    buffer = Serial.readString();
    Serial.print("Recebido: ");
    Serial.println(buffer);
    pos = buffer.toInt();

    passo_posicao(pos);
  }
}

void passo_posicao(int i){
  static uint32_t timer = 0;

  timer = millis(); 
  digitalWrite(6, HIGH);
  delayMicroseconds(1500 + 100*i/9);
  digitalWrite(6, LOW);
  
  //while(millis() - timer < 20);
}