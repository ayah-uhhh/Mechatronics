#include <Servo.h>

// Define the servo pin
const int servoPin1 = 9;
const int servoPin = 10;
// Create servo object
Servo servo1;
Servo servo2;
int desiredAngle = 0;            // Variable to store the desired angle
bool newAngleReceived = false;  // Flag to indicate if a new angle is received
bool startangle = true;
#define in1 4
#define in2 5
#define in3 6
#define in4 7

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Set initial rotation direction
  kills();

  Serial.begin(9600);
  // Attach servo to the pin
  servo1.attach(servoPin1);
  servo2.attach(servoPin);
}

void loop() {
  delay(10);
  if (startangle == true) {
    desiredAngle = 35;
    startangle = false;
    newAngleReceived = true;
  }
  
  if (Serial.available() > 0) {
    char data = Serial.read(); //letter then dash
    Serial.print("Ack");
    Serial.println(data);

    if (data == 'd') {
      // Increase the angle by 5 degrees
      desiredAngle += 5;
      if (desiredAngle > 35) {
        desiredAngle = 35;
      }
      newAngleReceived = true; // Set the flag to move the servo to the new angle
    }
    else if (data == 'u') {
      // Decrease the angle by 5 degrees
      desiredAngle -= 5;
      if (desiredAngle < 0) {
        desiredAngle = 0;
      }
      newAngleReceived = true; // Set the flag to move the servo to the new angle
    }
    else if (data == 'y') {
      fire(); // Call the fire function when the "pitch" command is received p
      delay(2000);
      push();
      delay(3000);
      kills();
    }
    else if (data == 'p') {
      push(); // Call the push function when the "push" command is received p
    }
    else if (data == 'k') {
      kills(); // omg stop
    }
    else if (data == 'q') {
      // Set the desired angle to 35 degrees
      desiredAngle = 35;
      newAngleReceived = true; // Set the flag to move the servo to the new angle
    }
  }

  // Move the servo to the desired angle if a new angle is received
  if (newAngleReceived == true) {
    moveServo(desiredAngle);
    newAngleReceived = false;
  }
}

void moveServo(int angle) {
  // Move the servo to the desired angle
  servo1.write(angle);
}

void fire() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void kills() 
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void push() {
   servo2.write(0);    //Spin forward @full speed
   delay(900);       //Adjusted delay time to match one quarter rotation
   servo2.write(90);  //Halt motion
}
