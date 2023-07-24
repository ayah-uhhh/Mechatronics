#include <Servo.h>

// Define the servo pin
const int servoPin1 = 9;

// Create servo object
Servo servo1;

int desiredAngle = 0;            // Variable to store the desired angle
bool newAngleReceived = false;  // Flag to indicate if a new angle is received

#define in1 4
#define in2 5
#define in3 6
#define in4 7
#define in5 8

int rotDirection = 0;
int pressed = false;

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(in5, INPUT);
  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

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
  // Attach servo to the pin
  servo1.attach(servoPin1);
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('-');
    Serial.print("You sent me: ");
    Serial.println(data);

    if (data == "up") {
      // Increase the angle by 5 degrees
      desiredAngle += 5;
      if (desiredAngle > 90) {
        desiredAngle = 90;
      }
      newAngleReceived = true; // Set the flag to move the servo to the new angle
    }
    else if (data == "down") {
      // Decrease the angle by 5 degrees
      desiredAngle -= 5;
      if (desiredAngle < 0) {
        desiredAngle = 0;
      }
      newAngleReceived = true; // Set the flag to move the servo to the new angle
    }
    else if (data == "fire") {
      fire(); // Call the fire function when the "fire" command is received
    }
    else if (data == "reset") {
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

  // Read in5 - Debounce
  if (digitalRead(in5) == true) {
    pressed = !pressed;
  }
  while (digitalRead(in5) == true);
  delay(20);

  // If in5 is pressed - change rotation direction
  if (pressed == true & rotDirection == 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    rotDirection = 1;
    delay(20);
  }
  // If in5 is pressed - change rotation direction
  if (pressed == false & rotDirection == 1) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    rotDirection = 0;
    delay(20);
  }
}
