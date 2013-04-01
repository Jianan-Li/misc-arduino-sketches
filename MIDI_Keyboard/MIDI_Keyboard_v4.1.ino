// Written by Jianan Li
// Project Website: jiananli.wordpress.com
// Email: ljn07050@gmail.com

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

// dynamic octave determined by potentiometer reading in the function determineOctave()
int octavei=0;
float notes[12]={};
float midiNotes[12]={};

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
  determineOctave(potReading);
  
  int noteIndex=12;
  // MIDI mode supports multiple simultaneous keypresses
  float midiNotesList[12]={};
  int j=0;
  
  int analogPin1Reading=analogRead(analogReadPin1);
  int analogPin2Reading=analogRead(analogReadPin2);
  int analogPin3Reading=analogRead(analogReadPin3);
  int analogPin4Reading=analogRead(analogReadPin4);
  
  //determine which key(s) are being pressed
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
    if (analogPin2Reading<=335 || (analogPin2Reading>=470 && analogPin2Reading<=508)) {
      if (analogPin2Reading<=200) {
        noteIndex=5;        
      }
      else if (analogPin2Reading>200 && analogPin2Reading<=250) {
        noteIndex=7;        
      }
      else if (analogPin2Reading>250 && analogPin2Reading<=330) {
        noteIndex=9;        
      }
      else if (analogPin2Reading>330) {
        noteIndex=11;        
      }
      midiNotesList[j]=midiNotes[noteIndex];
      j = j + 1;
    }
    else {
      if (analogPin2Reading<=370) {
        midiNotesList[j]=midiNotes[5]; j = j + 1;
        midiNotesList[j]=midiNotes[7]; j = j + 1;
      }
      else if (analogPin2Reading>370 && analogPin2Reading<=425) {
        midiNotesList[j]=midiNotes[5]; j = j + 1;
        midiNotesList[j]=midiNotes[9]; j = j + 1;
      }
      else if (analogPin2Reading>425 && analogPin2Reading<=455) {
        midiNotesList[j]=midiNotes[7]; j = j + 1;
        midiNotesList[j]=midiNotes[9]; j = j + 1;
      }
      else if (analogPin2Reading>455 && analogPin2Reading<=520) {
        midiNotesList[j]=midiNotes[5]; j = j + 1;
        midiNotesList[j]=midiNotes[7]; j = j + 1;
        midiNotesList[j]=midiNotes[9]; j = j + 1;
      }
      else if (analogPin2Reading>520 && analogPin2Reading<=565) {
        midiNotesList[j]=midiNotes[5]; j = j + 1;
        midiNotesList[j]=midiNotes[11]; j = j + 1;
      }
      else if (analogPin2Reading>565 && analogPin2Reading<=585) {
        midiNotesList[j]=midiNotes[7]; j = j + 1;
        midiNotesList[j]=midiNotes[11]; j = j + 1;
      }
      else if (analogPin2Reading>585 && analogPin2Reading<=610) {
        midiNotesList[j]=midiNotes[9]; j = j + 1;
        midiNotesList[j]=midiNotes[11]; j = j + 1;
      }
      else if (analogPin2Reading>610 && analogPin2Reading<=622) {
        midiNotesList[j]=midiNotes[5]; j = j + 1;
        midiNotesList[j]=midiNotes[7]; j = j + 1;
        midiNotesList[j]=midiNotes[11]; j = j + 1;
      }
      else if (analogPin2Reading>622 && analogPin2Reading<=642) {
        midiNotesList[j]=midiNotes[5]; j = j + 1;
        midiNotesList[j]=midiNotes[9]; j = j + 1;
        midiNotesList[j]=midiNotes[11]; j = j + 1;
      }
      else if (analogPin2Reading>642 && analogPin2Reading<=660) {
        midiNotesList[j]=midiNotes[7]; j = j + 1;
        midiNotesList[j]=midiNotes[9]; j = j + 1;
        midiNotesList[j]=midiNotes[11]; j = j + 1;
      }
      else if (analogPin2Reading>660) {
        midiNotesList[j]=midiNotes[5]; j = j + 1;
        midiNotesList[j]=midiNotes[7]; j = j + 1;
        midiNotesList[j]=midiNotes[9]; j = j + 1;
        midiNotesList[j]=midiNotes[11]; j = j + 1;
      }
    }
  }
  if (analogPin1Reading>10) {
    if (analogPin1Reading<=175 || (analogPin1Reading>=480 && analogPin1Reading<=512)) {
      if (analogPin1Reading<=100) {
        noteIndex=6;        
      }
      else if (analogPin1Reading>100 && analogPin1Reading<=175) {
        noteIndex=8;        
      }
      else if (analogPin1Reading>175) {
        noteIndex=10;        
      }
      midiNotesList[j]=midiNotes[noteIndex];
      j = j + 1;
    }
    else {
      if (analogPin1Reading<=240) {
        midiNotesList[j]=midiNotes[6]; j = j + 1;
        midiNotesList[j]=midiNotes[8]; j = j + 1;
      }
      else if (analogPin1Reading>240 && analogPin1Reading<=535) {
        midiNotesList[j]=midiNotes[6]; j = j + 1;
        midiNotesList[j]=midiNotes[10]; j = j + 1;
      }
      else if (analogPin1Reading>535 && analogPin1Reading<=560) {
        midiNotesList[j]=midiNotes[8]; j = j + 1;
        midiNotesList[j]=midiNotes[10]; j = j + 1;
      }
      else if (analogPin1Reading>560) {
        midiNotesList[j]=midiNotes[6]; j = j + 1;
        midiNotesList[j]=midiNotes[8]; j = j + 1;
        midiNotesList[j]=midiNotes[10]; j = j + 1;
      }
    }
  }
 
  // decide whether to use the piezo or midi software
  int pinModeReading=digitalRead(detectModePin);
  if (pinModeReading==HIGH && noteIndex!=12) {
    tone(piezoPin,notes[noteIndex],30);
  }
  else {
    // for notes in current noteslist but not in previous noteslist, play them
    for (int k = 0; k < 12; k++) {
      int noteBeingTested=midiNotesList[k];
      int n=0;
      for (int i = 0; i < 12; i++) {
        if (noteBeingTested!=previousMidiNotesList[i]) {
          n=n+1;
        }
      }
      if (n==12 && noteBeingTested!=0) {
        MIDI(144,noteBeingTested,127);
      }
    }
    // for notes in previous noteslist but not in current noteslist, stop them
    for (int k = 0; k < 12; k++) {
      int noteBeingTested=previousMidiNotesList[k];
      int n=0;
      for (int i = 0; i < 12; i++) {
        if (noteBeingTested!=midiNotesList[i]) {
          n=n+1;
        }
      }
      if (n==12 && noteBeingTested!=0) {
        MIDI(128,noteBeingTested,127);
      }
    }
    
    if (midiNotesList[0]!=0) {
      //// The current state is ended either with a change in analog readings or in potentiometer reading
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
      int newOctavei=octavei;
      while ((newAnalogReading1>=10 || newAnalogReading2>=10 || newAnalogReading3>=10 || newAnalogReading4>=10) && (((newAnalogReading1-oldReading1)<=10 && (newAnalogReading1-oldReading1)>=-10) && ((newAnalogReading2-oldReading2)<=10 && (newAnalogReading2-oldReading2)>=-10) && ((newAnalogReading3-oldReading3)<=10 && (newAnalogReading3-oldReading3)>=-10) && ((newAnalogReading4-oldReading4)<=10 && (newAnalogReading4-oldReading4)>=-10)) && (newOctavei==octavei)) {
        oldReading1=newAnalogReading1;
        oldReading2=newAnalogReading2;
        oldReading3=newAnalogReading3;
        oldReading4=newAnalogReading4;
        newAnalogReading1=analogRead(analogReadPin1);
        newAnalogReading2=analogRead(analogReadPin2);
        newAnalogReading3=analogRead(analogReadPin3);
        newAnalogReading4=analogRead(analogReadPin4);
        newPotReading = analogRead(potPin);
        if (newPotReading>750) {newOctavei=0;}
        else if (newPotReading<750 && newPotReading>500) {newOctavei=1;}
        else if (newPotReading<500 && newPotReading>250) {newOctavei=2;}
        else if (newPotReading<250) {newOctavei=3;}
      }
    }
  }
  // record the previous midinoteslist
  for (int k = 0; k < 12; k++){
    previousMidiNotesList[k]=midiNotesList[k];
  }
}

void MIDI(unsigned char MESSAGE, unsigned char PITCH, unsigned char VELOCITY) 
{
  Serial.write(MESSAGE);
  Serial.write(PITCH);
  Serial.write(VELOCITY);
}

void determineOctave(int potReading) {
  if (potReading>750) {
    digitalWrite(octave1,HIGH);
    digitalWrite(octave2,LOW);
    digitalWrite(octave3,LOW);
    octavei=0;
  }
  else if (potReading<750 && potReading>500) {
    digitalWrite(octave1,LOW);
    digitalWrite(octave2,HIGH);
    digitalWrite(octave3,LOW);
    octavei=1;
  }
  else if (potReading<500 && potReading>250) {
    digitalWrite(octave1,LOW);
    digitalWrite(octave2,LOW);
    digitalWrite(octave3,HIGH);
    octavei=2;
  }
  else if (potReading<250) {
    digitalWrite(octave1,LOW);
    digitalWrite(octave2,LOW);
    digitalWrite(octave3,LOW);
    octavei=3;
  }
  // change to the current octave
  notes[0]=c[octavei]; notes[1]=cs[octavei]; notes[2]=d[octavei]; notes[3]=ds[octavei]; notes[4]=e[octavei]; notes[5]=f[octavei];
  notes[6]=fs[octavei];notes[7]=g[octavei]; notes[8]=gs[octavei];notes[9]=a[octavei]; notes[10]=as[octavei]; notes[10]=b[octavei];
  //
  midiNotes[0]=mC[octavei]; midiNotes[1]=mCS[octavei]; midiNotes[2]=mD[octavei]; midiNotes[3]=mDS[octavei]; midiNotes[4]=mE[octavei]; midiNotes[5]=mF[octavei];
  midiNotes[6]=mFS[octavei]; midiNotes[7]=mG[octavei]; midiNotes[8]=mGS[octavei]; midiNotes[9]=mA[octavei]; midiNotes[10]=mAS[octavei]; midiNotes[11]=mB[octavei];
}
