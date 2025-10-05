long int start_position = 0;
int loopsDone  = 0;
bool goingToB = false;
bool reachedA = false;
long int target = 0
int movement_speed = 0;

void handleABShuttle(int A, int B, int speedAtoB, int speedBtoA, int loops) {

    if (reachedA == false){
        target = A;
        movement_speed = speedBtoA;
    }
    else if (goingToB == true){
        target = B;
        movement_speed = speedAtoB;

    }
    else {
        target = A;
        movement_speed = speedBtoA;
    }


    // Set speed based on direction
    stepper.setMaxSpeed(movement_speed);

    // Move to target and run stepper
    stepper.moveTo(target);
    stepper.run();

    // Check if target reached
    if (stepper.distanceToGo() == 0) {
        if(reachedA == false){
            goingToB = true;
            reachedA = true;
        }
        else if(goingToB == true){
            goingToB = false;
        }
        else{
            goingToB = true;
            loopsDone++;
        }


        if (loopsDone >= loops) {
            modeCurrent = IDLE;
            resetAB();
            Serial.println("AB Shuttle complete.");
          }

    }
}


void resetAB(){
    start_position = 0;
    loopsDone  = 0;
    goingToB = false;
    reachedA = false;
    target = 0
    movement_speed = 0;


}