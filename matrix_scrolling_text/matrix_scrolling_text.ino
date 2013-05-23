const int posPin=8; //anode
const int negPin=9; //cathode
const int latchPin=10;
const int clockPin=11;
const int buttonPin=12;
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

#define D  { \
    {0, 1, 1, 1, 1, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 1, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 1, 0, 0}, \
    {0, 1, 1, 1, 1, 0, 0, 0}  \
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

#define O { \
    {0, 0, 1, 1, 1, 1, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 0, 1, 1, 1, 1, 0, 0}  \
}

# define W { \
  {0, 1, 0, 0, 0, 0, 1, 0}, \
  {0, 1, 0, 0, 0, 0, 1, 0}, \
  {0, 1, 0, 0, 0, 0, 1, 0}, \
  {0, 1, 0, 0, 0, 0, 1, 0}, \
  {0, 1, 0, 1, 1, 0, 1, 0}, \
  {0, 1, 0, 1, 1, 0, 1, 0}, \
  {0, 1, 0, 1, 1, 0, 1, 0}, \
  {0, 0, 1, 0, 0, 1, 0, 0}  \
}

# define R { \
  {0, 1, 1, 1, 1, 1, 0, 0}, \
  {0, 1, 0, 0, 0, 0, 1, 0}, \
  {0, 1, 0, 0, 0, 0, 1, 0}, \
  {0, 1, 0, 0, 0, 0, 1, 0}, \
  {0, 1, 1, 1, 1, 1, 0, 0}, \
  {0, 1, 0, 0, 1, 0, 0, 0}, \
  {0, 1, 0, 0, 0, 1, 0, 0}, \
  {0, 1, 0, 0, 0, 0, 1, 0}  \
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

boolean notBlank=false;

const int messageLength = 11;   // it's necessary to set the variable to a constant 

byte patterns[messageLength][8][8] = {
  H,E,L,L,O,W,O,R,L,D,blank
};

byte message[8][messageLength*8+7]={}; //the message matrix

int numberOfFrames=messageLength*8;

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
  createMessage();
  extendMessage(); //adding the first 7 cols to the end of the message matrix
}

void createMessage() {
  for (int i=0; i<=7;i++) {
    for (int j=0; j<messageLength;j++) {
      for (int k=0; k<=7;k++) {
        message[i][j*8+k]=patterns[j][i][k];
      }
    }
  }
}

void extendMessage() {
  for (int i=0; i<=7;i++) {
    for (int j=0; j<=6; j++) {
      message[i][messageLength*8+j]=patterns[0][i][j];
    }
  }
}

int currentFrame=0; //keeping track of the current frame: from 0 to 39, then back to 0
byte currentMatrix[8][8]={};
int previousTime=0;
int currentTime=0;
boolean programStarted=false;

void loop() {
  while (digitalRead(detectButtonPress)==0 & programStarted==false) {
    refreshDisplay(8,8);
  }
  programStarted=true;
  generateFrame();
  while (currentTime-previousTime<=100) {
    displayFrame();
    currentTime = millis();
  }
  previousTime = millis();
  if (currentFrame==numberOfFrames-1) currentFrame=0;
  else currentFrame+=1;
}

void generateFrame() {
  for (int i=0; i<=7; i++) {
    for (int j=0; j<=7; j++) {
      currentMatrix[i][j]=message[i][currentFrame+j];
    }
  }
}

void displayFrame() {  //pIndex stands for pattern index
  for (int i=0; i<=7; i++) {
    for (int j=0; j<=7; j++) {
      if (currentMatrix[i][j]==1) {
        refreshDisplay(i,j);
        boolean notBlank=true;
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

