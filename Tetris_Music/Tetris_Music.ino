#include "pitches.h"
#define piezo 2
#define pauseMusic 1
#define restartMusic 0

int melody1[] = {
  NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, 
  NOTE_A4, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_A4, NOTE_A4, NOTE_A3, NOTE_B3, NOTE_C4,
  NOTE_D4, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, 
  NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_A4, NOTE_A4,
};

int melody2[] = {
  NOTE_E4, NOTE_C4, NOTE_A3, NOTE_C4, NOTE_E4, NOTE_C4, NOTE_A3, NOTE_C4,
  NOTE_D4, NOTE_B3, NOTE_GS3,NOTE_B3, NOTE_D4, NOTE_B3, NOTE_GS3,NOTE_B3,
  NOTE_C4, NOTE_A3, NOTE_E3, NOTE_A3, NOTE_C4, NOTE_A3, NOTE_E3, NOTE_A3,
  NOTE_B3, NOTE_GS3,NOTE_E3, NOTE_GS3,NOTE_B3,
  NOTE_E4, NOTE_C4, NOTE_A3, NOTE_C4, NOTE_E4, NOTE_C4, NOTE_A3, NOTE_C4,
  NOTE_D4, NOTE_B3, NOTE_GS3,NOTE_B3, NOTE_D4, NOTE_B3, NOTE_GS3,NOTE_B3,
  NOTE_C4, NOTE_A3, NOTE_C4, NOTE_E4, NOTE_A4, NOTE_A4, NOTE_GS4
};

byte noteDurations1[] = {
  4,2,2,2,1,1,2,2,
  4,2,2,4,2,2,2,2,2,2,4,4,4,4,2,2,2,2,
  2,4,2,2,1,1,2,2,6,2,2,1,1,2,2,
  4,2,2,4,4,4,4,8,
};

byte noteDurations2[] = {
  2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,
  2,2,2,2,8,
  2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,
  2,2,2,2,4,4,16
};

// calculates the number of elements in the melody array.
byte musicLength1=sizeof(melody1)/sizeof('NOTE_F5');
byte musicLength2=sizeof(melody2)/sizeof('NOTE_F5');

boolean isPlaying=false;

void setup() {  
  pinMode(piezo,OUTPUT);
  pinMode(restartMusic,INPUT);
  pinMode(pauseMusic,INPUT);
  delay(300);
}

void loop() {
  while (digitalRead(restartMusic)==HIGH) {
  }
  int duration;
  int pause;
  for (int n=0;n<2;n++) {
    for (int i=0; i< musicLength1; i++) {
      while (digitalRead(pauseMusic)==HIGH) {}
      if (digitalRead(restartMusic)==HIGH) {
        return;
      }
      duration = noteDurations1[i]*80;
      tone(piezo, melody1[i],duration);
      pause=noteDurations1[i]*112;
      delay(pause);
    }
  }
  for (int i=0; i< musicLength2; i++) {
    while (digitalRead(pauseMusic)==HIGH) {}
    if (digitalRead(restartMusic)==HIGH) {
        return;
     }
    duration = noteDurations2[i]*80;
    tone(piezo, melody2[i],duration);
    pause=noteDurations2[i]*112;
    delay(pause);
  }
}


