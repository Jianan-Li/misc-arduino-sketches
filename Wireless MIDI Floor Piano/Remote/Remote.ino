#include <SoftwareSerial.h>
SoftwareSerial mySerial(1, 0); // RX, TX

#define tutorialMode 1
#define octaveDown 3
#define octaveUp 4

byte previousChannel;
byte currentChannel;

byte previousOctaveDownState;
byte previousOcaveUpState;
byte previoustutorialModeState;

void setup() {
  mySerial.begin(31250);
  pinMode(octaveDown,INPUT);
  pinMode(octaveUp,INPUT);
  pinMode(tutorialMode,INPUT);
  digitalWrite(octaveDown,HIGH);
  digitalWrite(octaveUp,HIGH);
  digitalWrite(tutorialMode,HIGH);
}

void loop() {
  int potReading=analogRead(A1);
  currentChannel=potReading/64;
  if (currentChannel!=previousChannel) {
    mySerial.write(60+currentChannel);
  }
  if (previousOctaveDownState && !digitalRead(octaveDown)) {
    delay(4);
    if (!digitalRead(octaveDown)) {
      mySerial.write(51);
    }
  }
  if (previousOcaveUpState && !digitalRead(octaveUp)) {
    delay(4);
    if (!digitalRead(octaveUp)) {
      mySerial.write(52);
    }
  }
  if (previoustutorialModeState && !digitalRead(tutorialMode)) {
    delay(4);
    if (!digitalRead(tutorialMode)) {
      mySerial.write(101);
    }
  }
  previousChannel=currentChannel;
  previousOctaveDownState = digitalRead(octaveDown);
  previousOcaveUpState = digitalRead(octaveUp);
  previoustutorialModeState = digitalRead(tutorialMode);
}
