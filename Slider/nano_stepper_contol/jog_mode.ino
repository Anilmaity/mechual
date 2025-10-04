
// Handle jogging mode with custom accel/decel
void handleJogging(float rpm, char direction) {
    static unsigned long lastUpdate = 0;
    unsigned long now = millis();

    long pos = getStepsFromRight();

    if ((pos < 80 || totalStepsBetweenLimits - pos < 80 ) && calibrated){
      rpm = CALIBRATION_SPEED;
    }


        if (direction == 'R' && !isRightLimitTriggered() ) {

        stepper.moveTo(-LARGE_DISTANCE);
        stepper.setAcceleration(rpm);

        stepper.setMaxSpeed(-rpm);
        stepper.run();        
        }



        else if (direction == 'L' && !isLeftLimitTriggered() ) {

        stepper.moveTo(LARGE_DISTANCE);
        stepper.setAcceleration(rpm);

        stepper.setMaxSpeed(rpm);
        stepper.run();  

        }
        else{
        stepper.setMaxSpeed(int(rpm/1.5));
        stepper.stop();
        stepper.run();  

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







