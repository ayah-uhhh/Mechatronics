// This is a simple handshaker program for testing a very simple serial protocol
// All the motor control and/or button code is deleted/ignore/commented out because
//   this isn't intended to run on actual hardware, but rather for simulation for
//   the Pi. This will be expanded upon as more features are needed.

#define FALSE (0)
#define TRUE  (!FALSE)

#include <Motoron.h>

// Define the digital GPIO ports
const int FrontCornerInner = 2;  // These are all digital inputs
const int FrontCornerOuter = 3;
const int FrontSideInner   = 4;
const int FrontSideOuter   = 5;
const int BackCornerInner  = 6;
const int BackCornerOuter  = 7;
const int BackSideInner    = 8;
const int BackSideOuter    = 9;

// Define the analog GPIO ports and associated constants
#define ADC_STEPS        (1024)
#define ADC_HYSTERESIS   (200)
#define ADC_THRESHOLD_HI ((ADC_STEPS/2)+(ADC_HYSTERESIS/2))
#define ADC_THRESHOLD_LO ((ADC_STEPS/2)-(ADC_HYSTERESIS/2))

int AnalogButton1;   // Use the analog inputs for buttons...
int AnalogButton2;
int DigitalButton1;  // ...and here are the digitized versions, which
int DigitalButton2;  //   use analog hysterisis for debouncing
int PreviousButton1; // We save the previous state of the buttons so we
int PreviousButton2; //   can act upon a change of state of the button
int TriggerButton1;  // The "Trigger" bits here are set on the initial
int TriggerButton2;  //   press of the corresponding button

// Define the Motoron motor controller constants
const int FrontControllerAddress = 15;
const int BackControllerAddress = 16;
const int RightMotor = 1;
const int LeftMotor = 2;
const int DefaultSpeed = 500;
const int DefaultAcceleration = 100;
const int DefaultDeceleration = 100;
const int DefaultCommandTimeout = 500;

// This code creates an object for each Motoron controller.
// The number passed as the first argument to each constructor
// below should be the 7-bit I2C address of the controller.
MotoronI2C FrontCtrl(FrontControllerAddress);
MotoronI2C BackCtrl(BackControllerAddress);

// You can call functions directly on each of the objects
// created above (mc1, mc2, etc.) but if you want to write
// reusable code, the function below shows how to do that
// using references, a feature of the C++ language.
void setupMotoron(MotoronI2C & mc) {
  mc.reinitialize();
  mc.disableCrc();

  // Clear the reset flag, which is set after the controller
  // reinitializes and counts as an error.
  mc.clearResetFlag();
}

void setup() {
  // Wire.begin(); // Set up the I2C bus
  Serial.begin(9600);

  // Initialize the Motoron controllers in the case where they exist
/*
  setupMotoron(FrontCtrl);
  setupMotoron(BackCtrl);

  FrontCtrl.setCommandTimeoutMilliseconds(DefaultCommandTimeout);
  BackCtrl.setCommandTimeoutMilliseconds(DefaultCommandTimeout);

  FrontCtrl.setMaxAcceleration(LeftMotor, DefaultAcceleration);
  FrontCtrl.setMaxDeceleration(LeftMotor, DefaultDeceleration);

  FrontCtrl.setMaxAcceleration(RightMotor, DefaultAcceleration);
  FrontCtrl.setMaxDeceleration(RightMotor, DefaultDeceleration);

  BackCtrl.setMaxAcceleration(LeftMotor, DefaultAcceleration);
  BackCtrl.setMaxDeceleration(LeftMotor, DefaultDeceleration);

  BackCtrl.setMaxAcceleration(RightMotor, DefaultAcceleration);
  BackCtrl.setMaxDeceleration(RightMotor, DefaultDeceleration);
*/
}

void loop() {
  delay(1); // Poll using a 1 msec timebase

  if (Serial.available()) {
    int SerialCommand = Serial.read();
    switch (SerialCommand) {
      case 's': // Start run
        Serial.println("Acks");
      break;
      case 'l': // Slide left "move forward"
        Serial.println("Ackl");
      break;
      case 'r': // Slide right "move backward"
        Serial.println("Ackr");
      break;
      default: // Unknown command
        Serial.println("????");
      break;
    }
  }

// The buttons are read as analog inputs, and then hysteresis is applied
  AnalogButton1 = analogRead(A0); // Read the analog values of the LPF buttons
  AnalogButton2 = analogRead(A1);

  if (AnalogButton1 > ADC_THRESHOLD_HI) {
    DigitalButton1 = HIGH;
  }
  else if (AnalogButton1 < ADC_THRESHOLD_LO) {
    DigitalButton1 = LOW;
  }

  if (AnalogButton2 > ADC_THRESHOLD_HI) {
    DigitalButton2 = HIGH;
  }
  else if (AnalogButton2 < ADC_THRESHOLD_LO) {
    DigitalButton2 = LOW;
  }

  if(DigitalButton1 == LOW) { // The switch is pressed
    if (PreviousButton1 == HIGH) { //   and it's the initial press
      TriggerButton1 = TRUE;
    }
    PreviousButton1 = LOW;
  }
  else{                               // 
    PreviousButton1 = HIGH;
  }

  if(DigitalButton2 == LOW) { // The switch is pressed
    if (PreviousButton2 == HIGH) { //   and it's the initial press
      TriggerButton2 = TRUE;
    }
    PreviousButton2 = LOW;
  }
  else{                               // 
    PreviousButton2 = HIGH;
  }

  if (TriggerButton1 == TRUE) { // Put Button1 initial press code here
    TriggerButton1 = FALSE;

  }

  if (TriggerButton2 == TRUE) { // Put Button1 initial press code here
    TriggerButton2 = FALSE;

  }

}

