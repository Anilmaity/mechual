// StepperMotorControl.ino
// Modular and readable Arduino code for controlling a stepper motor with DM542 driver.
// Features: Calibration, Position Control, Jog Mode, Serial Communication.
// Converted from TMC5160 to DM542 (step/dir driver). No SPI or current control via software.
// Microsteps must be set on DM542 DIP switches. Adjust MICROSTEPS in code to match.
// Added: Global counter to track stepper position step count relative to limits (steps from right limit).
// Modified: Position commands (POS and REL) now reference steps from right limit.
// Modified: handleJogging() now takes speed in RPM and direction (R, L, I for right, left, idle).
// Created handlePositioning() and handleJogging() for modularity.

// SECTION 1: Includes and Definitions
#include <AccelStepper.h>

// Pin definitions
const uint8_t EN_PIN = 4;                // Enable pin
const uint8_t DIR_PIN = 2;               // Direction pin
const uint8_t STEP_PIN = 3;              // Step pin
const uint8_t LIMIT_SWITCH_RIGHT = A1;   // Right limit switch (CW direction)
const uint8_t LIMIT_SWITCH_LEFT = A0;    // Left limit switch (CCW direction)
const uint8_t BATTERY_PIN = A2;          // Battery voltage analog pin

// 124 18.5 rpm 19.10 
// Stepper motor configurations
const uint16_t FULL_STEPS_PER_REV = 200; // Steps per revolution for 1.8° motor

const uint16_t MICROSTEPS = 4;          // Microstepping (must match DM542 DIP switch setting, e.g., 16)
const long STEPS_PER_REV = FULL_STEPS_PER_REV * MICROSTEPS;

// Default operational settings
const float DEFAULT_RPM = 200.0;          // Default RPM
const float CALIBRATION_SPEED = 200.0;          // Default RPM
const float CurrentSpeed = 200;

const float DEFAULT_STEPS_PER_SEC = (DEFAULT_RPM * STEPS_PER_REV) / 60.0; // Steps per second
const float DEFAULT_ACCELERATION = 100;                    // Acceleration
const long LARGE_DISTANCE = 100000L;   // Arbitrary large distance for limit seeking
const unsigned long TIMEOUT_MS = 100000;  // Timeout in milliseconds per phase
const float BATTERY_DIVIDER_RATIO = 5.72; // Voltage divider ratio (adjust based on hardware, e.g., for 28.6V max)


// Emergency state
bool emergency = false;



// SECTION 2: Global Objects and States
// Stepper object
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Operational modes
enum Mode { IDLE, CALIBRATING, POSITIONING, JOGGING ,AB_SHUTTLE };
Mode modeCurrent = IDLE;

// Global variables for parsed data
String currentMode = "";  // "JM", "PM", "AB", "CA"
int jogSpeed = 0;
char jogCommand = ' ';  // 'R', 'L', 'I'
int position = 0;
int pointA = 0;
int pointB = 0;
int shuttleTime = 0;
int shuttleLoops = 0;
char calCommand = ' ';  // 'C', 'R', 'L'




// Calibration state structure
struct CalibrationState {
    int phase = 1;                       // Calibration phase (1-4)
    long startPosition = 0;              // Starting position
    long rightSteps = 0;                 // Steps to right limit (unused for total)
    long leftSteps = 0;                  // Steps from right to left limit (total steps)
    long rightPosition = 0;              // Right limit position
    long leftPosition = 0;               // Left limit position
    long centerPosition = 0;             // Calculated center position
    unsigned long phaseStartTime = 0;    // Start time for current phase
} calState;

// Global limit positions (adjusted after calibration) and total steps
long rightLimitPos = 0;
long leftLimitPos = 0;
long totalStepsBetweenLimits = 0;

// SECTION 3: Hardware Initialization Functions
// Configure the AccelStepper library
void configureStepper() {
    stepper.setMaxSpeed(DEFAULT_STEPS_PER_SEC);
    stepper.setAcceleration(DEFAULT_ACCELERATION);
    stepper.setEnablePin(EN_PIN);
    stepper.setPinsInverted(false, false, true); // Invert enable pin (LOW = enabled)
    stepper.enableOutputs();
}

// Initialize hardware pins and peripherals
void initializeHardware() {
    Serial.begin(1000000);               // High baud rate for serial
    while (!Serial) {}                   // Wait for serial to initialize
    
    pinMode(LIMIT_SWITCH_RIGHT, INPUT_PULLUP);
    pinMode(LIMIT_SWITCH_LEFT, INPUT_PULLUP);
}

// SECTION 4: Limit Switch Functions
// Check if right limit switch is triggered
bool isRightLimitTriggered() {
    bool triggered = digitalRead(LIMIT_SWITCH_RIGHT) == LOW;
    return triggered;
}

// Check if left limit switch is triggered
bool isLeftLimitTriggered() {
    bool triggered = digitalRead(LIMIT_SWITCH_LEFT) == LOW;
    return triggered;
}

// SECTION 5: Status and Debugging Functions
// Get steps from right limit (global counter)
long getStepsFromRight() {
    return stepper.currentPosition() - rightLimitPos;
}

// Get steps from left limit
long getStepsFromLeft() {
    return leftLimitPos - stepper.currentPosition();
}

// Print current status to serial
void printStatus() {
    Serial.print("Phase: ");
    Serial.print(calState.phase);
    Serial.print(", Current Position: ");
    Serial.print(stepper.currentPosition());
    Serial.print(", Target Position: ");
    Serial.print(stepper.distanceToGo());
    Serial.print(", Moving: ");
    Serial.println(stepper.isRunning() ? "Yes" : "No");
    
    Serial.print("Mode: ");
    switch (modeCurrent) {
        case IDLE: Serial.println("IDLE"); break;
        case CALIBRATING: Serial.println("CALIBRATING"); break;
        case POSITIONING: Serial.println("POSITIONING"); break;
        case JOGGING: Serial.println("JOGGING"); break;
    }
    
    if (totalStepsBetweenLimits > 0) {
        Serial.print("Steps from Right Limit: ");
        Serial.println(getStepsFromRight());
        Serial.print("Steps from Left Limit: ");
        Serial.println(getStepsFromLeft());
        Serial.print("Total Steps between Limits: ");
        Serial.println(totalStepsBetweenLimits);
    }
}

// SECTION 7: Motor Control Functions
// Stop the motor and reset mode to IDLE
void stopMotor() {
    Serial.println("Stopping motor.");
    if (modeCurrent == JOGGING) {
        stepper.setSpeed(0);
    } else {
        stepper.stop();
    }
    modeCurrent = IDLE;
}

// Check limits and stop if triggered
void safetyCheck() {
    if (modeCurrent == IDLE) return;
    
    float currentSpeed = stepper.speed();
    if (currentSpeed > 0 && isLeftLimitTriggered()) {
        stopMotor();
        sendlogs();

        Serial.println("Left limit hit, stopped.");
    } else if (currentSpeed < 0 && isRightLimitTriggered()) {
        stopMotor();
        sendlogs();
        Serial.println("Right limit hit, stopped.");
    }
}


// Get battery voltage
float getBatteryVoltage() {
    float adc = analogRead(BATTERY_PIN);
    float volt = adc * (5.0 / 1023.0);
    return volt * BATTERY_DIVIDER_RATIO;  // Adjust ratio for actual voltage
}

void sendlogs() {
    long pos = getStepsFromRight();
    float bat = getBatteryVoltage();
    int e = emergency ? 1 : 0;
    int lr = isRightLimitTriggered() ? 1 : 0;
    int ll = isLeftLimitTriggered() ? 1 : 0;

    Serial.print("P ");
    Serial.print(pos);
    Serial.print(" , B ");
    Serial.print(bat, 1);
    Serial.print(" , E ");
    Serial.print(e);
    Serial.print(" , LR ");
    Serial.print(lr);
    Serial.print(" , LL ");
    Serial.print(ll);
    Serial.print(" , TS ");
    Serial.print(totalStepsBetweenLimits);
    Serial.println(" .");

}

void sendshortlogs() {
    long pos = getStepsFromRight();


    Serial.print("P ");
    Serial.print(pos);
    Serial.println(" .");

}


// SECTION 9: Setup and Loop
void setup() {
    initializeHardware();
    configureStepper();

}

void loop() {
    readlogs();

    if (modeCurrent == CALIBRATING) {

      if (String(calCommand) == "C")
      {
        stepper.run();
        handleCalibration();
        if (calState.phase == 4) {
            stepper.setCurrentPosition(0); // Reset center to 0
            // Adjust limit positions relative to new zero (center)
            rightLimitPos -= calState.centerPosition;
            leftLimitPos -= calState.centerPosition;
            Serial.println("Calibration done, center position reset to 0.");
            modeCurrent = IDLE;
        }
      }
      
    } else if (modeCurrent == POSITIONING) {
         safetyCheck();
        handlePositioning(position);

    } else if (modeCurrent == JOGGING) {
         safetyCheck();

      handleJogging(jogSpeed , jogCommand);
    }
    else if (modeCurrent == AB_SHUTTLE) {
         safetyCheck();
        handleABShuttle();
    }

    // Periodic status print during calibration
    static unsigned long lastPrint = 0;
    // if (modeCurrent == CALIBRATING && millis() - lastPrint >= 1000) {
    //     printStatus();
    //     lastPrint = millis();
    // }

    if (millis() - lastPrint >= 100) {
        sendshortlogs();
        lastPrint = millis();
    }


}


