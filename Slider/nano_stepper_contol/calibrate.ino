#include <TMCStepper.h>
#include <AccelStepper.h>
#include <SPI.h>

// Pin definitions
const uint8_t EN_PIN = 4;
const uint8_t DIR_PIN = 3;
const uint8_t STEP_PIN = 2;
const uint8_t CS_PIN = 10;
const uint8_t LIMIT_SWITCH_RIGHT = A0; // Right limit switch (hit when moving CW)
const uint8_t LIMIT_SWITCH_LEFT = A1;  // Left limit switch (hit when moving CCW)

// Stepper motor configurations
const float R_SENSE = 0.075f;
const uint16_t FULL_STEPS_PER_REV = 200; // 1.8° motor
const uint16_t MICROSTEPS = 0;
const long STEPS_PER_REV = FULL_STEPS_PER_REV ;

// Calibration settings
const float CALIBRATION_RPM = 80.0; // Slower speed for calibration
const float CALIBRATION_STEPS_PER_SEC = (CALIBRATION_RPM * STEPS_PER_REV) / 60.0;
const float CALIBRATION_ACCELERATION = CALIBRATION_RPM * 10.0;
const long LARGE_DISTANCE = 10000000L; // Large enough to hit limits
const unsigned long TIMEOUT_MS = 60000; // 60 seconds timeout per phase

// Driver and stepper objects
TMC5160Stepper driver(CS_PIN, R_SENSE);
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Calibration state
struct CalibrationState {
    int phase = 1; // 1: Move CW to right limit, 2: Move CCW to left limit, 3: Move to center, 4: Done
    long startPosition = 0;
    long rightSteps = 0; // Steps to right limit from start
    long leftSteps = 0;  // Steps from right to left limit
    long rightPosition = 0;
    long leftPosition = 0;
    long centerPosition = 0;
    unsigned long phaseStartTime = 0;
} calState;

void configureDriver() {
    driver.begin();
    driver.rms_current(1800); // Adjusted to 1.7A, typical for NEMA 17; modify as needed
    driver.en_pwm_mode(1); // Use spreadCycle for more torque
    // driver.pwm_autoscale(0);
    driver.pwm_ampl(255);
    // driver.pwm_grad(25);     // Aggressive gradient for fast current scaling
    // driver.pwm_freq(4);      // High PWM frequency to minimize vibrations
    // driver.pwm_autoscale(true); // Enable auto-scaling for dynamic adjustment
    // driver.pwm_autograd(true);  // Enable auto-gradient for speed transitions
    // driver.pwm_reg(6);       // More responsive regulation loop
    // driver.pwm_lim(12);      // Higher PWM limit for increased torque
    // driver.freewheel(0);     // Normal operation (no freewheeling)


    driver.microsteps(MICROSTEPS);
    // Verify driver communication
    Serial.print("Driver GCONF: 0x");
    Serial.println(driver.GCONF(), HEX); // Should print a non-zero value if SPI is working
}

void configureStepper() {
    stepper.setMaxSpeed(CALIBRATION_STEPS_PER_SEC);
    stepper.setAcceleration(CALIBRATION_ACCELERATION);
    stepper.setEnablePin(EN_PIN);
    stepper.setPinsInverted(false, false, true); // Enable pin inverted (LOW = enabled)
    stepper.enableOutputs();
}

void initializeHardware() {
    SPI.begin();
    Serial.begin(1000000); // Standard baud rate for better compatibility
    while (!Serial) {}
    
    pinMode(CS_PIN, OUTPUT);
    digitalWrite(CS_PIN, HIGH);
    pinMode(LIMIT_SWITCH_RIGHT, INPUT_PULLUP);
    pinMode(LIMIT_SWITCH_LEFT, INPUT_PULLUP);
}

bool isRightLimitTriggered() {
    bool triggered = digitalRead(LIMIT_SWITCH_RIGHT) == HIGH;
    // Serial.print("Right Limit Switch: ");
    // Serial.println(triggered ? "HIGH (Triggered)" : "LOW");
    return triggered;
}

bool isLeftLimitTriggered() {
    bool triggered = digitalRead(LIMIT_SWITCH_LEFT) == HIGH;
    // Serial.print("Left Limit Switch: ");
    // Serial.println(triggered ? "HIGH (Triggered)" : "LOW");
    return triggered;
}

void printStatus() {
    Serial.print("Phase: ");
    Serial.print(calState.phase);
    Serial.print(", Current Position: ");
    Serial.print(stepper.currentPosition());
    Serial.print(", Target Position: ");
    Serial.print(stepper.targetPosition());
    Serial.print(", Moving: ");
    Serial.println(stepper.isRunning() ? "Yes" : "No");
}

void handleCalibration() {
    // Timeout check
    if (calState.phase < 4 && millis() - calState.phaseStartTime > TIMEOUT_MS) {
        Serial.println("Error: Timeout in phase " + String(calState.phase) + ". Check motor and limit switches!");
        calState.phase = 4; // Stop calibration
        stepper.stop();
        stepper.disableOutputs();
        return;
    }

    if (calState.phase == 1) { // Move CW to right limit
        if (isRightLimitTriggered()) {
            stepper.stop();
            calState.rightPosition = stepper.currentPosition();
            calState.rightSteps = -calState.rightPosition; // Since negative
            Serial.print("Right limit hit! Steps to right: ");
            Serial.println(calState.rightSteps);
            
            // Switch to phase 2: Move CCW to left limit
            calState.phase = 2;
            calState.phaseStartTime = millis();
            Serial.println("Switching to phase 2: Moving CCW to left limit...");
            if (isLeftLimitTriggered()) {
                Serial.println("Warning: Left limit switch already triggered at phase 2 start!");
                calState.leftPosition = stepper.currentPosition();
                calState.leftSteps = calState.leftPosition - calState.rightPosition;
                Serial.print("Left limit hit! Steps from right to left: ");
                Serial.println(calState.leftSteps);
                calState.centerPosition = (calState.leftPosition + calState.rightPosition) / 2;
                Serial.print("Center position calculated: ");
                Serial.println(calState.centerPosition);
                calState.phase = 3;
                calState.phaseStartTime = millis();
                stepper.moveTo(calState.centerPosition);
            } else {
                stepper.moveTo(LARGE_DISTANCE);
            }
            stepper.run();

            // printStatus();
            return;
        }
    } else if (calState.phase == 2) { // Move CCW to left limit
        if (isLeftLimitTriggered()) {
            stepper.stop();
            calState.leftPosition = stepper.currentPosition();
            calState.leftSteps = calState.leftPosition - calState.rightPosition;
            Serial.print("Left limit hit! Steps from right to left: ");
            Serial.println(calState.leftSteps);
            
            // Calculate center
            calState.centerPosition = (calState.leftPosition + calState.rightPosition) / 2;
            Serial.print("Center position calculated: ");
            Serial.println(calState.centerPosition);
            
            // Switch to phase 3: Move to center
            calState.phase = 3;
            calState.phaseStartTime = millis();
            stepper.moveTo(calState.centerPosition);
            stepper.run();

            // printStatus();
            return;
        }
    } else if (calState.phase == 3) { // Moving to center
        if (stepper.distanceToGo() == 0) {
            Serial.println("Calibration complete. At center position.");
            Serial.print("Final Right Steps: ");
            Serial.println(calState.rightSteps);
            Serial.print("Final Left Steps (from right): ");
            Serial.println(calState.leftSteps);
            Serial.print("Total Steps (right to left): ");
            Serial.println(calState.leftSteps + calState.rightSteps);
            calState.phase = 4; // Done
            // printStatus();
        }
    }
}

void setup() {
    initializeHardware();
    configureDriver();
    configureStepper();
    
    // Print initial limit switch states
    Serial.println("Initial Limit Switch States:");
    isRightLimitTriggered();
    isLeftLimitTriggered();
    
    // Start calibration: Set initial position to 0, move CW (negative)
    stepper.setCurrentPosition(0);
    calState.startPosition = 0;
    calState.phaseStartTime = millis();
    Serial.println("Starting calibration: Moving CW to right limit...");
    if (isRightLimitTriggered()) {
        Serial.println("Warning: Right limit switch already triggered at start!");
        calState.rightPosition = 0;
        calState.rightSteps = 0;
        calState.phase = 2;
        if (isLeftLimitTriggered()) {
            Serial.println("Warning: Left limit switch also triggered at start!");
            calState.leftPosition = 0;
            calState.leftSteps = 0;
            calState.centerPosition = 0;
            calState.phase = 3;
            stepper.moveTo(0);
        } else {
            stepper.moveTo(LARGE_DISTANCE);
        }
    } else {
        stepper.moveTo(-LARGE_DISTANCE);
    }
    // printStatus();
}

void loop() {
    stepper.run();
    handleCalibration();
    // Print status periodically (every 1 second)
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 1000) {
        // printStatus();
        lastPrint = millis();
    }
}