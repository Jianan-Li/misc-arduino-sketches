#include <digitalWriteFast.h>

unsigned long previousKeyState;
unsigned long keyState;

byte previousLightSwitchState;
byte lightSwitchState;

byte tetris[36]={16,11,12,14,12,11,9,12,16,14,12,11,12,14,16,12,9,9,14,14,17,21,19,17,16,12,16,14,12,11,12,14,16,12,9,9};

void setup() {
  Serial.begin(31250);
  for (int i=2;i<27;i++) {
    digitalWriteFast(i,HIGH);
  }
  for (int i=27;i<52;i++) {
    pinMode(i,OUTPUT);
    digitalWriteFast(i,LOW);
  } 
  digitalWriteFast(52,HIGH);
  pinMode(53,INPUT);
}

void loop() { 
  keyState = readKeyStateStable();
  if (keyState!=previousKeyState) sendMessage();
  previousKeyState=keyState;
  if (Serial.available()>0) {
    if (Serial.read()==101) startTutorial();
  }
  lightSwitchState=digitalReadFast(53);
  if (!previousLightSwitchState && lightSwitchState) {
    delay(5);
    lightSwitchState=digitalReadFast(53);
    if (!previousLightSwitchState && lightSwitchState) {
      allLightOn();
    }
  }
  else if (previousLightSwitchState && !lightSwitchState) {
    delay(5);
    lightSwitchState=digitalReadFast(53);
    if (previousLightSwitchState && !lightSwitchState) {
      allLightOff();
    }
  }
  previousLightSwitchState=lightSwitchState;
}

void allLightOn() {
  for (int i = 0; i < 25; i++) {
    digitalWrite(i+27,HIGH);  // keys with index 0 through 24
  }
}

void allLightOff() {
  for (int i = 0; i < 25; i++) {
    digitalWrite(i+27,LOW);  // keys with index 0 through 24
  }
}

void startTutorial() {
  digitalWrite(tetris[0]+27,HIGH);
  for (int i=0;i<sizeof(tetris);i++) {
    previousKeyState=keyState;
    keyState = readKeyStateStable();
    while (!isCorrectNote(tetris[i])) {
      previousKeyState=keyState;
      keyState = readKeyStateStable();
    }
    Serial.write(tetris[i]+1);
    digitalWrite(tetris[i]+27,LOW);
    if (i<sizeof(tetris)-1) digitalWrite(tetris[i+1]+27,HIGH);
    while(!digitalRead(tetris[i]+2)) {}
    Serial.write(tetris[i]+1+25);
  }
}

boolean isCorrectNote(int index) {
  for (int i = 0; i < 25; i++) {
    if (!((keyState >> (24-i)) & 1) && ((previousKeyState >> (24-i) & 1))) {
      if (i!=index) return false;
      else return true;
    }
  }
  return false;
}

void sendMessage() {
  for (int i = 0; i < 25; i++) {
    if (!((keyState >> (24-i)) & 1) && ((previousKeyState >> (24-i) & 1))) {
      Serial.write(i+1);
      digitalWrite(i+27,HIGH);  // keys with index 0 through 24
    }
    else if (((keyState >> (24-i)) & 1) && !((previousKeyState >> (24-i) & 1))) {  
      Serial.write((i+1)+25);
      digitalWrite(i+27,LOW);
    }
  }
}

unsigned long readKeyStateStable() {
  unsigned long read1= readKeyState();delay(4);
  unsigned long read2= readKeyState();delay(4);
  unsigned long read3= readKeyState();delay(4);
  unsigned long read4= readKeyState();delay(4);
  unsigned long read5= readKeyState();
  while (!(read1==read2 && read2==read3 && read3==read4 && read4==read5)) {
    delay(4);
    read1 = read2; read2 = read3; read3 = read4; read4 = read5;
    read5 = readKeyState();
  }
  return read5;
}

unsigned long readKeyState() {
  unsigned long bitValue;
  unsigned long bytesVal=0;
  for (int i=0;i<25;i++) {
    bitValue = digitalReadFast(i+2);
    if (i==19) bitValue = digitalReadFast(52);
    bytesVal |= (bitValue << (24-i));
  }
  return bytesVal;
}
