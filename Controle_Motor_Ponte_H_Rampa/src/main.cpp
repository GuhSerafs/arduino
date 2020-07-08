#include <Arduino.h>

#define IN1 6
#define IN2 5
#define PASSOS_ACC 100 // Menor que 100 pelo menos
#define TEMPO_ACC 1000
#define DURACAO_PASSO TEMPO_ACC/PASSOS_ACC

/*
Obs: passos_acc <= tempo_acc
*/

uint8_t velocidade_motor;
float velocidade_abs_motor;

typedef enum{HR, AHR, PRD} stt_motor;

/*
HR -> Horário
AHR -> Anti Horário
PRD -> Parado
*/

typedef enum{INI, TS, FNLZ} stt_modo;

/*
INI -> Inicializando o modo
TS -> Transição do modo
FNLZ -> Finalização do modo 
*/

stt_motor StatusMotor = PRD;
stt_modo StatusHorario = INI;
stt_modo StatusAntihorario = INI;
stt_modo StatusParado = INI;

// Protótipos das funções
void freiar();
void atualizar_velocidade();

// Código de Inicialização
void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  Serial.begin(9600);
  Serial.println("Arduino conectado");

  freiar();
}

// Superloop
void loop() {
  static uint32_t timer = 0;
  static uint32_t instante_transicao = 0;
  static int16_t ultima_velocidade_reportada;
  static int16_t velocidade_na_transicao = 0;
  static int16_t passo_velocidade = 0;
  static uint8_t contador_passos = 0;

  // A cada segundo, reporta a velocidade do motor, caso ela tenha mudado.
  if (millis() - timer >= 1000){
    timer = millis();

    if(ultima_velocidade_reportada != velocidade_abs_motor){
      Serial.println(velocidade_abs_motor);
      ultima_velocidade_reportada = velocidade_abs_motor;
    }
  }

  if (Serial.available()){
    char comando = Serial.read();
    Serial.println(comando);

    switch(comando){
      case 'W': 
        if(StatusMotor != HR){
          StatusMotor = HR;
          StatusHorario = INI;
        };
        break;
      case 'S': 
        if(StatusMotor != AHR){
          StatusMotor = AHR;
          StatusAntihorario = INI;
        };
        break;
      case 'X':
        if(StatusMotor != PRD){
          StatusMotor = PRD;
          StatusParado = INI;
        }
        break;
      case 'P':
        freiar();
        velocidade_abs_motor = 0;
        atualizar_velocidade();
        break;
      case 'Y':
        if(velocidade_abs_motor < 255){
          velocidade_abs_motor++;
          atualizar_velocidade();
        }
        break;
      case 'H':
        if(velocidade_abs_motor > -255){
          velocidade_abs_motor--;
          atualizar_velocidade();
        }
        break;
      default: 
        break;
    }
  }
  // Máquina de Estados do Motor
  switch(StatusMotor){
    case HR: // Controle da máquina de estados do horário
      switch(StatusHorario){
        case INI: 
          velocidade_na_transicao = velocidade_abs_motor;
          passo_velocidade = -(2*velocidade_na_transicao);
          instante_transicao = millis();
          contador_passos = 0;
          StatusHorario = TS;
        break;
        case TS: 
          if(millis() - instante_transicao >= DURACAO_PASSO){
            instante_transicao = millis();
            velocidade_abs_motor = (PASSOS_ACC*velocidade_abs_motor + passo_velocidade);
            velocidade_abs_motor /= PASSOS_ACC;
            //if(velocidade_abs_motor == 0) velocidade_abs_motor = 2; // Se a velocidade for 0, subtrai 1
            atualizar_velocidade();
            contador_passos++;

            if(contador_passos == PASSOS_ACC) {
              if (velocidade_abs_motor != -velocidade_na_transicao){
              velocidade_abs_motor = -velocidade_na_transicao;
              atualizar_velocidade();
              } 
              StatusHorario = FNLZ;
            }
          }
        break;
        case FNLZ:
          // Nada, por enquanto
        break;
      }
    break;

    case AHR: // Controle da máquina de estados do anti horário
      switch(StatusAntihorario){
        case INI: 
          velocidade_na_transicao = velocidade_abs_motor;
          passo_velocidade = -(2*velocidade_na_transicao);
          instante_transicao = millis();
          contador_passos = 0;
          StatusAntihorario = TS; 
        break;

        case TS: 
          if(millis() - instante_transicao >= DURACAO_PASSO){
            instante_transicao = millis();
            velocidade_abs_motor = (PASSOS_ACC*velocidade_abs_motor + passo_velocidade);
            velocidade_abs_motor /= PASSOS_ACC;
            //if(velocidade_abs_motor == 0) velocidade_abs_motor = -2; // Se a velocidade for 0, subtrai 1
            atualizar_velocidade();
            contador_passos++;

            if(contador_passos == PASSOS_ACC) {
              if (velocidade_abs_motor != -velocidade_na_transicao){
              velocidade_abs_motor = -velocidade_na_transicao;
              atualizar_velocidade();
              } 
              StatusAntihorario = FNLZ;
            }
          }
        break;

        case FNLZ:
          // Nada por enquanto.
        break;
      }
    break;

    case PRD: // Controle da máquina de estados do parado horário
      switch(StatusParado){
        case INI: 
          velocidade_na_transicao = velocidade_abs_motor;
          passo_velocidade = -velocidade_na_transicao;
          instante_transicao = millis();
          contador_passos = 0;
          StatusParado = TS; 
        break;

        case TS: 
          if(millis() - instante_transicao >= DURACAO_PASSO){
            instante_transicao = millis();
            velocidade_abs_motor = (PASSOS_ACC*velocidade_abs_motor + passo_velocidade);
            velocidade_abs_motor /= PASSOS_ACC;
            atualizar_velocidade();
            contador_passos++;

            if(contador_passos == PASSOS_ACC) {
              if (velocidade_abs_motor != 0){
              velocidade_abs_motor = 0;
              atualizar_velocidade();
              } 
              StatusParado = FNLZ;
            }
          }
        break;

        case FNLZ:
          // Nada por enquanto...
        break;
      }
    break;
  }
}

void freiar(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);

  StatusMotor = PRD;
  StatusParado = FNLZ;
  return;
}

void atualizar_velocidade(){
  Serial.println(velocidade_abs_motor);
  static uint8_t verif_transicao;

  velocidade_motor = abs(velocidade_abs_motor);
  verif_transicao = (StatusHorario == TS | StatusAntihorario == TS | StatusParado == TS);

  if (velocidade_abs_motor > 0 && StatusMotor != HR && !verif_transicao){
    StatusMotor = HR;
    StatusHorario = FNLZ;
  }else if(velocidade_abs_motor == 0 && StatusMotor != PRD && !verif_transicao){
    StatusMotor = PRD;
    StatusParado = FNLZ;
  }else if(velocidade_abs_motor < 0 && StatusMotor != AHR && !verif_transicao){
    StatusMotor = AHR;
    StatusAntihorario = FNLZ;
  }

  if (velocidade_abs_motor >= 0){
    analogWrite(IN1, velocidade_motor);
    digitalWrite(IN2, LOW);
  }else{
    analogWrite(IN2, velocidade_motor);
    digitalWrite(IN1, LOW);
  }
  return;
}