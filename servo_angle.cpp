#include <Servo.h>

// Define the servo pin
const int servoPin1 = 9;

// Create servo object
Servo servo1;

int desiredAngle = 0;            // Variable to store the desired angle
bool newAngleReceived = false;  // Flag to indicate if a new angle is received

void setup() {
  Serial.begin(9600);
  // Attach servo to the pin
  servo1.attach(servoPin1);
}

void loop() {
  // Read the desired angle from the user if a new angle is not already received
  if (!newAngleReceived) {
    desiredAngle = readAngle();
    newAngleReceived = true;
  }
  
  // Move the servo to the desired angle if a new angle is received
  if (newAngleReceived) {
    moveServo(desiredAngle);
    newAngleReceived = false;
  }
}

int readAngle() {
  // Prompt the user to enter an angle
  Serial.println("Enter an angle between 0 and 90 degrees:");
  
  while (true) {
    if (Serial.available() > 0) {
      // Read the input from the serial monitor
      int angle = Serial.parseInt();
      
      // Validate the input
      if (angle >= 0 && angle <= 90) {
        return angle;
      }
      else {
        // Prompt the user to enter a valid angle
        Serial.println("Invalid angle. Please enter an angle between 0 and 90 degrees:");
      }
    }
  }
}

void moveServo(int angle) {
  // Move the servo to the desired angle
  servo1.write(angle);
}