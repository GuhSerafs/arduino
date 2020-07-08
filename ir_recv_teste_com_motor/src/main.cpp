#include <Arduino.h>
#include <IRremote.h>

// Definições dos pinos utilizados
#define RECV_PIN 11 // Pino do receptor IR (ligado no S no módulo)
#define MOT_IN1 6   // Pino IN1 do motor (ligado no IN1 na ponte H)
#define MOT_IN2 5   // Pino IN2 do motor (ligado no IN2 na ponte H)

// Estrutura (variável) para lidar com o processamento dos dados da IR
decode_results resposta; 

// Protótipo das funções de controle do motor
void girarMotorHorario();
void girarMotorAntihorario();
void freiarMotor();

typedef enum{
   FR, HR, AHR
}m_status;

m_status motor = FR;

// Objeto para lidar com o processamento dos dados da IR
IRrecv irrecv(RECV_PIN);

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
  irrecv.enableIRIn(); // Inicializa o receptor infravermelho
  irrecv.blink13(true); // Habilita o pica no led da placa quando receber algo no IR
}

void loop() {
  
  // Aguarda até haver uma resposta no receptor infravermelho... 

  if (irrecv.decode(&resposta)) {
    
    // Se houver resposta, atende o comando recebido

    if(resposta.value == 0xFF6897 || resposta.value == 0xF20A6897){ // Btn para girar sentido horário
      girarMotorHorario();
      motor = HR;
    }else if(resposta.value == 0xFFB04F || resposta.value == 0xF20AA857){ // Btn para girar sentido antihorário
      girarMotorAntihorario();
      motor = AHR;
  }else if(resposta.value == 0xFF38C7 || resposta.value == 0x4151EDD4){ // Btn para freiar o motor
      freiarMotor();
      motor = FR;
    }
    Serial.println(resposta.value, HEX);
    irrecv.resume(); // Habilita próxima recepção
  }
}

void girarMotorHorario(){
  //Serial.println("Girando Motor Horario");

  //Freia o motor
  digitalWrite(MOT_IN1, LOW);
  digitalWrite(MOT_IN2, LOW);

  // Acelera no sentido horário
  for(int i=0; i<255; i++){
    delay(2);
    analogWrite(MOT_IN1, i);
  }
  digitalWrite(MOT_IN1, HIGH);
  digitalWrite(MOT_IN2, LOW);
}

void girarMotorAntihorario(){
  //Serial.println("Girando Motor Antihorario");

  // Freia o motor
  digitalWrite(MOT_IN1, LOW);
  digitalWrite(MOT_IN2, LOW);

  // Acelera no sentido horário 
  for(int i=0; i<255; i++){
    delay(2);
    analogWrite(MOT_IN2, i);
  }
  digitalWrite(MOT_IN1, LOW);
  digitalWrite(MOT_IN2, HIGH);
}

void freiarMotor(){
  //Serial.println("Freiando motor");

  // Freia o motor
  digitalWrite(MOT_IN1, HIGH);
  digitalWrite(MOT_IN2, HIGH);
}