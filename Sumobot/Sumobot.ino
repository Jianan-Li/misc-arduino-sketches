#include <Servo.h>
Servo LFservo; 
Servo RFservo;
Servo LBservo;
Servo RBservo;
int tLED=2;

int incomingByte=0;
int rightHorValue=0;
int rightVerValue=0;
int LS=0;
int RS=0;

void setup() {
  pinMode(tLED,OUTPUT);
  LFservo.attach(6); 
  RFservo.attach(5);
  LBservo.attach(9);
  RBservo.attach(10);
  stopMoving();
  Serial.begin(19200);
  while (Serial.available()<=0) {}
  incomingByte = Serial.read();
  while ((incomingByte!=30) && (incomingByte!=31)) {
    incomingByte = Serial.read();
  }
}

void loop() {
  while (Serial.available()<=0) {}
  rightHorValue = Serial.read();
  while (Serial.available()<=0) {}
  rightVerValue = Serial.read();
  while (Serial.available()<=0) {}
  LS = Serial.read();
  while (Serial.available()<=0) {}
  RS = Serial.read();
  // uncomment to see all the data received
//  Serial.print(rightHorValue);
//  Serial.print("  ");
//  Serial.print(rightVerValue);
//  Serial.print("  ");
//  Serial.print(LS);
//  Serial.print("  ");
//  Serial.println(RS);
  
  if (rightVerValue>=6) {
    if (rightHorValue>=6) forwardLeft();
    else if (rightHorValue<=3) forwardRight();
    else moveForward();
    digitalWrite(tLED,HIGH);
  }
  else if (rightVerValue<=3) {
    if (rightHorValue>=6) backwardLeft();
    else if (rightHorValue<=3) backwardRight();
    else moveBackward();
    digitalWrite(tLED,HIGH);
  }
  else if (LS==21) { 
    turnLeft();
    digitalWrite(tLED,HIGH);
  }
  else if (RS==31) { 
    turnRight();
    digitalWrite(tLED,HIGH);
  }
  else {
    stopMoving();
    digitalWrite(tLED,LOW);
  }
}

void moveForward() {      // these values are specifically tuned to the four continuous servos installed on my sumobot
  LFservo.write(180);
  RFservo.write(78);
  LBservo.write(180);
  RBservo.write(78);
}

void forwardLeft() {
  LFservo.write(95);
  RFservo.write(0);
  LBservo.write(95);
  RBservo.write(0);
}

void forwardRight() {
  LFservo.write(180);
  RFservo.write(85);
  LBservo.write(180);
  RBservo.write(85);
}

void moveBackward() {
  LFservo.write(0);
  RFservo.write(101);
  LBservo.write(0);
  RBservo.write(101);
}

void backwardLeft() {
  LFservo.write(85);
  RFservo.write(180);
  LBservo.write(85);
  RBservo.write(180);
}

void backwardRight() {
  LFservo.write(0);
  RFservo.write(95);
  LBservo.write(0);
  RBservo.write(95);
}

void stopMoving() {
  LFservo.write(90);
  RFservo.write(90);
  LBservo.write(90);
  RBservo.write(90);
}

void turnLeft() {
  LFservo.write(85);
  RFservo.write(85);
  LBservo.write(85);
  RBservo.write(85);  
}

void turnRight() {
  LFservo.write(95);
  RFservo.write(95);  
  LBservo.write(95);
  RBservo.write(95); 
}
