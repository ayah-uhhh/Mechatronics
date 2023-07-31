#include <Servo.h>
Servo servo1;
const int servoPin = 9;
int data = 0;

void setup(){
  servo1.attach(servoPin);
  Serial.begin(9600);
}

void loop(){
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    data = Serial.read();
    Serial.print("Recieved: ");
    Serial.println(data);
    
      if (data == 'p') {        //check to see if user input is correct
        load();
      } 
      else {
        Serial.print("Incorrect rotation entry \n");
      }
    data = 0;
  }
}

void load() {
   servo1.write(0);    //Spin forward @full speed
   delay(900);       //Adjusted delay time to match one quarter rotation
   servo1.write(90);  //Halt motion
}
