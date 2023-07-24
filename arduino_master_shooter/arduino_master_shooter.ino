#include <Servo.h>

// Define the servo pin
const int servoPin1 = 9;

// Create servo object
Servo servo1;

int desiredAngle = 0;            // Variable to store the desired angle
bool newAngleReceived = false;  // Flag to indicate if a new angle is received
bool firerecieved = false;

#define in1 4
#define in2 5
#define in3 6
#define in4 7
//#define in5 8

int rotDirection = 0; //the rotation direction
int pressed = false;

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  //pinMode(in5, INPUT);
  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  Serial.begin(9600);
  // Attach servo to the pin
  servo1.attach(servoPin1);
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('-');
    Serial.print("Ack");
    Serial.println(data);

    if (data == "u") {
      // Increase the angle by 5 degrees
      desiredAngle += 5;
      if (desiredAngle > 90) {
        desiredAngle = 90;
      }
      newAngleReceived = true; // Set the flag to move the servo to the new angle
    }
    else if (data == "d") {
      // Decrease the angle by 5 degrees
      desiredAngle -= 5;
      if (desiredAngle < 0) {
        desiredAngle = 0;
      }
      newAngleReceived = true; // Set the flag to move the servo to the new angle
    }
    else if (data == "p") {
      fire(); // Call the fire function when the "pitch" command is received
      firerecieved = true;
    }
    else if (data == "r") {
      // Set the desired angle to 0 degrees
      desiredAngle = 0;
      newAngleReceived = true; // Set the flag to move the servo to the new angle
    }
  }

  // Move the servo to the desired angle if a new angle is received
  if (newAngleReceived) {
    moveServo(desiredAngle);
    newAngleReceived = false;
  }
}

void moveServo(int angle) {
  // Move the servo to the desired angle
  servo1.write(angle);
}

void fire() {
  int potValue = analogRead(A0); // Read potentiometer value
  int pwmOutput = map(potValue, 0, 1023, 0, 255); // Map the potentiometer value from 0 to 255


  // If in5 is pressed - change rotation direction
  if (firerecieved = true & rotDirection == 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    rotDirection = 1;
    delay(20);
  }
  // If in5 is pressed - change rotation direction
  if (firerecieved = true & rotDirection == 1) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    rotDirection = 0;
    delay(20);
  }
}
