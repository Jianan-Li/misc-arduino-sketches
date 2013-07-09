// Written by Jianan Li
// Project Website: jiananli.wordpress.com
// Email: ljn07050@gmail.com, jianan.li@duke.edu

// Anglog In
int const analogReadPin5=A0;  // f2, g2, a2, b2, c3
int const analogReadPin4=A1;  // cs2,ds2,fs2,gs2,as2
int const analogReadPin3=A2;  // a1, b1, c2, d2, e2
int const analogReadPin2=A3;  // cs2,ds2,fs2,gs2,as2
int const analogReadPin1=A4;  // c1, d1, e1, f1, g1
int const detectModePin=7;    // whether piezo or midi
int const potPin=A5;          // change octave

// other pins
int const piezoPin=3;
int const octave1=4;       //these three pins are for LEDs
int const octave2=5;
int const octave3=6;
int const piezoModePin=2;  // switching between these two changes the mode
int const midiModePin=3;

// frequencies for piezo
float c[]={130.8,261.6,523.3,1047,2093,4186};
float cs[]={138.6,277.2,554.4,1109,2217};
float d[]={146.8,293.7,587.3,1175,2349};
float ds[]={155.6,311.1,622.3,1245,2489};
float e[]={164.8,329.6,659.3,1319,2637};
float f[]={174.6,349.2,698.5,1397,2794};
float fs[]={185.0,370.0,740.0,1480,2960};
float g[]={196.0,392.0,784.0,1568,3136};
float gs[]={207.7,415.3,830.6,1661,3322};
float a[]={220.0,440.0,880.0,1760,3520};
float as[]={233.1,466.2,932.3,1865,3729};
float b[]={246.9,493.9,987.8,1976,3951};

// note value for midi
int mC[]={48,60,72,84,96,108};
int mCS[]={49,61,73,85,97};
int mD[]={50,62,74,86,98};
int mDS[]={51,63,75,87,99};
int mE[]={52,64,76,88,100};
int mF[]={53,65,77,89,101};
int mFS[]={54,66,78,90,102};
int mG[]={55,67,79,91,103};
int mGS[]={56,68,80,92,104};
int mA[]={57,69,81,93,105};
int mAS[]={58,70,82,94,106};
int mB[]={59,71,83,95,107};

// voltage thresholds
int t[]={56,103,196,333,517};
int t1[]={ 46, 94,133,187,218,251,280,329,
          349,373,392,420,436,454,469,513,
          525,538,549,564,574,584,593,610,
          619,627,634,645,651,657,663};
int t2[]={ 46, 94,133,189,221,253,279,332,
          354,377,396,424,440,458,473,516,
          526,539,550,566,576,586,595,613,
          621,629,637,648,654,661,668};
          
// dynamic octave determined by potentiometer reading in the function determineOctave()
int octavei=0;
float notes[25]={};           // for piezo
float midiNotes[25]={};       // for midi

// initilize notes
int noteIndex=25;             // for piezo
float midiNotesList[25]={};   // for midi

// for comparison (sending starting and ending midi message)
float previousMidiNotesList[25]={};

void setup() {
  pinMode(piezoModePin,OUTPUT);
  digitalWrite(piezoModePin,HIGH);
  pinMode(midiModePin,OUTPUT);
  digitalWrite(midiModePin,LOW);
  pinMode(detectModePin,INPUT);
  pinMode(octave1,OUTPUT);
  pinMode(octave2,OUTPUT);
  pinMode(octave3,OUTPUT);
  pinMode(piezoPin,OUTPUT);
  Serial.begin(57600);
}

void loop() {
  //Determine Octave
  int potReading=analogRead(potPin);delay(1);
  determineOctave(potReading);
  
  //Determine Note(s)
  noteIndex=25;              //reset
  for (int i=0;i<25;i++) midiNotesList[i]=0; //reset
  int analogPin1Reading=analogRead(analogReadPin1);delay(1);
  int analogPin2Reading=analogRead(analogReadPin2);delay(1);
  int analogPin3Reading=analogRead(analogReadPin3);delay(1);
  int analogPin4Reading=analogRead(analogReadPin4);delay(1);
  int analogPin5Reading=analogRead(analogReadPin5);delay(1);
  
//  Serial.print(analogPin1Reading);
//  Serial.print('\t');
//  Serial.print(analogPin2Reading);
//  Serial.print('\t');
//  Serial.print(analogPin3Reading);
//  Serial.print('\t');
//  Serial.print(analogPin4Reading);
//  Serial.print('\t');
//  Serial.println(analogPin5Reading);

  
  int pinModeReading=digitalRead(detectModePin);
  if (pinModeReading==HIGH) {
    determineNotes(analogPin1Reading,analogPin2Reading,analogPin3Reading,analogPin4Reading,analogPin5Reading);
  }
  else {
    determineMIDINotes(analogPin1Reading,analogPin2Reading,analogPin3Reading,analogPin4Reading,analogPin5Reading);
  }

  // Playing the note(s) using either piezo or midi software
  
  if (pinModeReading==HIGH && noteIndex!=25) {
    tone(piezoPin,notes[noteIndex]);
    int previousIndex=noteIndex;
    while (noteIndex!=25) {
      noteIndex=25;
      analogPin1Reading=analogRead(analogReadPin1);delay(1);
      analogPin2Reading=analogRead(analogReadPin2);delay(1);
      analogPin3Reading=analogRead(analogReadPin3);delay(1);
      analogPin4Reading=analogRead(analogReadPin4);delay(1);
      analogPin5Reading=analogRead(analogReadPin5);delay(1);
      determineNotes(analogPin1Reading,analogPin2Reading,analogPin3Reading,analogPin4Reading,analogPin5Reading);
//      Serial.print(analogPin1Reading);
//      Serial.print('\t');
//      Serial.print(analogPin2Reading);
//      Serial.print('\t');
//      Serial.print(analogPin3Reading);
//      Serial.print('\t');
//      Serial.print(analogPin4Reading);
//      Serial.print('\t');
//      Serial.print(analogPin5Reading);
//      Serial.print('\t');
//      Serial.print(previousIndex);
//      Serial.print('\t');
//      Serial.println(noteIndex);
      if (noteIndex!=previousIndex) {
        noTone(piezoPin);
        break;
      }
    }
  }
  else {
    // for notes in current noteslist but not in previous noteslist, play them
    for (int k = 0; k < 25; k++) {
      int noteBeingTested=midiNotesList[k];
      int n=0;
      for (int i = 0; i < 25; i++) {
        if (noteBeingTested!=previousMidiNotesList[i]) {
          n=n+1;
        }
      }
      if (n==25 && noteBeingTested!=0) {
        MIDI(144,noteBeingTested,127);
      }
    }
    // for notes in previous noteslist but not in current noteslist, stop them
    for (int k = 0; k < 25; k++) {
      int noteBeingTested=previousMidiNotesList[k];
      int n=0;
      for (int i = 0; i < 25; i++) {
        if (noteBeingTested!=midiNotesList[i]) {
          n=n+1;
        }
      }
      if (n==25 && noteBeingTested!=0) {
        MIDI(128,noteBeingTested,127);
      }
    }
    // Only goes into this 'pausing' state when there is key press
    // if there is any notes being played, the first element of the midiNotesList array wouldn't be zero
    if (midiNotesList[0]!=0) {
      // The current state is ended either with a change in 4 analog readings or the potentiometer reading
      pause(analogPin1Reading,analogPin2Reading,analogPin3Reading,analogPin4Reading,analogPin5Reading,potReading);
    }
  }
  
  // Keep a copy of the previous midinoteslist
  for (int k = 0; k < 25; k++){
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
  if (potReading>824) {
    digitalWrite(octave1,HIGH);
    digitalWrite(octave2,LOW);
    digitalWrite(octave3,LOW);
    octavei=0;
  }
  else if (potReading<=824 && potReading>624) {
    digitalWrite(octave1,LOW);
    digitalWrite(octave2,HIGH);
    digitalWrite(octave3,LOW);
    octavei=1;
  }
  else if (potReading<=624 && potReading>424) {
    digitalWrite(octave1,LOW);
    digitalWrite(octave2,LOW);
    digitalWrite(octave3,HIGH);
    octavei=2;
  }
  else if (potReading<=424) {
    digitalWrite(octave1,LOW);
    digitalWrite(octave2,LOW);
    digitalWrite(octave3,LOW);
    octavei=3;
  }
  // change to the current octave
  notes[0]=c[octavei]; notes[1]=cs[octavei]; notes[2]=d[octavei]; notes[3]=ds[octavei]; notes[4]=e[octavei]; notes[5]=f[octavei];
  notes[6]=fs[octavei];notes[7]=g[octavei]; notes[8]=gs[octavei];notes[9]=a[octavei]; notes[10]=as[octavei]; notes[11]=b[octavei];
  notes[12]=c[octavei+1]; notes[13]=cs[octavei+1]; notes[14]=d[octavei+1]; notes[15]=ds[octavei+1]; notes[16]=e[octavei+1]; notes[17]=f[octavei+1];
  notes[18]=fs[octavei+1];notes[19]=g[octavei+1]; notes[20]=gs[octavei+1];notes[21]=a[octavei+1]; notes[22]=as[octavei+1]; notes[23]=b[octavei+1];
  notes[24]=c[octavei+2];
  //
  midiNotes[0]=mC[octavei]; midiNotes[1]=mCS[octavei]; midiNotes[2]=mD[octavei]; midiNotes[3]=mDS[octavei]; midiNotes[4]=mE[octavei]; midiNotes[5]=mF[octavei];
  midiNotes[6]=mFS[octavei]; midiNotes[7]=mG[octavei]; midiNotes[8]=mGS[octavei]; midiNotes[9]=mA[octavei]; midiNotes[10]=mAS[octavei]; midiNotes[11]=mB[octavei];
  midiNotes[12]=mC[octavei+1]; midiNotes[13]=mCS[octavei+1]; midiNotes[14]=mD[octavei+1]; midiNotes[15]=mDS[octavei+1]; midiNotes[16]=mE[octavei+1]; midiNotes[17]=mF[octavei+1];
  midiNotes[18]=mFS[octavei+1]; midiNotes[19]=mG[octavei+1]; midiNotes[20]=mGS[octavei+1]; midiNotes[21]=mA[octavei+1]; midiNotes[22]=mAS[octavei+1]; midiNotes[23]=mB[octavei+1];
  midiNotes[24]=mC[octavei+2];
}

void determineMIDINotes(int P1, int P2, int P3, int P4, int P5) {
  int j=0;
  if (P1>10) {
    if (P1<=t1[0] || (P1>t1[1] && P1<=t1[2]) || (P1>t1[3] && P1<=t1[4]) || (P1>t1[7] && P1<=t1[8])|| (P1>t1[15] && P1<=t1[16])|| (P1>t1[23] && P1<=t1[24])|| (P1>t1[19] && P1<=t1[20])|| (P1>t1[11] && P1<=t1[12])|| (P1>t1[17] && P1<=t1[18])|| (P1>t1[9] && P1<=t1[10])|| (P1>t1[5] && P1<=t1[6])|| (P1>t1[27] && P1<=t1[28])|| (P1>t1[25] && P1<=t1[26])|| (P1>t1[21] && P1<=t1[22])|| (P1>t1[13] && P1<=t1[14])|| (P1>t1[29] && P1<=t1[30])) {midiNotesList[j]=midiNotes[0]; j = j + 1;}
    if ((P1>t1[0] && P1<=t1[1]) || (P1>t1[1] && P1<=t1[2]) || (P1>t1[4] && P1<=t1[5]) || (P1>t1[8] && P1<=t1[9])|| (P1>t1[16] && P1<=t1[17])|| (P1>t1[24] && P1<=t1[25])|| (P1>t1[20] && P1<=t1[21])|| (P1>t1[12] && P1<=t1[13])|| (P1>t1[17] && P1<=t1[18])|| (P1>t1[9] && P1<=t1[10])|| (P1>t1[5] && P1<=t1[6])|| (P1>t1[28] && P1<=t1[29])|| (P1>t1[25] && P1<=t1[26])|| (P1>t1[21] && P1<=t1[22])|| (P1>t1[13] && P1<=t1[14])|| (P1>t1[29] && P1<=t1[30])) {midiNotesList[j]=midiNotes[2]; j = j + 1;}
    if ((P1>t1[2] && P1<=t1[3]) || (P1>t1[3] && P1<=t1[4]) || (P1>t1[4] && P1<=t1[5]) || (P1>t1[10] && P1<=t1[11])|| (P1>t1[18] && P1<=t1[19])|| (P1>t1[26] && P1<=t1[27])|| (P1>t1[20] && P1<=t1[21])|| (P1>t1[12] && P1<=t1[13])|| (P1>t1[19] && P1<=t1[20])|| (P1>t1[11] && P1<=t1[12])|| (P1>t1[5] && P1<=t1[6])|| (P1>t1[28] && P1<=t1[29])|| (P1>t1[27] && P1<=t1[28])|| (P1>t1[21] && P1<=t1[22])|| (P1>t1[13] && P1<=t1[14])|| (P1>t1[29] && P1<=t1[30])) {midiNotesList[j]=midiNotes[4]; j = j + 1;}
    if ((P1>t1[6] && P1<=t1[7]) || (P1>t1[7] && P1<=t1[8]) || (P1>t1[8] && P1<=t1[9]) || (P1>t1[10] && P1<=t1[11])|| (P1>t1[22] && P1<=t1[23])|| (P1>t1[26] && P1<=t1[27])|| (P1>t1[24] && P1<=t1[25])|| (P1>t1[12] && P1<=t1[13])|| (P1>t1[23] && P1<=t1[24])|| (P1>t1[11] && P1<=t1[12])|| (P1>t1[9] && P1<=t1[10])|| (P1>t1[28] && P1<=t1[29])|| (P1>t1[27] && P1<=t1[28])|| (P1>t1[25] && P1<=t1[26])|| (P1>t1[13] && P1<=t1[14])|| (P1>t1[29] && P1<=t1[30])) {midiNotesList[j]=midiNotes[5]; j = j + 1;}
    if ((P1>t1[15] && P1<=t1[16]) || (P1>t1[16] && P1<=t1[17]) || (P1>t1[18] && P1<=t1[19]) || (P1>t1[22] && P1<=t1[23])|| (P1>t1[14] && P1<=t1[15])|| (P1>t1[26] && P1<=t1[27])|| (P1>t1[24] && P1<=t1[25])|| (P1>t1[20] && P1<=t1[21])|| (P1>t1[23] && P1<=t1[24])|| (P1>t1[19] && P1<=t1[20])|| (P1>t1[17] && P1<=t1[18])|| (P1>t1[28] && P1<=t1[29])|| (P1>t1[27] && P1<=t1[28])|| (P1>t1[25] && P1<=t1[26])|| (P1>t1[21] && P1<=t1[22])|| (P1>t1[29] && P1<=t1[30])) {midiNotesList[j]=midiNotes[7]; j = j + 1;}
  }
  if (P2>10) {
    if (P2<=t1[0] || (P2>t1[1] && P2<=t1[2]) || (P2>t1[3] && P2<=t1[4]) || (P2>t1[7] && P2<=t1[8])|| (P2>t1[15] && P2<=t1[16])|| (P2>t1[23] && P2<=t1[24])|| (P2>t1[19] && P2<=t1[20])|| (P2>t1[11] && P2<=t1[12])|| (P2>t1[17] && P2<=t1[18])|| (P2>t1[9] && P2<=t1[10])|| (P2>t1[5] && P2<=t1[6])|| (P2>t1[27] && P2<=t1[28])|| (P2>t1[25] && P2<=t1[26])|| (P2>t1[21] && P2<=t1[22])|| (P2>t1[13] && P2<=t1[14])|| (P2>t1[29] && P2<=t1[30])) {midiNotesList[j]=midiNotes[1]; j = j + 1;}
    if ((P2>t1[0] && P2<=t1[1]) || (P2>t1[1] && P2<=t1[2]) || (P2>t1[4] && P2<=t1[5]) || (P2>t1[8] && P2<=t1[9])|| (P2>t1[16] && P2<=t1[17])|| (P2>t1[24] && P2<=t1[25])|| (P2>t1[20] && P2<=t1[21])|| (P2>t1[12] && P2<=t1[13])|| (P2>t1[17] && P2<=t1[18])|| (P2>t1[9] && P2<=t1[10])|| (P2>t1[5] && P2<=t1[6])|| (P2>t1[28] && P2<=t1[29])|| (P2>t1[25] && P2<=t1[26])|| (P2>t1[21] && P2<=t1[22])|| (P2>t1[13] && P2<=t1[14])|| (P2>t1[29] && P2<=t1[30])) {midiNotesList[j]=midiNotes[3]; j = j + 1;}
    if ((P2>t1[2] && P2<=t1[3]) || (P2>t1[3] && P2<=t1[4]) || (P2>t1[4] && P2<=t1[5]) || (P2>t1[10] && P2<=t1[11])|| (P2>t1[18] && P2<=t1[19])|| (P2>t1[26] && P2<=t1[27])|| (P2>t1[20] && P2<=t1[21])|| (P2>t1[12] && P2<=t1[13])|| (P2>t1[19] && P2<=t1[20])|| (P2>t1[11] && P2<=t1[12])|| (P2>t1[5] && P2<=t1[6])|| (P2>t1[28] && P2<=t1[29])|| (P2>t1[27] && P2<=t1[28])|| (P2>t1[21] && P2<=t1[22])|| (P2>t1[13] && P2<=t1[14])|| (P2>t1[29] && P2<=t1[30])) {midiNotesList[j]=midiNotes[6]; j = j + 1;}
    if ((P2>t1[6] && P2<=t1[7]) || (P2>t1[7] && P2<=t1[8]) || (P2>t1[8] && P2<=t1[9]) || (P2>t1[10] && P2<=t1[11])|| (P2>t1[22] && P2<=t1[23])|| (P2>t1[26] && P2<=t1[27])|| (P2>t1[24] && P2<=t1[25])|| (P2>t1[12] && P2<=t1[13])|| (P2>t1[23] && P2<=t1[24])|| (P2>t1[11] && P2<=t1[12])|| (P2>t1[9] && P2<=t1[10])|| (P2>t1[28] && P2<=t1[29])|| (P2>t1[27] && P2<=t1[28])|| (P2>t1[25] && P2<=t1[26])|| (P2>t1[13] && P2<=t1[14])|| (P2>t1[29] && P2<=t1[30])) {midiNotesList[j]=midiNotes[8]; j = j + 1;}
    if ((P2>t1[15] && P2<=t1[16]) || (P2>t1[16] && P2<=t1[17]) || (P2>t1[18] && P2<=t1[19]) || (P2>t1[22] && P2<=t1[23])|| (P2>t1[14] && P2<=t1[15])|| (P2>t1[26] && P2<=t1[27])|| (P2>t1[24] && P2<=t1[25])|| (P2>t1[20] && P2<=t1[21])|| (P2>t1[23] && P2<=t1[24])|| (P2>t1[19] && P2<=t1[20])|| (P2>t1[17] && P2<=t1[18])|| (P2>t1[28] && P2<=t1[29])|| (P2>t1[27] && P2<=t1[28])|| (P2>t1[25] && P2<=t1[26])|| (P2>t1[21] && P2<=t1[22])|| (P2>t1[29] && P2<=t1[30])) {midiNotesList[j]=midiNotes[10]; j = j + 1;}
  }
  if (P3>10) {
    if (P3<=t1[0] || (P3>t1[1] && P3<=t1[2]) || (P3>t1[3] && P3<=t1[4]) || (P3>t1[7] && P3<=t1[8])|| (P3>t1[15] && P3<=t1[16])|| (P3>t1[23] && P3<=t1[24])|| (P3>t1[19] && P3<=t1[20])|| (P3>t1[11] && P3<=t1[12])|| (P3>t1[17] && P3<=t1[18])|| (P3>t1[9] && P3<=t1[10])|| (P3>t1[5] && P3<=t1[6])|| (P3>t1[27] && P3<=t1[28])|| (P3>t1[25] && P3<=t1[26])|| (P3>t1[21] && P3<=t1[22])|| (P3>t1[13] && P3<=t1[14])|| (P3>t1[29] && P3<=t1[30])) {midiNotesList[j]=midiNotes[9]; j = j + 1;}
    if ((P3>t1[0] && P3<=t1[1]) || (P3>t1[1] && P3<=t1[2]) || (P3>t1[4] && P3<=t1[5]) || (P3>t1[8] && P3<=t1[9])|| (P3>t1[16] && P3<=t1[17])|| (P3>t1[24] && P3<=t1[25])|| (P3>t1[20] && P3<=t1[21])|| (P3>t1[12] && P3<=t1[13])|| (P3>t1[17] && P3<=t1[18])|| (P3>t1[9] && P3<=t1[10])|| (P3>t1[5] && P3<=t1[6])|| (P3>t1[28] && P3<=t1[29])|| (P3>t1[25] && P3<=t1[26])|| (P3>t1[21] && P3<=t1[22])|| (P3>t1[13] && P3<=t1[14])|| (P3>t1[29] && P3<=t1[30])) {midiNotesList[j]=midiNotes[11]; j = j + 1;}
    if ((P3>t1[2] && P3<=t1[3]) || (P3>t1[3] && P3<=t1[4]) || (P3>t1[4] && P3<=t1[5]) || (P3>t1[10] && P3<=t1[11])|| (P3>t1[18] && P3<=t1[19])|| (P3>t1[26] && P3<=t1[27])|| (P3>t1[20] && P3<=t1[21])|| (P3>t1[12] && P3<=t1[13])|| (P3>t1[19] && P3<=t1[20])|| (P3>t1[11] && P3<=t1[12])|| (P3>t1[5] && P3<=t1[6])|| (P3>t1[28] && P3<=t1[29])|| (P3>t1[27] && P3<=t1[28])|| (P3>t1[21] && P3<=t1[22])|| (P3>t1[13] && P3<=t1[14])|| (P3>t1[29] && P3<=t1[30])) {midiNotesList[j]=midiNotes[12]; j = j + 1;}
    if ((P3>t1[6] && P3<=t1[7]) || (P3>t1[7] && P3<=t1[8]) || (P3>t1[8] && P3<=t1[9]) || (P3>t1[10] && P3<=t1[11])|| (P3>t1[22] && P3<=t1[23])|| (P3>t1[26] && P3<=t1[27])|| (P3>t1[24] && P3<=t1[25])|| (P3>t1[12] && P3<=t1[13])|| (P3>t1[23] && P3<=t1[24])|| (P3>t1[11] && P3<=t1[12])|| (P3>t1[9] && P3<=t1[10])|| (P3>t1[28] && P3<=t1[29])|| (P3>t1[27] && P3<=t1[28])|| (P3>t1[25] && P3<=t1[26])|| (P3>t1[13] && P3<=t1[14])|| (P3>t1[29] && P3<=t1[30])) {midiNotesList[j]=midiNotes[14]; j = j + 1;}
    if ((P3>t1[15] && P3<=t1[16]) || (P3>t1[16] && P3<=t1[17]) || (P3>t1[18] && P3<=t1[19]) || (P3>t1[22] && P3<=t1[23])|| (P3>t1[14] && P3<=t1[15])|| (P3>t1[26] && P3<=t1[27])|| (P3>t1[24] && P3<=t1[25])|| (P3>t1[20] && P3<=t1[21])|| (P3>t1[23] && P3<=t1[24])|| (P3>t1[19] && P3<=t1[20])|| (P3>t1[17] && P3<=t1[18])|| (P3>t1[28] && P3<=t1[29])|| (P3>t1[27] && P3<=t1[28])|| (P3>t1[25] && P3<=t1[26])|| (P3>t1[21] && P3<=t1[22])|| (P3>t1[29] && P3<=t1[30])) {midiNotesList[j]=midiNotes[16]; j = j + 1;}
  }
  if (P4>10) {
    if (P4<=t1[0] || (P4>t1[1] && P4<=t1[2]) || (P4>t1[3] && P4<=t1[4]) || (P4>t1[7] && P4<=t1[8])|| (P4>t1[15] && P4<=t1[16])|| (P4>t1[23] && P4<=t1[24])|| (P4>t1[19] && P4<=t1[20])|| (P4>t1[11] && P4<=t1[12])|| (P4>t1[17] && P4<=t1[18])|| (P4>t1[9] && P4<=t1[10])|| (P4>t1[5] && P4<=t1[6])|| (P4>t1[27] && P4<=t1[28])|| (P4>t1[25] && P4<=t1[26])|| (P4>t1[21] && P4<=t1[22])|| (P4>t1[13] && P4<=t1[14])|| (P4>t1[29] && P4<=t1[30])) {midiNotesList[j]=midiNotes[13]; j = j + 1;}
    if ((P4>t1[0] && P4<=t1[1]) || (P4>t1[1] && P4<=t1[2]) || (P4>t1[4] && P4<=t1[5]) || (P4>t1[8] && P4<=t1[9])|| (P4>t1[16] && P4<=t1[17])|| (P4>t1[24] && P4<=t1[25])|| (P4>t1[20] && P4<=t1[21])|| (P4>t1[12] && P4<=t1[13])|| (P4>t1[17] && P4<=t1[18])|| (P4>t1[9] && P4<=t1[10])|| (P4>t1[5] && P4<=t1[6])|| (P4>t1[28] && P4<=t1[29])|| (P4>t1[25] && P4<=t1[26])|| (P4>t1[21] && P4<=t1[22])|| (P4>t1[13] && P4<=t1[14])|| (P4>t1[29] && P4<=t1[30])) {midiNotesList[j]=midiNotes[15]; j = j + 1;}
    if ((P4>t1[2] && P4<=t1[3]) || (P4>t1[3] && P4<=t1[4]) || (P4>t1[4] && P4<=t1[5]) || (P4>t1[10] && P4<=t1[11])|| (P4>t1[18] && P4<=t1[19])|| (P4>t1[26] && P4<=t1[27])|| (P4>t1[20] && P4<=t1[21])|| (P4>t1[12] && P4<=t1[13])|| (P4>t1[19] && P4<=t1[20])|| (P4>t1[11] && P4<=t1[12])|| (P4>t1[5] && P4<=t1[6])|| (P4>t1[28] && P4<=t1[29])|| (P4>t1[27] && P4<=t1[28])|| (P4>t1[21] && P4<=t1[22])|| (P4>t1[13] && P4<=t1[14])|| (P4>t1[29] && P4<=t1[30])) {midiNotesList[j]=midiNotes[18]; j = j + 1;}
    if ((P4>t1[6] && P4<=t1[7]) || (P4>t1[7] && P4<=t1[8]) || (P4>t1[8] && P4<=t1[9]) || (P4>t1[10] && P4<=t1[11])|| (P4>t1[22] && P4<=t1[23])|| (P4>t1[26] && P4<=t1[27])|| (P4>t1[24] && P4<=t1[25])|| (P4>t1[12] && P4<=t1[13])|| (P4>t1[23] && P4<=t1[24])|| (P4>t1[11] && P4<=t1[12])|| (P4>t1[9] && P4<=t1[10])|| (P4>t1[28] && P4<=t1[29])|| (P4>t1[27] && P4<=t1[28])|| (P4>t1[25] && P4<=t1[26])|| (P4>t1[13] && P4<=t1[14])|| (P4>t1[29] && P4<=t1[30])) {midiNotesList[j]=midiNotes[20]; j = j + 1;}
    if ((P4>t1[15] && P4<=t1[16]) || (P4>t1[16] && P4<=t1[17]) || (P4>t1[18] && P4<=t1[19]) || (P4>t1[22] && P4<=t1[23])|| (P4>t1[14] && P4<=t1[15])|| (P4>t1[26] && P4<=t1[27])|| (P4>t1[24] && P4<=t1[25])|| (P4>t1[20] && P4<=t1[21])|| (P4>t1[23] && P4<=t1[24])|| (P4>t1[19] && P4<=t1[20])|| (P4>t1[17] && P4<=t1[18])|| (P4>t1[28] && P4<=t1[29])|| (P4>t1[27] && P4<=t1[28])|| (P4>t1[25] && P4<=t1[26])|| (P4>t1[21] && P4<=t1[22])|| (P4>t1[29] && P4<=t1[30])) {midiNotesList[j]=midiNotes[22]; j = j + 1;}
  }
  if (P5>10) {
    if (P5<=t1[0] || (P5>t1[1] && P5<=t1[2]) || (P5>t1[3] && P5<=t1[4]) || (P5>t1[7] && P5<=t1[8])|| (P5>t1[15] && P5<=t1[16])|| (P5>t1[23] && P5<=t1[24])|| (P5>t1[19] && P5<=t1[20])|| (P5>t1[11] && P5<=t1[12])|| (P5>t1[17] && P5<=t1[18])|| (P5>t1[9] && P5<=t1[10])|| (P5>t1[5] && P5<=t1[6])|| (P5>t1[27] && P5<=t1[28])|| (P5>t1[25] && P5<=t1[26])|| (P5>t1[21] && P5<=t1[22])|| (P5>t1[13] && P5<=t1[14])|| (P5>t1[29] && P5<=t1[30])) {midiNotesList[j]=midiNotes[17]; j = j + 1;}
    if ((P5>t1[0] && P5<=t1[1]) || (P5>t1[1] && P5<=t1[2]) || (P5>t1[4] && P5<=t1[5]) || (P5>t1[8] && P5<=t1[9])|| (P5>t1[16] && P5<=t1[17])|| (P5>t1[24] && P5<=t1[25])|| (P5>t1[20] && P5<=t1[21])|| (P5>t1[12] && P5<=t1[13])|| (P5>t1[17] && P5<=t1[18])|| (P5>t1[9] && P5<=t1[10])|| (P5>t1[5] && P5<=t1[6])|| (P5>t1[28] && P5<=t1[29])|| (P5>t1[25] && P5<=t1[26])|| (P5>t1[21] && P5<=t1[22])|| (P5>t1[13] && P5<=t1[14])|| (P5>t1[29] && P5<=t1[30])) {midiNotesList[j]=midiNotes[19]; j = j + 1;}
    if ((P5>t1[2] && P5<=t1[3]) || (P5>t1[3] && P5<=t1[4]) || (P5>t1[4] && P5<=t1[5]) || (P5>t1[10] && P5<=t1[11])|| (P5>t1[18] && P5<=t1[19])|| (P5>t1[26] && P5<=t1[27])|| (P5>t1[20] && P5<=t1[21])|| (P5>t1[12] && P5<=t1[13])|| (P5>t1[19] && P5<=t1[20])|| (P5>t1[11] && P5<=t1[12])|| (P5>t1[5] && P5<=t1[6])|| (P5>t1[28] && P5<=t1[29])|| (P5>t1[27] && P5<=t1[28])|| (P5>t1[21] && P5<=t1[22])|| (P5>t1[13] && P5<=t1[14])|| (P5>t1[29] && P5<=t1[30])) {midiNotesList[j]=midiNotes[21]; j = j + 1;}
    if ((P5>t1[6] && P5<=t1[7]) || (P5>t1[7] && P5<=t1[8]) || (P5>t1[8] && P5<=t1[9]) || (P5>t1[10] && P5<=t1[11])|| (P5>t1[22] && P5<=t1[23])|| (P5>t1[26] && P5<=t1[27])|| (P5>t1[24] && P5<=t1[25])|| (P5>t1[12] && P5<=t1[13])|| (P5>t1[23] && P5<=t1[24])|| (P5>t1[11] && P5<=t1[12])|| (P5>t1[9] && P5<=t1[10])|| (P5>t1[28] && P5<=t1[29])|| (P5>t1[27] && P5<=t1[28])|| (P5>t1[25] && P5<=t1[26])|| (P5>t1[13] && P5<=t1[14])|| (P5>t1[29] && P5<=t1[30])) {midiNotesList[j]=midiNotes[23]; j = j + 1;}
    if ((P5>t1[15] && P5<=t1[16]) || (P5>t1[16] && P5<=t1[17]) || (P5>t1[18] && P5<=t1[19]) || (P5>t1[22] && P5<=t1[23])|| (P5>t1[14] && P5<=t1[15])|| (P5>t1[26] && P5<=t1[27])|| (P5>t1[24] && P5<=t1[25])|| (P5>t1[20] && P5<=t1[21])|| (P5>t1[23] && P5<=t1[24])|| (P5>t1[19] && P5<=t1[20])|| (P5>t1[17] && P5<=t1[18])|| (P5>t1[28] && P5<=t1[29])|| (P5>t1[27] && P5<=t1[28])|| (P5>t1[25] && P5<=t1[26])|| (P5>t1[21] && P5<=t1[22])|| (P5>t1[29] && P5<=t1[30])) {midiNotesList[j]=midiNotes[24]; j = j + 1;}
  }
}

void determineNotes(int analogPin1Reading, int analogPin2Reading, int analogPin3Reading, int analogPin4Reading, int analogPin5Reading) {
  if (analogPin1Reading>15) {
    if (analogPin1Reading<=t[0]) noteIndex=0;
    else if (analogPin1Reading>t[1]-40 && analogPin1Reading<=t[1]) noteIndex=2;
    else if (analogPin1Reading>t[2]-40 && analogPin1Reading<=t[2]) noteIndex=4;
    else if (analogPin1Reading>t[3]-40 && analogPin1Reading<=t[3]) noteIndex=5;
    else if (analogPin1Reading>t[4]-40 && analogPin1Reading<=t[4]) noteIndex=7;
  }
  if (analogPin2Reading>15) {
    if (analogPin2Reading<=t[0]) noteIndex=1;
    else if (analogPin2Reading>t[1]-40 && analogPin2Reading<=t[1]) noteIndex=3;
    else if (analogPin2Reading>t[2]-40 && analogPin2Reading<=t[2]) noteIndex=6;
    else if (analogPin2Reading>t[3]-40 && analogPin2Reading<=t[3]) noteIndex=8;
    else if (analogPin2Reading>t[4]-40 && analogPin2Reading<=t[4]) noteIndex=10;
  }
  if (analogPin3Reading>15) {
    if (analogPin3Reading<=t[0]) noteIndex=9;
    else if (analogPin3Reading>t[1]-40 && analogPin3Reading<=t[1]) noteIndex=11;
    else if (analogPin3Reading>t[2]-40 && analogPin3Reading<=t[2]) noteIndex=12;
    else if (analogPin3Reading>t[3]-100 && analogPin3Reading<=t[3]) noteIndex=14;
    else if (analogPin3Reading>t[4]-40 && analogPin3Reading<=t[4]) noteIndex=16;
  }
  if (analogPin4Reading>15) {
    if (analogPin4Reading<=t[0]) noteIndex=13;
    else if (analogPin4Reading>t[1]-40 && analogPin4Reading<=t[1]) noteIndex=15;   
    else if (analogPin4Reading>t[2]-40 && analogPin4Reading<=t[2]) noteIndex=18;
    else if (analogPin4Reading>t[3]-40 && analogPin4Reading<=t[3]) noteIndex=20;
    else if (analogPin4Reading>t[4]-40 && analogPin4Reading<=t[4]) noteIndex=22;
  }
  if (analogPin5Reading>15) {
    if (analogPin5Reading<=t[0]) noteIndex=17;
    else if (analogPin5Reading>t[1]-40 && analogPin5Reading<=t[1]) noteIndex=19;   
    else if (analogPin5Reading>t[2]-40 && analogPin5Reading<=t[2]) noteIndex=21;
    else if (analogPin5Reading>t[3]-40 && analogPin5Reading<=t[3]) noteIndex=23;
    else if (analogPin5Reading>t[4]-40 && analogPin5Reading<=t[4]) noteIndex=24;
  }
}


void pause(int analogPin1Reading, int analogPin2Reading, int analogPin3Reading, int analogPin4Reading,int analogPin5Reading, int potReading) {
  // The current state is ended either with a change in 4 analog readings or the potentiometer reading
  int oldReading1= analogPin1Reading;
  int oldReading2= analogPin2Reading;
  int oldReading3= analogPin3Reading;
  int oldReading4= analogPin4Reading;
  int oldReading5= analogPin5Reading;
  int newAnalogReading1= oldReading1;
  int newAnalogReading2= oldReading2;
  int newAnalogReading3= oldReading3;
  int newAnalogReading4= oldReading4;
  int newAnalogReading5= oldReading5;
  //compare potentiometer readings
  int newPotReading=potReading;
  int newOctavei=octavei;
  while ((newAnalogReading1>=10 || newAnalogReading2>=10 || newAnalogReading3>=10 || newAnalogReading4>=10 || newAnalogReading5>=10) && (((newAnalogReading1-oldReading1)<=5 && (newAnalogReading1-oldReading1)>=-5) && ((newAnalogReading2-oldReading2)<=5 && (newAnalogReading2-oldReading2)>=-5) && ((newAnalogReading3-oldReading3)<=5 && (newAnalogReading3-oldReading3)>=-5) && ((newAnalogReading4-oldReading4)<=5 && (newAnalogReading4-oldReading4)>=-5) && ((newAnalogReading5-oldReading5)<=5 && (newAnalogReading5-oldReading5)>=-5)) && (newOctavei==octavei)) {
    oldReading1=newAnalogReading1;
    oldReading2=newAnalogReading2;
    oldReading3=newAnalogReading3;
    oldReading4=newAnalogReading4;
    oldReading5=newAnalogReading5;
    newAnalogReading1=analogRead(analogReadPin1);delay(1);
    newAnalogReading2=analogRead(analogReadPin2);delay(1);
    newAnalogReading3=analogRead(analogReadPin3);delay(1);
    newAnalogReading4=analogRead(analogReadPin4);delay(1);
    newAnalogReading5=analogRead(analogReadPin5);delay(1);
    newPotReading = analogRead(potPin);delay(1);
    if (newPotReading>824) newOctavei=0;
    else if (newPotReading<=824 && newPotReading>624) newOctavei=1;
    else if (newPotReading<=624 && newPotReading>424) newOctavei=2;
    else if (newPotReading<=424) newOctavei=3;
  }
}
