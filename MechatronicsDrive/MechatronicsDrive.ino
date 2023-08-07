#define FALSE (0)
#define TRUE  (!FALSE)

#include <Motoron.h>

// Define the GPIO ports
const int FrontCornerOuter = 45; // These are the IR line sensor digital inputs
const int FrontCornerInner = 44;
const int FrontSideInner   = 43;
const int FrontSideOuter   = 42;
const int BackSideOuter    = 49;
const int BackSideInner    = 48;
const int BackCornerInner  = 47;
const int BackCornerOuter  = 46;

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
const int LeftControllerAddress = 15;
const int RightControllerAddress = 16;
const int FrontMotor = 1;
const int BackMotor = 2;
const int MinFwdSpeed = 0;
const int MaxFwdSpeed = 600;
const int DefaultSpeed = 400;
const int DefaultAcceleration = 100;
const int DefaultDeceleration = 500;
const int DefaultCommandTimeout = 100;

// Define the driving states and variables
const int DriveIdle = 0;
const int DriveStraight = 1;
const int DriveInsideCorner = 2;
const int DriveOutsideCorner = 3;
const int DriveFindLine = 4;
const int DriveAlignLineBack = 5;
const int DriveAlignLineFront = 6;
const int DriveStrafeFrontRight = 7;
const int DriveStrafeBackRight = 8;

const int VeerCorrection = 30;
const int PivotSpeed = DefaultSpeed;
const int InsideCornerSpeedLeft = -300;
const int InsideCornerSpeedRight = 250;
const int DefaultLoopTimeMs = 20;
const int BrakingLoopTimeMs = 100;
const int OvershootLoopTimeMs = 100;

int DriveState = DriveIdle;
int FrontRightSpeed = DefaultSpeed;
int FrontLeftSpeed = DefaultSpeed;
int BackRightSpeed = DefaultSpeed;
int BackLeftSpeed = DefaultSpeed;
int StraightCount = 0;
int CornerCount = 0;
int LoopDelayTime = DefaultLoopTimeMs;

// This code creates an object for each Motoron controller.
// The number passed as the first argument to each constructor
// below should be the 7-bit I2C address of the controller.
MotoronI2C LeftCtrl(LeftControllerAddress);
MotoronI2C RightCtrl(RightControllerAddress);

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
  Wire.begin();
  Serial.begin(9600);

  setupMotoron(LeftCtrl);
  setupMotoron(RightCtrl);

  LeftCtrl.setCommandTimeoutMilliseconds(DefaultCommandTimeout);
  RightCtrl.setCommandTimeoutMilliseconds(DefaultCommandTimeout);

  LeftCtrl.setMaxAcceleration(FrontMotor, DefaultAcceleration);
  LeftCtrl.setMaxDeceleration(FrontMotor, DefaultDeceleration);

  LeftCtrl.setMaxAcceleration(BackMotor, DefaultAcceleration);
  LeftCtrl.setMaxDeceleration(BackMotor, DefaultDeceleration);

  RightCtrl.setMaxAcceleration(FrontMotor, DefaultAcceleration);
  RightCtrl.setMaxDeceleration(FrontMotor, DefaultDeceleration);

  RightCtrl.setMaxAcceleration(BackMotor, DefaultAcceleration);
  RightCtrl.setMaxDeceleration(BackMotor, DefaultDeceleration);

  pinMode(FrontCornerInner, INPUT); // Set the IR sensor pins to inputs
  pinMode(FrontCornerOuter, INPUT);
  pinMode(FrontSideInner  , INPUT);
  pinMode(FrontSideOuter  , INPUT);
  pinMode(BackCornerInner , INPUT);
  pinMode(BackCornerOuter , INPUT);
  pinMode(BackSideInner   , INPUT);
  pinMode(BackSideOuter   , INPUT);

  LoopDelayTime = DefaultLoopTimeMs;

  Serial.println("Hello World!!!"); // Show sign of life
}

void loop() {
  delay(LoopDelayTime);
  LoopDelayTime = DefaultLoopTimeMs;
  ButtonsHandle();
  if (Serial.available()) {
    int SerialCommand = Serial.read();
    Serial.write(SerialCommand);
    switch (SerialCommand) {
      case 's': // Start
        //Serial.println("Starting..."); // Start
        Serial.println("Acks");
        CornerCount = 0;
        DriveState = DriveStrafeFrontRight;
        StrafeFrontRight();
        DriveContinue(); 
        /*
        DriveForwardDefault();
        PivotRight();
        DriveContinue(); // Should check sensors first to see if we're already good
        */ 
      break;
      case 'c': // Take the corner
        Serial.println("Take the corner and find the next");
        CornerLeft();
        DriveContinue(); // Should check sensors first to see if we're already good
      break;
      case 'L': // Pivot left by shifting the back (shooter perspective) right
        FrontLeftGo(-(DefaultSpeed/2));
        BackLeftGo(-(DefaultSpeed/2));
        FrontRightGo(0);
        BackRightGo(0);
        Serial.println("AckR");
      break;
      case 'R': // Pivot right by shifting the back (shooter perspective) left
        FrontLeftGo(DefaultSpeed/2);
        BackLeftGo(DefaultSpeed/2);
        FrontRightGo(0);
        BackRightGo(0);
        Serial.println("AckL");
      break;
      //case 'd': // Backward
      case 'r': // Backward
        FrontLeftGo(-DefaultSpeed);
        BackLeftGo(-DefaultSpeed);
        FrontRightGo(-DefaultSpeed);
        BackRightGo(-DefaultSpeed);
        Serial.println("Ackr");
      break;
      //case 'f': // Forward
      case 'l': // Forward
        FrontLeftGo(DefaultSpeed);
        BackLeftGo(DefaultSpeed);
        FrontRightGo(DefaultSpeed);
        BackRightGo(DefaultSpeed);
        Serial.println("Ackl");
      break;
      default:
      break;
    }
  }
  if (TriggerButton1) { // Start by button
    TriggerButton1 = FALSE;
    //Serial.println("Button 1 pressed"); // Start
    Serial.println("Acks");
    /*
    CornerCount = 0;
    DriveForwardDefault();
    */ 
    DriveState = DriveStrafeFrontRight;
    StrafeFrontRight();
    DriveContinue(); 
  }
  if (TriggerButton2) { // Stop by button
    TriggerButton2 = FALSE;
    //Serial.println("Button 2 pressed"); // 
    Serial.println("Ackk");
    //CornerLeft();
    //DriveContinue(); // Should check sensors first to see if we're already good
    DriveState = DriveStrafeBackRight;
    StrafeBackRight();
    DriveContinue(); 
  }


  // Below is where the driving occurs
  switch ( DriveState ) {
    case DriveIdle: // Do nothing
    break;
    case DriveFindLine: // Stop after finding an outside corner
      if ( !SensorDetect(FrontSideOuter) &&
           !SensorDetect(FrontSideInner)
      ) {
        PivotRight();
        DriveContinue(); // 
        break;
      }
      // Fall through to "DriveAlignLineBack" below
    case DriveAlignLineBack: // 
      AllStop();
      DriveState = DriveIdle;

    break;
    case DriveStrafeFrontRight: // 
      if ( !SensorDetect(FrontSideOuter) &&
           !SensorDetect(FrontSideInner)
      ) {
        StrafeFrontRight();
        DriveContinue(); // 
      }
      else {
        AllStop();
        DriveState = DriveIdle;
      }
      break;

    case DriveStrafeBackRight: // 
      if ( !SensorDetect(BackSideOuter) &&
           !SensorDetect(BackSideInner)
      ) {
        StrafeBackRight();
        DriveContinue(); // 
      }
      else {
        AllStop();
        DriveState = DriveIdle;
      }
      break;


    case DriveAlignLineFront: // 

    break;
    case DriveInsideCorner: // Turn left
      if (    SensorDetect(FrontCornerOuter) 
           || SensorDetect(FrontCornerInner)
           //|| SensorDetect(FrontSideInner) 
      ) { // The front edge is still over a line
        CornerLeft();
        DriveContinue();
      }
      else { // Go straight
        DriveForwardDefault();
      }
      /*
      else if ( SensorDetect(FrontSideOuter) ) {
        //AllStop(); // We're back on target, so stop turning
        DriveState = DriveStraight; 
        DriveContinue();
      }
      else { // We are out off the line
        PivotRight();
        LoopDelayTime = OvershootLoopTimeMs;
        DriveContinue();
      }
      */
      break;
    case DriveStraight:
      if ( SensorDetect(FrontCornerOuter) ||
           SensorDetect(FrontCornerInner)) 
      { // The front edge is over a line
        Serial.println("Corner detected");
        Serial.println("Ackc");
        //AllStop();
        /*
        CornerLeft();
        DriveContinue();
        Serial.println("Ackc");
        */
        if (CornerCount >= 3) {
          AllStop(); // Stop
        }
        else {
          CornerCount++;
          CornerLeft();
          DriveContinue();
        }
        /*
        */
      }
      else {
        if (SensorDetect(FrontSideInner)) { // We have veered to the right, over the line
          if (FrontRightSpeed < MaxFwdSpeed) {
            FrontRightSpeed = DefaultSpeed + VeerCorrection;
          }
          if (FrontLeftSpeed > MinFwdSpeed) {
            FrontLeftSpeed = DefaultSpeed - VeerCorrection;
          }
          Serial.println("Front veered to the right");
        }
        else if (!SensorDetect(FrontSideOuter)) { // We have veered to the left, off the line
          if (FrontRightSpeed > MinFwdSpeed) {
            FrontRightSpeed = DefaultSpeed - VeerCorrection;
          }
          if (FrontLeftSpeed < MaxFwdSpeed) {
            FrontLeftSpeed = DefaultSpeed + VeerCorrection;
          }
          Serial.println("Front veered to the left");
        }
        else { // This is the correct sensor readings for straight driving
          FrontRightSpeed = DefaultSpeed;
          FrontLeftSpeed = DefaultSpeed; 
        }
        DriveContinue(); // This sends the updated speed to all the wheels
      }
      break;
    default:
      Serial.println("Ackl"); // Lost
      AllStop();
      DriveState = DriveIdle;
    break;
  }
}

void AllStop() {
  CornerCount = 0;
  DriveState = DriveIdle;
  LoopDelayTime = BrakingLoopTimeMs;
  LeftCtrl.setBraking(BackMotor, BackLeftSpeed);
  LeftCtrl.setBraking(FrontMotor, FrontLeftSpeed);  
  RightCtrl.setBraking(BackMotor, BackRightSpeed);  
  RightCtrl.setBraking(FrontMotor, FrontRightSpeed);    
}

void PivotRight() {
  //DriveState = DriveFindLine;
  FrontLeftSpeed = PivotSpeed;
  BackLeftSpeed = -PivotSpeed;
  FrontRightSpeed = -PivotSpeed;
  BackRightSpeed = PivotSpeed;
}











void StrafeFrontRight() {
  //DriveState = DriveFindLine;
  FrontLeftSpeed = -PivotSpeed;
  BackLeftSpeed = PivotSpeed;
  FrontRightSpeed = 0;
  BackRightSpeed = -PivotSpeed;
}

void StrafeBackRight() {
  //DriveState = DriveFindLine;
  FrontLeftSpeed = PivotSpeed;
  BackLeftSpeed = -PivotSpeed;
  FrontRightSpeed = PivotSpeed;
  BackRightSpeed = 0;
}














void CornerLeft() {
  DriveState = DriveInsideCorner;
  FrontLeftSpeed = InsideCornerSpeedLeft;
  BackLeftSpeed = InsideCornerSpeedLeft;
  FrontRightSpeed = InsideCornerSpeedRight;
  BackRightSpeed = InsideCornerSpeedRight;
}

void DriveContinue() {
  FrontLeftGo(FrontLeftSpeed);
  BackLeftGo(BackLeftSpeed);
  FrontRightGo(FrontRightSpeed);
  BackRightGo(BackRightSpeed);
}

void DriveForwardDefault() {
  //CornerCount = 0;
  DriveState = DriveStraight;
  FrontLeftSpeed = DefaultSpeed; 
  BackLeftSpeed = DefaultSpeed;   
  FrontRightSpeed = DefaultSpeed;
  BackRightSpeed = DefaultSpeed; 
  FrontLeftGo(DefaultSpeed);
  BackLeftGo(DefaultSpeed);
  FrontRightGo(DefaultSpeed);
  BackRightGo(DefaultSpeed);
}

int SensorDetect(int SensorNumber) {
  return(!digitalRead(SensorNumber));
}

void FrontRightGo(int Speed) {
  RightCtrl.setSpeed(FrontMotor, Speed);
}

void BackRightGo(int Speed) {
  RightCtrl.setSpeed(BackMotor, Speed);
}

void FrontLeftGo(int Speed) {
  LeftCtrl.setSpeed(FrontMotor, -Speed);
}

void BackLeftGo(int Speed) {
  LeftCtrl.setSpeed(BackMotor, -Speed);
}

void ButtonsHandle() {
  AnalogButton1 = analogRead(A3);
  AnalogButton2 = analogRead(A2);

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
    if (PreviousButton1 == HIGH) { // Test for initial button press detection
      TriggerButton1 = TRUE;
    }
    PreviousButton1 = LOW;
  }
  else{
    PreviousButton1 = HIGH;
  }

  if(DigitalButton2 == LOW) { // The switch is pressed
    if (PreviousButton2 == HIGH) { // Test for initial button press detection
      TriggerButton2 = TRUE;
    }
    PreviousButton2 = LOW;
  }
  else{
    PreviousButton2 = HIGH;
  }
}

      /*
      if ( SensorDetect(FrontCornerOuter) ||
           SensorDetect(FrontCornerInner) ||
           SensorDetect(FrontSideInner) ) 
      { // The front edge is still over a line
        CornerLeft();
        DriveContinue();
      }
      else if ( SensorDetect(FrontSideOuter) ) { 
        DriveForwardDefault(); // We are correctly on the line
      }
      else { // We are out off the line
        PivotRight();
        DriveContinue();
      }
      break;
      */ 

        /*
        if (SensorDetect(BackSideInner)) { // We have veered to the right, over the line
          if (BackRightSpeed < MaxFwdSpeed) {
            BackRightSpeed += VeerCorrection;
          }
          if (BackLeftSpeed > MinFwdSpeed) {
            BackLeftSpeed -= VeerCorrection;
          }
          Serial.println("Veered to the right");
        }
        else if (!SensorDetect(BackSideOuter)) { // We have veered to the left, off the line
          if (BackRightSpeed > MinFwdSpeed) {
            BackRightSpeed -= VeerCorrection;
          }
          if (BackLeftSpeed < MaxFwdSpeed) {
            BackLeftSpeed += VeerCorrection;
          }
          Serial.println("Veered to the left");
        }
        */

