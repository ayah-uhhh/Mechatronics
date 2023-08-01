#define FALSE (0)
#define TRUE  (!FALSE)

#include <Motoron.h>

// Define the GPIO ports
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
const int DefaultCommandTimeout = 300;

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

  // By default, the Motoron is configured to stop the motors if
  // it does not get a motor control command for 1500 ms.  You
  // can uncomment a line below to adjust this time or disable
  // the timeout feature.
  // mc.setCommandTimeoutMilliseconds(1000);
  // mc.disableCommandTimeout();
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
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
/*
*/
  pinMode(FrontCornerInner, INPUT);           // set pin to input
  pinMode(FrontCornerOuter, INPUT);           // set pin to input
  pinMode(FrontSideInner  , INPUT);           // set pin to input
  pinMode(FrontSideOuter  , INPUT);           // set pin to input
  pinMode(BackCornerInner , INPUT);           // set pin to input
  pinMode(BackCornerOuter , INPUT);           // set pin to input
  pinMode(BackSideInner   , INPUT);           // set pin to input
  pinMode(BackSideOuter   , INPUT);           // set pin to input

  Serial.println("Hello World!!!2");
  //Serial.write('Y');

}

void loop() {
  delay(10);

  if (Serial.available()) {
    int SerialCommand = Serial.read();

    //Serial.write(SerialCommand);
    switch (SerialCommand) {
      //case 'd': // Backward
      case 'r': // Backward
        Serial.println("Ackr");
        FrontCtrl.setSpeed(LeftMotor, -DefaultSpeed);
        FrontCtrl.setSpeed(RightMotor, -DefaultSpeed);
        BackCtrl.setSpeed(LeftMotor, -DefaultSpeed);
        BackCtrl.setSpeed(RightMotor, -DefaultSpeed);
      break;
      //case 'f': // Forward
      case 'l': // Forward
        Serial.println("Ackl");
        FrontCtrl.setSpeed(LeftMotor, DefaultSpeed);
        FrontCtrl.setSpeed(RightMotor, DefaultSpeed);
        BackCtrl.setSpeed(LeftMotor, DefaultSpeed);
        BackCtrl.setSpeed(RightMotor, DefaultSpeed);
      break;
      /*
      //case 'a': // Strafe left
      case 'l': // Strafe left
        FrontCtrl.setSpeed(LeftMotor, -DefaultSpeed);
        FrontCtrl.setSpeed(RightMotor, DefaultSpeed);
        BackCtrl.setSpeed(LeftMotor, DefaultSpeed);
        BackCtrl.setSpeed(RightMotor, -DefaultSpeed);
      break;
      //case 's': // Strafe right
      case 'r': // Strafe right
        FrontCtrl.setSpeed(LeftMotor, DefaultSpeed);
        FrontCtrl.setSpeed(RightMotor, -DefaultSpeed);
        BackCtrl.setSpeed(LeftMotor, -DefaultSpeed);
        BackCtrl.setSpeed(RightMotor, DefaultSpeed);
      break;
      /*
      case 'e': // Pivot left
        FrontCtrl.setSpeed(LeftMotor, 0);
        FrontCtrl.setSpeed(RightMotor, DefaultSpeed);
        BackCtrl.setSpeed(LeftMotor, 0);
        BackCtrl.setSpeed(RightMotor, DefaultSpeed);
      break;
      case 'r': // Pivot right
        FrontCtrl.setSpeed(LeftMotor, DefaultSpeed);
        FrontCtrl.setSpeed(RightMotor, 0);
        BackCtrl.setSpeed(LeftMotor, DefaultSpeed);
        BackCtrl.setSpeed(RightMotor, 0);
      break;
      case 'q':
      break;
      case 'w':
      break;
      default:
      break;
      */
    }
  }
}

