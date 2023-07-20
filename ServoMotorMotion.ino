#include <Servo.h>
Servo servo1;
int servoPin = 9;

void setup(){
  servo1.attach(servoPin);
}

void loop(){
  servo1.write(0);    //Spin forward @full speed
  delay(202);       //Adjusted delay time to match one quarter rotation
  servo1.write(90);  //Halt motion
  delay(500);         
  //servo1.write(180);
  delay(1000);
}
