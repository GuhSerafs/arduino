#include <Arduino.h>

#define BUZZ 3

char recv;

#define A4    440
#define B4    494
#define C5    523
#define D5b   554
#define D5    587
#define E5b   622
#define E5    659
#define F5    698
#define G5b   740
#define G5    784
#define A5b   831
#define A5    880
#define B5b   932
#define B5    988
#define C6    1046
#define D6b   1109
#define D6    1175
#define E6b   1245
#define E6    1319    
#define F6    1397
#define G6b   1480
#define G6    1568
#define A6b   1661
#define A6    1760
#define B6b   1865
#define B6    1976
#define C7    2093
#define D7b   2217
#define D7    2349


uint32_t t = 100;


void setup() {
  // put your setup code here, to run once:
  pinMode(BUZZ, OUTPUT);
  Serial.begin(9600);
}


void loop() {
  // put your main code here, to run repeatedly:   
  
  if(Serial.available()){
    recv = Serial.read();
    //Serial.println(recv);

    switch(recv){
      case '1': 
        tone(BUZZ, A4, t);
        delay(t);
        break;
      case 'Q': 
        tone(BUZZ, C5, t);
        delay(t);
        break;
      case '2':
        tone(BUZZ, D5b, t);
        delay(t);
        break;
      case 'W': 
        tone(BUZZ, D5, t);
        delay(t);
        break;
      case '3':
        tone(BUZZ, E5b, t);
        delay(t);
        break;
      case 'E': 
        tone(BUZZ, E5, t);
        delay(t);
        break;
      case 'R': 
        tone(BUZZ, F5, t);
        delay(t);
        break;
      case '5':
        tone(BUZZ, G5b, t);
        delay(t);
        break;
      case 'T': 
        tone(BUZZ, G5, t);
        delay(t);
        break;
      case '6': 
        tone(BUZZ, A5b, t);
        delay(t);
        break;
      case 'Y': 
        tone(BUZZ, A5, t); 
        delay(t);
        break;
      case '7':
        tone(BUZZ, B5b, t);
        delay(t);
        break;
      case 'U': 
        tone(BUZZ, B5, t);
        delay(t);
        break;
      case 'I': 
        tone(BUZZ, C6, t);
        delay(t);
        break;
      case '8': 
        tone(BUZZ, D6b, t);
        delay(t);
        break;
      case 'O': 
        tone(BUZZ, D6, t);
        delay(t);
        break;
      case '9': 
        tone(BUZZ, E6b, t);
        delay(t);
        break;
      case 'P': 
        tone(BUZZ, E6, t);
        delay(t);
        break;
      case '-': 
        tone(BUZZ, F6, t);
        delay(t);
        break;
      case 'Z': 
        tone(BUZZ, G6b, t);
        delay(t);
        break;
      case 'X':
        tone(BUZZ, G6, t);
        delay(t); 
        break;
      case 'C':
        tone(BUZZ, A6b, t);
        delay(t); 
        break;
      case 'V':
        tone(BUZZ, A6, t);
        delay(t); 
        break;
      case 'B':
        tone(BUZZ, B6b, t);
        delay(t); 
        break;
      case 'N':
        tone(BUZZ, B6, t);
        delay(t); 
        break;
      default: 
        break;
    }
    noTone(BUZZ);
  }
}