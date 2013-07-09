#define NUMBER_OF_SHIFT_CHIPS   3
#define DATA_WIDTH   NUMBER_OF_SHIFT_CHIPS * 8
#define PULSE_WIDTH_USEC  5
#define BYTES_VAL_T unsigned long

int ploadPin        = 12;  // Connects to Parallel load pin
int dataPin         = 2;  // Connects to the Q7 pin
int clockPin        = 13;  // Connects to the Clock pin
int speakerPin      = 11;  // Connects to the speaker
int lastNotePin     = 7; // Coonests to the 25th note 
int detectModePin   = 8;  // determine whether to use speaker or midi
int octave1         = 3; //three LEDs
int octave2         = 4;
int octave3         = 5;
int octave4         = 6;
int potPin          = A5;
int mode = 0;  // determine whether to use the speaker or midi

BYTES_VAL_T lastNote;
BYTES_VAL_T pinValues;
BYTES_VAL_T oldPinValues;

// frequencies for speaker
float noteFrequency[6][12]={
  {130.8, 138.6, 146.8, 155.6, 164.8, 174.6, 185.0, 196.0, 207.7, 220.0, 233.1, 246.9},
  {261.6, 277.2, 293.7, 311.1, 329.6, 349.2, 370.0, 392.0, 415.3, 440.0, 466.2, 493.9}, 
  {523.3, 554.4, 587.3, 622.3, 659.3, 698.5, 740.0, 784.0, 830.6, 880.0, 932.3, 987.8}, 
  {1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976}, 
  {2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951}, 
  {4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902}};

int noteValue[6][12]={
  {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59}, 
  {60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71}, 
  {72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83}, 
  {84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95}, 
  {96, 97, 98, 99,100,101,102,103,104,105,106,107}, 
  {108, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}};

float currentKeyboardFre[25]={};
int currentKeyboardVal[25]={};
int octave=0;
int potReading=0;
BYTES_VAL_T currentMidiNotes;
BYTES_VAL_T previousMidiNotes;

BYTES_VAL_T read_shift_regs()
{
    BYTES_VAL_T bitVal;
    BYTES_VAL_T bytesVal = 0;
    
    digitalWrite(ploadPin, LOW);
    delayMicroseconds(PULSE_WIDTH_USEC);
    digitalWrite(ploadPin, HIGH);
    
    //check the last note first because it represent the biggest value 2^25
    lastNote=digitalRead(lastNotePin);
    bytesVal |= lastNote << 24;
    
    for(int i = 0; i < DATA_WIDTH; i++)
    {
        bitVal = digitalRead(dataPin);
        bytesVal |= (bitVal << ((DATA_WIDTH-1) - i));

        digitalWrite(clockPin, HIGH); //rising edge
        delayMicroseconds(PULSE_WIDTH_USEC);
        digitalWrite(clockPin, LOW);
    }
    return(bytesVal);
}

void setup()
{
    Serial.begin(57600);
    pinMode(ploadPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, INPUT);
    pinMode(detectModePin,INPUT);
    digitalWrite(clockPin, LOW);
    digitalWrite(ploadPin, HIGH);
    pinMode(octave1,OUTPUT);
    pinMode(octave2,OUTPUT);
    pinMode(octave3,OUTPUT);
    pinMode(octave4,OUTPUT);
    pinValues = read_shift_regs();
    //display_pin_values();
    oldPinValues = pinValues;
}

void determineOctave() {
  potReading=analogRead(potPin);
  if (potReading>824) {
    digitalWrite(octave1,HIGH);
    digitalWrite(octave2,LOW);
    digitalWrite(octave3,LOW);
    digitalWrite(octave4,LOW);
    octave=0;
  }
  else if (potReading<=824 && potReading>624) {
    digitalWrite(octave1,LOW);
    digitalWrite(octave2,HIGH);
    digitalWrite(octave3,LOW);
    digitalWrite(octave4,LOW);
    octave=1;
  }
  else if (potReading<=624 && potReading>424) {
    digitalWrite(octave1,LOW);
    digitalWrite(octave2,LOW);
    digitalWrite(octave3,HIGH);
    digitalWrite(octave4,LOW);
    octave=2;
  }
  else if (potReading<=424) {
    digitalWrite(octave1,LOW);
    digitalWrite(octave2,LOW);
    digitalWrite(octave3,LOW);
    digitalWrite(octave4,HIGH);
    octave=3;
  }
}

void currentKeyboard() {
  for (int i = 0; i < 12; i++) {
    currentKeyboardFre[i]=noteFrequency[octave][i];
    currentKeyboardFre[i+12]=noteFrequency[octave+1][i];
    currentKeyboardVal[i]=noteValue[octave][i];
    currentKeyboardVal[i+12]=noteValue[octave+1][i];
  }
  currentKeyboardFre[24]=noteFrequency[octave+2][0];
  currentKeyboardVal[24]=noteValue[octave+2][0];
}

void speakerMode() {
  for(int i = 0; i <= DATA_WIDTH; i++) {  //from 0 to 24 corresponding to 25 keys
    if ((pinValues >> i) & 1) {
      tone(speakerPin, currentKeyboardFre[i]);
      break;
    }
    if (i==24) noTone(speakerPin);
  }
}

void midiMode() {
  currentMidiNotes=pinValues;
  for(int i = 0; i <= DATA_WIDTH; i++) {
    if (((currentMidiNotes >> i) & 1) && !((previousMidiNotes >> i & 1))) {
      MIDI(144,currentKeyboardVal[i],127);
    }
    else if (!((currentMidiNotes >> i) & 1) && ((previousMidiNotes >> i & 1))) {
      MIDI(128,currentKeyboardVal[i],127);
    }
  }
  previousMidiNotes=currentMidiNotes;
}

void MIDI(unsigned char MESSAGE, unsigned char PITCH, unsigned char VELOCITY) {
  Serial.write(MESSAGE);
  Serial.write(PITCH);
  Serial.write(VELOCITY);
}

void loop() {                                                       
    pinValues = read_shift_regs();
    determineOctave();
    currentKeyboard();
    if(pinValues != oldPinValues) {
      mode=digitalRead(detectModePin);
      if (mode==0)  speakerMode();
      else midiMode();
      oldPinValues = pinValues;
    }
}
