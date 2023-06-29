//PIN VARIABLES
const int FR_DC  = 2; // FR = Wheel 1
const int FR_PWM = 3;
const int BR_DC  = 4; // BR = Wheel 2
const int BR_PWM = 5;
const int FL_PWM = 6; // FL = Wheel 3
const int FL_DC  = 7;
const int BL_DC  = 8; // BL = Wheel 4
const int BL_PWM = 9;
const int buttonPin = 13;
int PwmFrequency = 400;

int SelectedTest;
int sped = 150;
int dig;
int dig2;
int dig3;
int buttonState = 0;
int trigger = 0;
int state; // 0 is right correction, 1 is left correction

void setup() {
  // put your setup code here, to run once:
pinMode(49, INPUT);
pinMode(51, INPUT);
pinMode(53, INPUT);
pinMode(10, INPUT);
pinMode(buttonPin,INPUT);
Serial.begin(9600);
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
  
  }

void loop() {
  // put your main code here, to run repeatedly:

   buttonState = digitalRead(buttonPin); 
  if (buttonState == HIGH) {
    trigger = trigger + 1;
     delay(500);
    Serial.print(trigger);
  }
  if (trigger == 1)
    {
       dig = digitalRead(51);
 // int analog = analogRead(51); //center

   dig2 = digitalRead(49);
  //int analog2 = analogRead(49); //left

   dig3 = digitalRead(53);
  //int analog3 = analogRead(53); //right

  if(dig == 0) //center is on
      {
        if(sped <= 255 && sped >= 0)
        {
        FL_Forward(sped);
        FR_Forward(sped);
        BL_Forward(sped);
        BR_Forward(sped);
        }
    }
  if(dig == 1) //center is off
     {if(dig2 == 0) // left is on
        {//move left somehow
          if(sped <= 255 && sped >= 0)
        {
        
        FL_Forward(sped);
        FR_Backward(sped);
        BL_Forward(sped);
        BR_Backward(sped);
        state = 0;
      }
        }
      if(dig3 == 0) //right is on
      {//move right somehow
        if(sped <= 255 && sped >= 0)
      {
      FL_Backward(sped);
      FR_Forward(sped);
      BL_Backward(sped);
      BR_Forward(sped);
      state = 1;
    }
      }
    if(dig2 == 1 && dig3 == 1) //right and left off
      {//stopp
        if(sped <= 255 && sped >= 0)
      {
     FL_Forward(0);
     FR_Forward(0);
     BL_Forward(0);
     BR_Forward(0);
    }
      
    }
     }
   
  if(dig == 1 && dig2 == 1 && dig3 == 1)//overshooting
  {
    if(state == 0) // overshoot left turn, go right to fix
    {
        FL_Forward(sped);
        FR_Backward(sped);
        BL_Forward(sped);
        BR_Backward(sped);      
    }
    if(state == 1) // overshoot right turn, go left to fix
    {
     FL_Backward(sped);
      FR_Forward(sped);
      BL_Backward(sped);
      BR_Forward(sped);
    }
    }
  }
else
  {
    Serial.println("Here");
    FL_Forward(0);
    FR_Forward(0);
    BL_Forward(0);
    BR_Forward(0);
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
