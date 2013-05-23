const int posPin=8; //anode
const int negPin=9; //cathode
const int latchPin=10;
const int clockPin=11;
const int buttonPin=12;  //switching between images
const int detectButtonPress=A0;

#define H { \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 1, 1, 1, 1, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}  \
}

#define E  { \
    {0, 1, 1, 1, 1, 1, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 1, 1, 1, 1, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 1, 1, 1, 1, 1, 0}  \
}

#define L { \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 1, 1, 1, 1, 1, 0}  \
}

#define L { \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 1, 1, 1, 1, 1, 0}  \
}

#define O { \
    {0, 0, 0, 1, 1, 0, 0, 0}, \
    {0, 0, 1, 0, 0, 1, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 0, 1, 0, 0, 1, 0, 0}, \
    {0, 0, 0, 1, 1, 0, 0, 0}  \
}

#define creeper { \
  {1, 1, 1, 1, 1, 1, 1, 1}, \
  {1, 1, 1, 1, 1, 1, 1, 1}, \
  {1, 0, 0, 1, 1, 0, 0, 1}, \
  {1, 0, 0, 1, 1, 0, 0, 1}, \
  {1, 1, 1, 0, 0, 1, 1, 1}, \
  {1, 1, 0, 0, 0, 0, 1, 1}, \
  {1, 1, 0, 0, 0, 0, 1, 1}, \
  {1, 1, 0, 1, 1, 0, 1, 1}  \
}

# define heart { \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 1, 1, 0, 1, 1, 0, 0}, \
  {1, 0, 0, 1, 0, 0, 1, 0}, \
  {1, 0, 0, 0, 0, 0, 1, 0}, \
  {0, 1, 0, 0, 0, 1, 0, 0}, \
  {0, 0, 1, 0, 1, 0, 0, 0}, \
  {0, 0, 0, 1, 0, 0, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}  \
}

# define all { \
  {1, 1, 1, 1, 1, 1, 1, 1}, \
  {1, 1, 1, 1, 1, 1, 1, 1}, \
  {1, 1, 1, 1, 1, 1, 1, 1}, \
  {1, 1, 1, 1, 1, 1, 1, 1}, \
  {1, 1, 1, 1, 1, 1, 1, 1}, \
  {1, 1, 1, 1, 1, 1, 1, 1}, \
  {1, 1, 1, 1, 1, 1, 1, 1}, \
  {1, 1, 1, 1, 1, 1, 1, 1}  \
}

# define blank { \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}, \
  {0, 0, 0, 0, 0, 0, 0, 0}  \
}

int numberOfPatterns=9;
int blankIndex=0;

byte patterns[9][8][8] = {
  blank,H,E,L,L,O,creeper,heart,all
};

int currentPattern=0;

boolean notBlank=false;

void setup() 
{
  pinMode(posPin,OUTPUT);
  pinMode(negPin,OUTPUT);
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  digitalWrite(posPin,LOW);
  digitalWrite(negPin,LOW);
  digitalWrite(clockPin,LOW);
  digitalWrite(latchPin,LOW);
  //button for changing patterns
  pinMode(buttonPin,OUTPUT);
  digitalWrite(buttonPin,HIGH);
}

void loop() {
  while (digitalRead(detectButtonPress)==1) displayPattern(blankIndex);
  while (digitalRead(detectButtonPress)==0) displayPattern(currentPattern);
  if (currentPattern==numberOfPatterns-1) currentPattern=0;
  else currentPattern+=1;
}

void displayPattern(int pIndex) {  //pIndex stands for pattern index
  for (int i=0; i<=7; i++) {
    for (int j=0; j<=7; j++) {
      if (patterns[pIndex][i][j]==1) {
        refreshDisplay(i,j);
        boolean notBlank=true;
        delayMicroseconds(100); //about the biggest possible interval for refreshing the 64 leds one by one without being perceived
      }
    }
  }
  if (notBlank==false) {
    refreshDisplay(8,8);
  }
}

void refreshDisplay(int row, int col) { //both row and col are from 0 to 7
  for (int i=0; i<=7; i++) {
    if (i==row) digitalWrite(posPin,HIGH);
    else digitalWrite(posPin,LOW);
    if (i==col) digitalWrite(negPin,LOW);
    else digitalWrite(negPin,HIGH);
    
    digitalWrite(clockPin,HIGH);
    digitalWrite(clockPin,LOW);
  }
  digitalWrite(latchPin,HIGH);
  digitalWrite(latchPin,LOW);
}

