//PIN VARIABLES
const int FR_DC  = 2; // FR = Wheel 1
const int FR_PWM = 3;
const int BR_DC  = 4; // BR = Wheel 2
const int BR_PWM = 5;
const int FL_PWM = 6; // FL = Wheel 3
const int FL_DC  = 7;
const int BL_DC  = 8; // BL = Wheel 4
const int BL_PWM = 9;
int PwmFrequency = 400;

int SelectedTest;
int TestSpeed = 150;

void setup() {
  pinMode(BL_PWM, OUTPUT);
  pinMode(BL_DC , OUTPUT);
  pinMode(FL_PWM, OUTPUT);
  pinMode(FL_DC , OUTPUT);
  pinMode(BR_PWM, OUTPUT);
  pinMode(BR_DC , OUTPUT);
  pinMode(FR_PWM, OUTPUT);
  pinMode(FR_DC , OUTPUT);
  analogWrite(BL_PWM, 0);
  digitalWrite(BL_DC, LOW);
  analogWrite(FL_PWM, 0);
  digitalWrite(FL_DC, LOW);
  analogWrite(BR_PWM, 0);
  digitalWrite(BR_DC, LOW);
  analogWrite(FR_PWM, 0);
  digitalWrite(FR_DC, LOW);
  Serial.begin(9600);
}
void loop(){
  if (Serial.available() > 0){
    String data = Serial.readStringUntil('-');
    Serial.print("You sent me: ");
    Serial.println(data);
  }
  if(data == "up")
  {
  }
  else if(data == "down")
  {
    
  }
  else if(data == "fire")
  {
    
  }
}
