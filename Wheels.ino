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
  Serial.begin(9600);                //begin serial communication with the computer
  Serial.println("Choose a test (1-8 = wheel test, 9 = Interactive, 0 = Stop) "); //Prompt to get input in the serial monitor.
  Serial.setTimeout(1000);
}

void loop() {
  if (Serial.available() > 0){          //if the user has entered something in the serial monitor
    SelectedTest = Serial.parseInt();  //set the motor speed equal to the number in the serial message
    
    if (SelectedTest == 1) {
      FR_Forward(TestSpeed);
    }
    else if (SelectedTest == 2) {
      BR_Forward(TestSpeed);
    }
    else if (SelectedTest == 3) {
      FL_Forward(TestSpeed);
    }
    else if (SelectedTest == 4) {
      BL_Forward(TestSpeed);
    }
    else if (SelectedTest == 5) {
      FR_Backward(TestSpeed);
    }
    else if (SelectedTest == 6) {
      BR_Backward(TestSpeed);
    }
    else if (SelectedTest == 7) {
      FL_Backward(TestSpeed);
    }
    else if (SelectedTest == 8) {
      BL_Backward(TestSpeed);
    }
    else {
      analogWrite(BL_PWM, 0);
      digitalWrite(BL_DC, LOW);
      analogWrite(FL_PWM, 0);
      digitalWrite(FL_DC, LOW);
      analogWrite(BR_PWM, 0);
      digitalWrite(BR_DC, LOW);
      analogWrite(FR_PWM, 0);
      digitalWrite(FR_DC, LOW);
    }
    Serial.print("Selected Test: ");      //print the speed that the motor is set to run at
    Serial.println(SelectedTest);
    
  }
}

int SpeedConvert(int InputSpeed) {
  return(InputSpeed);
}

void FR_Forward(int Speed) {
  digitalWrite(FR_DC, LOW);
  analogWrite(FR_PWM, SpeedConvert(Speed));
}

void BR_Forward(int Speed) {
  digitalWrite(BR_DC, LOW);
  analogWrite(BR_PWM, SpeedConvert(Speed));
}

void FL_Forward(int Speed) {
  digitalWrite(FL_DC, LOW);
  analogWrite(FL_PWM, SpeedConvert(Speed));
}

void BL_Forward(int Speed) {
  digitalWrite(BL_DC, LOW);
  analogWrite(BL_PWM, SpeedConvert(Speed));
}

void FR_Backward(int Speed) {
  digitalWrite(FR_DC, HIGH);
  analogWrite(FR_PWM, SpeedConvert(255-Speed));
}

void BR_Backward(int Speed) {
  digitalWrite(BR_DC, HIGH);
  analogWrite(BR_PWM, SpeedConvert(255-Speed));
}

void FL_Backward(int Speed) {
  digitalWrite(FL_DC, HIGH);
  analogWrite(FL_PWM, SpeedConvert(255-Speed));
}

void BL_Backward(int Speed) {
  digitalWrite(BL_DC, HIGH);
  analogWrite(BL_PWM, SpeedConvert(255-Speed));
}
