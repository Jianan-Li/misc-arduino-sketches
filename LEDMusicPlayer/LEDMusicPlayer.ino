//Use pushbotton as a toggle switch.

//When the pushbotton is pressed, the pushbotton pin reads a
//high voltage and then a low voltage, and then ledOn is set
//to the opposite of its current value (either false or true).

//Based on Starter Kit Project 02.
#include "pitches.h"

const int piezoPin = 12; //piezo
const int rPin = 5;  //red LED
const int gPin = 4;  //green LED
const int bPin = 3;  //blue LED
const int pPin = 2;  //pushbutton

int ledState = 0;
int ledOn = false;

// notes
int melody[] = {
  NOTE_F5,NOTE_D5,NOTE_AS4,NOTE_D5,NOTE_F5,NOTE_AS5,NOTE_D6,NOTE_C6,NOTE_AS5,NOTE_D5,NOTE_E5,NOTE_F5,
  NOTE_F5,NOTE_F5,NOTE_D6,NOTE_C6,NOTE_AS5,NOTE_A5,NOTE_G5,NOTE_A5,NOTE_AS5,NOTE_AS5,NOTE_F5,NOTE_D5,NOTE_AS4,
  NOTE_D6,NOTE_D6,NOTE_D6,NOTE_DS6,NOTE_F6,NOTE_F6,NOTE_DS6,NOTE_D6,NOTE_C6,NOTE_D6,NOTE_DS6,NOTE_DS6,
  0,NOTE_DS6,NOTE_D6,NOTE_C6,NOTE_AS5,NOTE_A5,NOTE_G5,NOTE_A5,NOTE_AS5,NOTE_D5,NOTE_E5,NOTE_F5,
  NOTE_F5,NOTE_AS5,NOTE_AS5,NOTE_AS5,NOTE_A5,NOTE_G5,NOTE_G5,NOTE_G5,NOTE_C6,NOTE_DS6,NOTE_D6,NOTE_C6,NOTE_AS5,NOTE_AS5,NOTE_A5,
  NOTE_F5,NOTE_F5,NOTE_AS5,NOTE_C6,NOTE_D6,NOTE_DS6,NOTE_F6,NOTE_AS5,NOTE_C6,NOTE_D6,NOTE_DS6,NOTE_C6,NOTE_AS5
};

// durations: 2 = half note, and 8/3,4,6,8,12. It appears that 8/2.9 is more accurate than 8/3.
float noteDurations[] = {
  6,12,4,4,4,2,6,12,4,4,4,2,
  8,8,8/2.9,8,4,2,8,8,4,4,4,4,4,
  6,12,4,4,4,2,8,8,4,4,4,2,
  8,8,8/2.9,8,4,2,8,8,4,4,4,2,
  4,4,4,8,8,4,4,4,4,8,8,8,8,4,4,
  8,8,8/2.9,8,8,8,2,8,8,4,4,4,2
};

// calculates the number of elements in the melody array.
int musicLength=sizeof(melody)/sizeof('NOTE_F5');

void setup() {  
  pinMode(pPin, INPUT);
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);
}

void loop() {
  int pPinState=digitalRead(pPin);
  if(pPinState==HIGH) {
    ledState = 1;
  }
  if (pPinState==LOW and ledState ==1) {
    ledState = 2;
    ledOn = not ledOn;
  }
  if (ledOn && pPinState!=HIGH) {
    for (int thisNote = 0; thisNote < musicLength; thisNote++) {
      // blink the three LEDs in sequence
      if (thisNote%3==0){    
        digitalWrite(rPin, HIGH);
        digitalWrite(gPin, LOW);
        digitalWrite(bPin, LOW);
      }
      else if (thisNote%3==1){    
        digitalWrite(rPin, LOW);
        digitalWrite(gPin, HIGH);
        digitalWrite(bPin, LOW);
      }
      else if (thisNote%3==2){    
        digitalWrite(rPin, LOW);
        digitalWrite(gPin, LOW);
        digitalWrite(bPin, HIGH);
      }

      // calculate the note duration. change tempo by changing 2000 to other values
      int noteDuration = 2000/noteDurations[thisNote];
      tone(piezoPin, melody[thisNote],noteDuration);
      
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well
      float pauseBetweenNotes = noteDuration * 1.30;
      
      //split the delay into two parts and check to see
      //whether the pushbutton is pressed to turn off
      //the sound and light
      delay(pauseBetweenNotes/2);
      if(digitalRead(pPin)==HIGH) {
        break;      
      }
      delay(pauseBetweenNotes/2);
      if(digitalRead(pPin)==HIGH) {
        break;      
      }
    }
  }
  else if (not ledOn) {
    digitalWrite(rPin, LOW);
    digitalWrite(gPin, LOW);
    digitalWrite(bPin, LOW);
  }
}
