
// SECTION 6: Calibration Functions
// Start the calibration process
void startCalibration() {
    calState.phase = 1;
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
}

// Handle calibration logic
void handleCalibration() {
    // Check for timeout
    if (calState.phase < 4 && millis() - calState.phaseStartTime > TIMEOUT_MS) {
        Serial.println("Error: Timeout in phase " + String(calState.phase) + ". Check motor and limit switches!");
        calState.phase = 4;
        stopMotor();
        return;
    }

    if (calState.phase == 1) { // Move CW to right limit
        if (isRightLimitTriggered()) {
            stepper.stop();
            calState.rightPosition = stepper.currentPosition();
            calState.rightSteps = -calState.rightPosition;
            Serial.print("Right limit hit! Steps to right: ");
            Serial.println(calState.rightSteps);
            
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
            return;
        }
    } else if (calState.phase == 2) { // Move CCW to left limit
        if (isLeftLimitTriggered()) {
            stepper.stop();
            calState.leftPosition = stepper.currentPosition();
            calState.leftSteps = calState.leftPosition - calState.rightPosition;
            Serial.print("Left limit hit! Steps from right to left: ");
            Serial.println(calState.leftSteps);
            
            calState.centerPosition = (calState.leftPosition + calState.rightPosition) / 2;
            Serial.print("Center position calculated: ");
            Serial.println(calState.centerPosition);
            
            // Set global limit positions and total steps
            rightLimitPos = calState.rightPosition;
            leftLimitPos = calState.leftPosition;
            totalStepsBetweenLimits = calState.leftSteps;
            
            calState.phase = 3;
            calState.phaseStartTime = millis();
            stepper.moveTo(calState.centerPosition);
            return;
        }
    } else if (calState.phase == 3) { // Move to center
        if (stepper.distanceToGo() == 0) {
            Serial.println("Calibration complete. At center position.");
            Serial.print("Final Right Steps (from initial): ");
            Serial.println(calState.rightSteps);
            Serial.print("Final Left Steps (from right): ");
            Serial.println(calState.leftSteps);
            Serial.print("Total Steps between Limits: ");
            Serial.println(totalStepsBetweenLimits);
            calState.phase = 4;
        }
    }
}
