void handlePositioning(long stepsFromRightLimit) {
    static float currentSpeed = 0.0;    // Current applied speed (steps/sec)
    static const float accelRate = 7.0; // Acceleration/deceleration rate (steps/sec^2)
    static const long decelDistance = 700; // Fixed distance to start decelerating (steps)

    long target = rightLimitPos + stepsFromRightLimit;
    float targetSpeed = 0.0;

    long pos = getStepsFromRight();

    // Check limit switches to determine if movement is safe
    if ((stepsFromRightLimit  - pos <  0 && isRightLimitTriggered()) || 
        (stepsFromRightLimit  - pos > 0 && isLeftLimitTriggered())) {
        targetSpeed = 0.0; // Stop if limit is triggered
        currentSpeed = 0.0;
        stepper.setSpeed(0.0);
        //currentMode = IDLE;
       // Serial.println("Limit switch triggered. Stopping.");
        return;
    }

   

    // Drive stepper to target position
    //stepper.setSpeed(stepsFromRightLimit < 0 ? -currentSpeed : currentSpeed);
    stepper.setMaxSpeed(jogSpeed);
    stepper.moveTo(target);
    stepper.run();

    
  
}
