#include <Arduino.h>

#define IN1 6
#define IN2 5

uint8_t velocidade_motor;

// Protótipos das funções
void girar_horario();
void girar_antihorario();
void freiar();
void atualizar_velocidade();

// Código de Inicialização
void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  Serial.begin(9600);
  Serial.println("Arduino conectado");
}

// Superloop
void loop() {
  static uint32_t timer = 0;
  static uint8_t ultima_velocidade_reportada;

  // A cada segundo, reporta a velocidade do motor, caso ela tenha mudado.
  if (millis() - timer >= 1000){
    timer = millis();

    if(ultima_velocidade_reportada != velocidade_motor){
      Serial.println(velocidade_motor);
      ultima_velocidade_reportada = velocidade_motor;
    }
  }

  if (Serial.available()){
    char comando = Serial.read();
    Serial.println(comando);

    switch(comando){
      case 'W': 
        girar_horario();
        break;
      case 'S': 
        girar_antihorario();
        break;
      case 'P':
        freiar();
        break;
      case 'Y':
        if(velocidade_motor < 255){
          velocidade_motor++;
          atualizar_velocidade();
        }
        break;
      case 'H':
        if(velocidade_motor > 0){
          velocidade_motor--;
          atualizar_velocidade();
        }
        break;
      default: 
        break;
    }
  }
}

void girar_horario(){
  analogWrite(IN1, velocidade_motor);
  digitalWrite(IN2, LOW);
  return;
}

void girar_antihorario(){
  digitalWrite(IN1, LOW);
  analogWrite(IN2, velocidade_motor);
  return;
}

void freiar(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  return;
}

void atualizar_velocidade(){
  if (digitalRead(IN1) == LOW){
    analogWrite(IN2, velocidade_motor);
    return;
  }else if(digitalRead(IN2) == LOW){
    analogWrite(IN1, velocidade_motor);
    return;
  }

  return;
}