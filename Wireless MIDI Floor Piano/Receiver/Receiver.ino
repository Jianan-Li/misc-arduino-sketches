#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX

byte channel = 0;
byte octave = 1; // from 0 to 5

byte noteValue[5][25]={
  {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60},
  {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72}, 
  {60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84}, 
  {72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96},
  {84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,107,108}};
  
void setup() {
  Serial.begin(31250);
  mySerial.begin(31250);
}

void loop() {
  while (mySerial.available()<=0) {}
  int value = mySerial.read();
  if (value<=25) {       //start note message: from 1 to 25
    MIDI(144+channel,noteValue[octave][value-1],127);
  }
  else if (value<=50) {  //end note message: from 26 to 50
    MIDI(128+channel,noteValue[octave][value-25-1],127);
  }
  else if (value<=52) {  //51 (octave down) to 52 (octave up)
    if (octave>0 && value == 51) {
      allNotesOff();
      octave-=1;
    }
    else if (octave<4 && value == 52) {
      allNotesOff();
      octave+=1;
    }
  }
  else if (value<80) {   // channel message from 60 (0) to 75 (15)
    allNotesOff(); 
    channel = value-60; 
  }
  else if (value==101) mySerial.write(value);
}

void allNotesOff() {
  for (int i=0;i<25;i++) {
    MIDI(128+channel,noteValue[octave][i],127);
  }
}

void MIDI(byte MESSAGE, byte PITCH, byte VELOCITY) {
  Serial.write(MESSAGE);
  Serial.write(PITCH);
  Serial.write(VELOCITY);
}
