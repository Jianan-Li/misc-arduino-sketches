// Written by Jianan Li

// Anglog In
int const analogReadPin1=A0;  // FS,GS,AS
int const analogReadPin2=A1;  // F,G,A,B
int const analogReadPin3=A2;  // CS,DS
int const analogReadPin4=A3;  // C,D,E
int const detectModePin=A4;   // whether piezo or midi
int const potPin=A5;          // change octave

// other pins
int const piezoPin=10;
int const octave1=11;  //these three pins are for LEDs
int const octave2=12;
int const octave3=13;
int const OutPin=8;    //5V
int const piezoModePin=2;  // switching between these two changes the mode
int const midiModePin=3;

// frequencies for piezo
float c[]={261.6,523.3,1047,2093};
float cs[]={277.2,554.4,1109,2217};
float d[]={293.7,587.3,1175,2349};
float ds[]={311.1,622.3,1245,2489};
float e[]={329.6,659.3,1319,2637};
float f[]={349.2,698.5,1397,2794};
float fs[]={370.0,740.0,1480,2960};
float g[]={392.0,784.0,1568,3136};
float gs[]={415.3,830.6,1661,3322};
float a[]={440.0,880.0,1760,3520};
float as[]={466.2,932.3,1865,3729};
float b[]={493.9,987.8,1976,3951};

// note value for midi
int mC[]={60,72,84,96};
int mCS[]={61,73,85,97};
int mD[]={62,74,86,98};
int mDS[]={63,75,87,99};
int mE[]={64,76,88,100};
int mF[]={65,77,89,101};
int mFS[]={66,78,90,102};
int mG[]={67,79,91,103};
int mGS[]={68,80,92,104};
int mA[]={69,81,93,105};
int mAS[]={70,82,94,106};
int mB[]={71,83,95,107};

// for comparison
float previousMidiNotesList[12]={};

void setup() {
  pinMode(piezoModePin,OUTPUT);
  digitalWrite(piezoModePin,HIGH);
  pinMode(midiModePin,OUTPUT);
  digitalWrite(midiModePin,LOW);
  pinMode(detectModePin,INPUT);
  pinMode(octave1,OUTPUT);
  pinMode(octave2,OUTPUT);
  pinMode(octave3,OUTPUT);
  pinMode(OutPin,OUTPUT);
  digitalWrite(OutPin,HIGH);
  pinMode(piezoPin,OUTPUT);
  Serial.begin(57600);
}

void loop() {
  int potReading=analogRead(potPin);
  int i=0;
  if (potReading>750) {
    digitalWrite(octave1,HIGH);
    digitalWrite(octave2,LOW);
    digitalWrite(octave3,LOW);
    i=0;
  }
  else if (potReading<750 && potReading>500) {
    digitalWrite(octave1,LOW);
    digitalWrite(octave2,HIGH);
    digitalWrite(octave3,LOW);
    i=1;
  }
  else if (potReading<500 && potReading>250) {
    digitalWrite(octave1,LOW);
    digitalWrite(octave2,LOW);
    digitalWrite(octave3,HIGH);
    i=2;
  }
  else if (potReading<250) {
    digitalWrite(octave1,LOW);
    digitalWrite(octave2,LOW);
    digitalWrite(octave3,LOW);
    i=3;
  }
  
  float notes[] = {c[i],cs[i],d[i],ds[i],e[i],f[i],fs[i],g[i],gs[i],a[i],as[i],b[i]};
  float midiNotes[] = {mC[i],mCS[i],mD[i],mDS[i],mE[i],mF[i],mFS[i],mG[i],mGS[i],mA[i],mAS[i],mB[i]};
  int noteIndex=12;
  // MIDI mode supports multiple simultaneous keypresses
  float midiNotesList[12]={};
  int j=0;
  
  int analogPin1Reading=analogRead(analogReadPin1);
  int analogPin2Reading=analogRead(analogReadPin2);
  int analogPin3Reading=analogRead(analogReadPin3);
  int analogPin4Reading=analogRead(analogReadPin4);
  
  //Serial.println(analogPin4Reading);
  
  if (analogPin4Reading>10) {
    if (analogPin4Reading<=530) {
      if (analogPin4Reading<=400) {
        noteIndex=0;
      }
      else if (analogPin4Reading>400 && analogPin4Reading<=465) {
        noteIndex=2;   
      }
      else if (analogPin4Reading>465 && analogPin4Reading<=530) {
        noteIndex=4;
      }
      midiNotesList[j]=midiNotes[noteIndex];
      j = j + 1;
    }
    else {
      if (analogPin4Reading<=575) {
        midiNotesList[j]=midiNotes[0]; j = j + 1;
        midiNotesList[j]=midiNotes[2]; j = j + 1;
      }
      else if (analogPin4Reading>575 && analogPin4Reading<=615) {
        midiNotesList[j]=midiNotes[0]; j = j + 1;
        midiNotesList[j]=midiNotes[4]; j = j + 1;
      }
      else if (analogPin4Reading>615 && analogPin4Reading<=680) {
        midiNotesList[j]=midiNotes[2]; j = j + 1;
        midiNotesList[j]=midiNotes[4]; j = j + 1;
      }
      else if (analogPin4Reading>680) {
        midiNotesList[j]=midiNotes[0]; j = j + 1;
        midiNotesList[j]=midiNotes[2]; j = j + 1;
        midiNotesList[j]=midiNotes[4]; j = j + 1;
      }
    }
  }
  if (analogPin3Reading>10) {
    if (analogPin3Reading<=480) {
      if (analogPin3Reading<=370) {
        noteIndex=1;        
      }
      else if (analogPin3Reading>370) {
        noteIndex=3;        
      }
      midiNotesList[j]=midiNotes[noteIndex];
      j = j + 1;
    }
    else {
      midiNotesList[j]=midiNotes[1]; j = j + 1;
      midiNotesList[j]=midiNotes[3]; j = j + 1;
    }
  }
  if (analogPin2Reading>10) {
    if (analogPin2Reading<=300 || (analogPin2Reading>=470 && analogPin2Reading<=500)) {
      if (analogPin2Reading<=200) {
        noteIndex=5;        
      }
      else if (analogPin2Reading>200 && analogPin2Reading<=240) {
        noteIndex=7;        
      }
      else if (analogPin2Reading>240 && analogPin2Reading<=350) {
        noteIndex=9;        
      }
      else if (analogPin2Reading>350) {
        noteIndex=11;        
      }
      midiNotesList[j]=midiNotes[noteIndex];
      j = j + 1;
    }
    else {
      if (analogPin2Reading<=350) {
        midiNotesList[j]=midiNotes[5]; j = j + 1;
        midiNotesList[j]=midiNotes[7]; j = j + 1;
      }
      else if (analogPin2Reading>350 && analogPin2Reading<=382) {
        midiNotesList[j]=midiNotes[5]; j = j + 1;
        midiNotesList[j]=midiNotes[9]; j = j + 1;
      }
      else if (analogPin2Reading>382 && analogPin2Reading<=420) {
        midiNotesList[j]=midiNotes[7]; j = j + 1;
        midiNotesList[j]=midiNotes[9]; j = j + 1;
      }
      else if (analogPin2Reading>420 && analogPin2Reading<=490) {
        midiNotesList[j]=midiNotes[5]; j = j + 1;
        midiNotesList[j]=midiNotes[7]; j = j + 1;
        midiNotesList[j]=midiNotes[9]; j = j + 1;
      }
      else if (analogPin2Reading>490 && analogPin2Reading<=533) {
        midiNotesList[j]=midiNotes[5]; j = j + 1;
        midiNotesList[j]=midiNotes[11]; j = j + 1;
      }
      else if (analogPin2Reading>533 && analogPin2Reading<=549) {
        midiNotesList[j]=midiNotes[7]; j = j + 1;
        midiNotesList[j]=midiNotes[11]; j = j + 1;
      }
      else if (analogPin2Reading>549 && analogPin2Reading<=566) {
        midiNotesList[j]=midiNotes[9]; j = j + 1;
        midiNotesList[j]=midiNotes[11]; j = j + 1;
      }
      else if (analogPin2Reading>566 && analogPin2Reading<=582) {
        midiNotesList[j]=midiNotes[5]; j = j + 1;
        midiNotesList[j]=midiNotes[7]; j = j + 1;
        midiNotesList[j]=midiNotes[11]; j = j + 1;
      }
      else if (analogPin2Reading>582 && analogPin2Reading<=594) {
        midiNotesList[j]=midiNotes[5]; j = j + 1;
        midiNotesList[j]=midiNotes[9]; j = j + 1;
        midiNotesList[j]=midiNotes[11]; j = j + 1;
      }
      else if (analogPin2Reading>594 && analogPin2Reading<=610) {
        midiNotesList[j]=midiNotes[7]; j = j + 1;
        midiNotesList[j]=midiNotes[9]; j = j + 1;
        midiNotesList[j]=midiNotes[11]; j = j + 1;
      }
      else if (analogPin2Reading>610) {
        midiNotesList[j]=midiNotes[5]; j = j + 1;
        midiNotesList[j]=midiNotes[7]; j = j + 1;
        midiNotesList[j]=midiNotes[9]; j = j + 1;
        midiNotesList[j]=midiNotes[11]; j = j + 1;
      }
    }
  }
  if (analogPin1Reading>10) {
    if (analogPin1Reading<=350) {
      if (analogPin1Reading<=240) {
        noteIndex=6;        
      }
      else if (analogPin1Reading>240 && analogPin1Reading<=280) {
        noteIndex=8;        
      }
      else if (analogPin1Reading>280) {
        noteIndex=10;        
      }
      midiNotesList[j]=midiNotes[noteIndex];
      j = j + 1;
    }
    else {
      if (analogPin1Reading<=410) {
        midiNotesList[j]=midiNotes[6]; j = j + 1;
        midiNotesList[j]=midiNotes[8]; j = j + 1;
      }
      else if (analogPin1Reading>410 && analogPin1Reading<=437) {
        midiNotesList[j]=midiNotes[6]; j = j + 1;
        midiNotesList[j]=midiNotes[10]; j = j + 1;
      }
      else if (analogPin1Reading>437 && analogPin1Reading<=480) {
        midiNotesList[j]=midiNotes[8]; j = j + 1;
        midiNotesList[j]=midiNotes[10]; j = j + 1;
      }
      else if (analogPin1Reading>480) {
        midiNotesList[j]=midiNotes[6]; j = j + 1;
        midiNotesList[j]=midiNotes[8]; j = j + 1;
        midiNotesList[j]=midiNotes[10]; j = j + 1;
      }
    }
  }
  
  // used below to check if the current noteslist has note from the previous noteslist. if so, stop them from being played.
  int n=0;
  //if there is keypress
  if (noteIndex!=12 || midiNotesList[0]!=0) {
    // decide whether to use the piezo or midi software
    int pinModeReading=digitalRead(detectModePin);
    if (pinModeReading==HIGH) {
      tone(piezoPin,notes[noteIndex],30);
    }
    else if (pinModeReading==LOW) {
      for (int k = 0; k < 12; k++){
        if (midiNotesList[k]!=0) {
          //check if the current note is in the previous noteslist (note from the previous chord). if so, stop them from being played.
          // find the number of notes in the previous noteslist
          int notesInPreviousList=0;
          for (int a = 0; a < 12; a++) {
            if (previousMidiNotesList[a]!=0) {
              notesInPreviousList = notesInPreviousList+1;
            }
          }
          int notesInCurrentList=0;
          for (int a = 0; a < 12; a++) {
            if (midiNotesList[a]!=0) {
              notesInCurrentList = notesInCurrentList+1;
            }
          }
          if (notesInPreviousList>notesInCurrentList) {
            for (int l = 0; l < 12; l++) {
              if (midiNotesList[k]!=previousMidiNotesList[l]) {
                n = n+1;
              }
            }
            if (n==12) {
              MIDI(144,midiNotesList[k],127); 
            }
            //reset n
            n=0;
          }
          else {
            MIDI(144,midiNotesList[k],127);
          }
        }
      }
      
      // compare analogreadings to end the current note and start a new note
      int oldReading1= analogRead(analogReadPin1);
      int oldReading2= analogRead(analogReadPin2);
      int oldReading3= analogRead(analogReadPin3);
      int oldReading4= analogRead(analogReadPin4);
      int newAnalogReading1= analogRead(analogReadPin1);
      int newAnalogReading2= analogRead(analogReadPin2);
      int newAnalogReading3= analogRead(analogReadPin3);
      int newAnalogReading4= analogRead(analogReadPin4);
      //compare potentiometer readings
      int newPotReading=analogRead(potPin);
      int newI=i;
      while ((newAnalogReading1>=10 || newAnalogReading2>=10 || newAnalogReading3>=10 || newAnalogReading4>=10) && (((newAnalogReading1-oldReading1)<=10 && (newAnalogReading1-oldReading1)>=-10) && ((newAnalogReading2-oldReading2)<=10 && (newAnalogReading2-oldReading2)>=-10) && ((newAnalogReading3-oldReading3)<=10 && (newAnalogReading3-oldReading3)>=-10) && ((newAnalogReading4-oldReading4)<=10 && (newAnalogReading4-oldReading4)>=-10)) && (newI==i)) {
        oldReading1=newAnalogReading1;
        oldReading2=newAnalogReading2;
        oldReading3=newAnalogReading3;
        oldReading4=newAnalogReading4;
        delay(1);
        newAnalogReading1=analogRead(analogReadPin1);
        newAnalogReading2=analogRead(analogReadPin2);
        newAnalogReading3=analogRead(analogReadPin3);
        newAnalogReading4=analogRead(analogReadPin4);
        delay(1);
        newPotReading = analogRead(potPin);
        if (newPotReading>750) {newI=0;}
        else if (newPotReading<750 && newPotReading>500) {newI=1;}
        else if (newPotReading<500 && newPotReading>250) {newI=2;}
        else if (newPotReading<250) {newI=3;}
      }
      //reset n to 0
      n=0;
      for (int k = 0; k < 12; k++){
        if (midiNotesList[k]!=0) {
          //check if the current note is in the previous noteslist (note from the previous chord). if so, stop them from being played.
          // find the number of notes in the previous noteslist
          int notesInPreviousList=0;
          for (int a = 0; a < 12; a++) {
            if (previousMidiNotesList[a]!=0) {
              notesInPreviousList = notesInPreviousList+1;
            }
          }
          int notesInCurrentList=0;
          for (int a = 0; a < 12; a++) {
            if (midiNotesList[a]!=0) {
              notesInCurrentList = notesInCurrentList+1;
            }
          }
          if (notesInPreviousList>notesInCurrentList) {
            for (int l = 0; l < 12; l++) {
              if (midiNotesList[k]!=previousMidiNotesList[l]) {
                n = n+1;
              }
            }
            if (n==12) {
              MIDI(128,midiNotesList[k],127); 
            }
            //reset n
            n=0;
          }
          else {
            MIDI(128,midiNotesList[k],127);
          }
        }
      }
    }
  }
  // remember the previous MidiNotesList
  for (int k = 0; k < 12; k++){
    previousMidiNotesList[k]=midiNotesList[k];
  }
  delay(2);
}


void MIDI(unsigned char MESSAGE, unsigned char PITCH, unsigned char VELOCITY) 
{
  Serial.write(MESSAGE);
  Serial.write(PITCH);
  Serial.write(VELOCITY);
}
