// Handle jogging mode with speed in RPM and direction (R, L, I)
void handleJogging(float rpm, char direction) {

    if (direction == 'I') {
        stepper.setSpeed(0);
        currentMode = IDLE;
        // Serial.println("Jogging stopped (Idle).");
        return;
    }
    
    // float stepsPerSec = (rpm * STEPS_PER_REV) / 60.0;
    float stepsPerSec = rpm;

    float actualSpeed = (direction == 'L') ? stepsPerSec : -stepsPerSec; // L: CCW (positive), R: CW (negative)
    if((actualSpeed > 0  && isLeftLimitTriggered() == false ) || (actualSpeed < 0  && isRightLimitTriggered() == false ) ){
      
      stepper.setSpeed(actualSpeed);
      stepper.runSpeed();

    }

}