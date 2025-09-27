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
const uint8_t DIR_PIN = 3;               // Direction pin
const uint8_t STEP_PIN = 2;              // Step pin
const uint8_t LIMIT_SWITCH_RIGHT = A0;   // Right limit switch (CW direction)
const uint8_t LIMIT_SWITCH_LEFT = A1;    // Left limit switch (CCW direction)

// Stepper motor configurations
const uint16_t FULL_STEPS_PER_REV = 200; // Steps per revolution for 1.8° motor
const uint16_t MICROSTEPS = 16;          // Microstepping (must match DM542 DIP switch setting, e.g., 16)
const long STEPS_PER_REV = FULL_STEPS_PER_REV * MICROSTEPS;

// Default operational settings
const float DEFAULT_RPM = 80.0;          // Default RPM
const float DEFAULT_STEPS_PER_SEC = (DEFAULT_RPM * STEPS_PER_REV) / 60.0; // Steps per second
const float DEFAULT_ACCELERATION = DEFAULT_RPM * 10.0;                    // Acceleration
const long LARGE_DISTANCE = 10000000L;   // Arbitrary large distance for limit seeking
const unsigned long TIMEOUT_MS = 60000;  // Timeout in milliseconds per phase

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
    bool triggered = digitalRead(LIMIT_SWITCH_RIGHT) == HIGH;
    return triggered;
}

// Check if left limit switch is triggered
bool isLeftLimitTriggered() {
    bool triggered = digitalRead(LIMIT_SWITCH_LEFT) == HIGH;
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
    Serial.print(stepper.targetPosition());
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
        Serial.println("Left limit hit, stopped.");
    } else if (currentSpeed < 0 && isRightLimitTriggered()) {
        stopMotor();
        Serial.println("Right limit hit, stopped.");
    }
}





// SECTION 9: Setup and Loop
void setup() {
    initializeHardware();
    configureStepper();
    
    // Print initial states
    Serial.println("Initial Limit Switch States:");
    Serial.print("Right: ");
    Serial.println(isRightLimitTriggered() ? "Triggered" : "Not triggered");
    Serial.print("Left: ");
    Serial.println(isLeftLimitTriggered() ? "Triggered" : "Not triggered");
    
    Serial.println("System ready. Send commands via serial.");
    Serial.print("Note: Ensure DM542 microsteps match MICROSTEPS=");
    Serial.println(MICROSTEPS);
}

void loop() {
    safetyCheck();
    readlogs();

    if (modeCurrent == CALIBRATING) {
      if (calCommand == "C")
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
        handlePositioning( 1000);
    } else if (modeCurrent == JOGGING) {
        handleJogging(80 , "L");
    }

    // Periodic status print during calibration
    static unsigned long lastPrint = 0;
    if (modeCurrent == CALIBRATING && millis() - lastPrint >= 1000) {
        printStatus();
        lastPrint = millis();
    }
}