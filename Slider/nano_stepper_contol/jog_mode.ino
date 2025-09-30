// Global or static variables
float currentSpeed = 0.0;           // current applied speed (steps/sec)
const float accelRate = 0.40;       // steps/sec^2 (acceleration rate)

// Handle jogging mode with custom accel/decel
void handleJogging(float rpm, char direction) {
    static unsigned long lastUpdate = 0;
    unsigned long now = millis();

    float targetSpeed = 0.0;

    if (direction == 'I') {
        targetSpeed = 0.0;   // stop
        currentMode = IDLE;
    }
    else {
        // Convert RPM → steps/sec
        // float stepsPerSec = (rpm * STEPS_PER_REV) / 60.0;
        float stepsPerSec = rpm;  // (you already use raw steps/sec)

        if (direction == 'L' && !isLeftLimitTriggered()) {
            targetSpeed = stepsPerSec;
        }
        else if (direction == 'R' && !isRightLimitTriggered()) {
            targetSpeed = -stepsPerSec;
        }
        else {
            targetSpeed = 0.0;  // hit limit → stop
        }
    }

    // Apply acceleration ramp
    if (currentSpeed < targetSpeed) {
        currentSpeed += accelRate;
        if (currentSpeed > targetSpeed) currentSpeed = targetSpeed;
    }
    else if (currentSpeed > targetSpeed) {
        currentSpeed -= accelRate;
        if (currentSpeed < targetSpeed) currentSpeed = targetSpeed;
    }

    // Drive stepper
    if (currentSpeed != 0.0) {
        stepper.setSpeed(currentSpeed);
        stepper.runSpeed();
    }
}



// // Handle jogging mode with speed in RPM and direction (R, L, I)
// void handleJogging(float rpm, char direction) {
//
//     if (direction == 'I') {
//         stepper.setSpeed(0);
//         currentMode = IDLE;
//         // Serial.println("Jogging stopped (Idle).");
//         return;
//     }
//
//     // float stepsPerSec = (rpm * STEPS_PER_REV) / 60.0;
//     float stepsPerSec = rpm;
//
//     float actualSpeed = (direction == 'L') ? stepsPerSec : -stepsPerSec; // L: CCW (positive), R: CW (negative)
//     if((actualSpeed > 0  && isRightLimitTriggered() == false ) || (actualSpeed < 0  && isLeftLimitTriggered() == false ) ){
//
//           stepper.setSpeed(actualSpeed);
//           stepper.runSpeed();
//
//
//     }
//
// }







