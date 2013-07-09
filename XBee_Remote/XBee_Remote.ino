#define LjsH A4
#define LjsV A5
#define LjsS 12
#define RjsH A0
#define RjsV A1
#define RjsS 9
#define RU 8
#define RD 6
#define RL 5
#define RR 7
#define LU 13
#define LD 4
#define LL 3
#define LR 2
#define LS 11
#define RS 10

int leftHorValue=0;
int leftVerValue=0;
int rightHorValue=0;
int rightVerValue=0;

void setup() {
  pinMode(LjsS,INPUT);
  pinMode(RjsS,INPUT);
  pinMode(LU,INPUT);
  pinMode(LD,INPUT);
  pinMode(LL,INPUT);
  pinMode(LR,INPUT);
  pinMode(RU,INPUT);
  pinMode(RD,INPUT);
  pinMode(RL,INPUT);
  pinMode(RR,INPUT);
  pinMode(LS,INPUT);
  pinMode(RS,INPUT);
  Serial.begin(19200);
}

void loop() {
  if (digitalRead(LS)==1) Serial.write(21);
  else Serial.write(20);
  if (digitalRead(RS)==1) Serial.write(31);
  else Serial.write(30);
  rightHorValue=(analogRead(RjsH)*0.99/128)+1;  // convert 0-1023 to 1-8 (small adjustment to the horizontal analog reading)
  Serial.write(rightHorValue);
  rightVerValue=(analogRead(RjsV)/128)+1;
  Serial.write(rightVerValue);
  delay(50);
  //uncomment to see all the data that can be transmitted
//  if (digitalRead(LjsS)==0) Serial.print(21);
//  else Serial.print(20);
//  Serial.print("  ");
//  leftHorValue=(analogRead(LjsH)/128)+1;
//  Serial.print(leftHorValue);
//  Serial.print("  ");
//  leftVerValue=(analogRead(LjsV)/128)+1;
//  Serial.print(leftVerValue);
//  Serial.print("  ");
//  if (digitalRead(RjsS)==0) Serial.print(1);
//  else Serial.print(0);
//  Serial.print("  ");
//  rightHorValue=(analogRead(RjsH)*0.99/128)+1;
//  Serial.print(rightHorValue);
//  Serial.print("  ");
//  rightVerValue=(analogRead(RjsV)/128)+1;
//  Serial.print(rightVerValue);
//  Serial.print("  ");
//  Serial.print(digitalRead(LU));
//  Serial.print("  ");
//  Serial.print(digitalRead(LD));
//  Serial.print("  ");
//  Serial.print(digitalRead(LL));
//  Serial.print("  ");
//  Serial.print(digitalRead(LR));
//  Serial.print("  ");
//  Serial.print(digitalRead(RU));
//  Serial.print("  ");
//  Serial.print(digitalRead(RD));
//  Serial.print("  ");
//  Serial.print(digitalRead(RL));
//  Serial.print("  ");
//  Serial.print(digitalRead(RR));
//  Serial.print("  ");
//  Serial.print(digitalRead(LS));
//  Serial.print("  ");
//  Serial.println(digitalRead(RS));
//  delay(50);
}
