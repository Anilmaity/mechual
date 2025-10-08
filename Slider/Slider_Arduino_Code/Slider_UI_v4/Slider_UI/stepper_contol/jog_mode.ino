
// Handle jogging mode with custom accel/decel
void handleJogging(float rpm, char direction) {
    static unsigned long lastUpdate = 0;
    unsigned long now = millis();

    long pos = getStepsFromRight();

    // if ((pos < 80 || totalStepsBetweenLimits - pos < 80 ) && calibrated){
    //   rpm = CALIBRATION_SPEED;
    // }

      
        if (direction == 'R' && !isRightLimitTriggered()  ) {

        stepper.moveTo(rightLimitPos+1);
        stepper.setAcceleration(DEFAULT_ACCELERATION);

        stepper.setMaxSpeed(rpm);
        stepper.run();        
        }





       if (direction == 'L' && !isLeftLimitTriggered() ) 
        {

        stepper.moveTo(leftLimitPos);
        stepper.setAcceleration(DEFAULT_ACCELERATION);
        stepper.setMaxSpeed(rpm);
        stepper.run();  

        }

        else if  (direction == 'I'){

            stepper.setMaxSpeed(stepper.speed());
            stepper.stop();
            stepper.run(); 
        }
      

        stepper.run();  


    
}


int getRotationDirection() {
  long currentPos = stepper.currentPosition();
  long targetPos = stepper.targetPosition();

  if (targetPos > currentPos) {
    return 1; // Clockwise
  } else if (targetPos < currentPos) {
    return -1; // Counterclockwise
  } else {
    return 0; // Stopped or no movement
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







