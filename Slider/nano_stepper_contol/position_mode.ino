void handlePositioning(long stepsFromRightLimit) {
    static float currentSpeed = 0.0;    // Current applied speed (steps/sec)
    static const float accelRate = 7.0; // Acceleration/deceleration rate (steps/sec^2)
    static const long decelDistance = 700; // Fixed distance to start decelerating (steps)

    long target = rightLimitPos + stepsFromRightLimit;
    float targetSpeed = 0.0;

    // Check limit switches to determine if movement is safe
    if ((stepsFromRightLimit < 0 && isLeftLimitTriggered()) || 
        (stepsFromRightLimit > 0 && isRightLimitTriggered())) {
        targetSpeed = 0.0; // Stop if limit is triggered
        currentSpeed = 0.0;
        stepper.setSpeed(0.0);
        currentMode = IDLE;
        Serial.println("Limit switch triggered. Stopping.");
        return;
    }

    // Calculate remaining distance
    long distanceToGo = abs(stepper.distanceToGo());
    
    // Determine target speed with deceleration
    if (distanceToGo > decelDistance) {
        targetSpeed = jogSpeed; // Accelerate to full speed
    } else {
        // Linear deceleration based on remaining distance
        targetSpeed = jogSpeed * (float)distanceToGo / decelDistance;
        if (targetSpeed > jogSpeed) targetSpeed = jogSpeed; // Cap at jogSpeed
    }

    // Apply acceleration/deceleration ramp
    if (currentSpeed < targetSpeed) {
        currentSpeed += accelRate * 0.005; // Adjust for 5ms loop
        if (currentSpeed > targetSpeed) currentSpeed = targetSpeed;
    } else if (currentSpeed > targetSpeed) {
        currentSpeed -= accelRate * 0.005; // Adjust for 5ms loop
        if (currentSpeed < targetSpeed) currentSpeed = targetSpeed;
    }

    // Drive stepper to target position
    stepper.setSpeed(stepsFromRightLimit < 0 ? -currentSpeed : currentSpeed);
    stepper.moveTo(target);
    stepper.runSpeedToPosition();

    
    // Check if target reached
    if (distanceToGo == 0) {
        currentSpeed = 0.0; // Reset speed
        stepper.setMaxSpeed(DEFAULT_STEPS_PER_SEC);
        currentMode = IDLE;
        // Serial.println("Position reached.");
        // Serial.print("Current steps from right limit: ");
        // Serial.println(stepsFromRightLimit);
    }
}

// void handlePositioning(long stepsFromRightLimit) {
//     static float currentSpeed = 0.0;    // Current applied speed (steps/sec)
//     static const float accelRate = 0.10;// Acceleration rate (steps/sec^2)

//     long target = rightLimitPos + stepsFromRightLimit;
//     float targetSpeed = 0.0;

//     // Check limit switches to determine if movement is safe
//     if ((stepsFromRightLimit < 0 && isRightLimitTriggered()) || 
//         (stepsFromRightLimit > 0 && isLeftLimitTriggered())) {
//         targetSpeed = 0.0; // Stop if limit is triggered
//         currentSpeed = 0.0;
//         stepper.setSpeed(0.0);
//         currentMode = IDLE;
//         //Serial.println("Limit switch triggered. Stopping.");
//         return;
//     } else {
//         targetSpeed = jogSpeed; // Target speed for movement
//     }

//     // Apply acceleration ramp
//     if (currentSpeed < targetSpeed) {
//         currentSpeed += accelRate;
//         if (currentSpeed > targetSpeed) currentSpeed = targetSpeed;
//     } else if (currentSpeed > targetSpeed) {
//         currentSpeed -= accelRate;
//         if (currentSpeed < targetSpeed) currentSpeed = targetSpeed;
//     }

//     // Drive stepper to target position
//     stepper.setSpeed(currentSpeed);
//     stepper.moveTo(target);
//     stepper.runSpeedToPosition();

//     // Check if target reached
//     if (stepper.distanceToGo() == 0) {
//         currentSpeed = 0.0; // Reset speed
//         stepper.setMaxSpeed(DEFAULT_STEPS_PER_SEC);
//         currentMode = IDLE;
//        // Serial.println("Position reached.");
//         //Serial.print("Current steps from right limit: ");
//        // Serial.println(stepsFromRightLimit);
//     }
// }