%servo code maybe

#include <Servo.h>

// Define the servo pins
const int servoPin1 = 9;
const int servoPin2 = 10;

// Create servo objects
Servo servo1;
Servo servo2;

void setup() {
  // Attach servos to the respective pins
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
}

void loop() {
  // Read the desired angle from the user
  int desiredAngle = readAngle();
  
  // Move both servos to the desired angle
  moveServos(desiredAngle);
  
  // Delay before reading the next desired angle
  delay(1000);
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

void moveServos(int angle) {
  // Move both servos to the desired angle
  servo1.write(angle);
  servo2.write(angle);
}
