long int start_position = 0;

void handleABShuttle(int A, int B, int speedAtoB, int speedBtoA, int loops) {
    static int loopsDone = 0;           // Tracks completed A-to-B-to-A cycles
    static bool goingToB = false;       // False: move to A; True: move to B
    static bool firstRun = true;        // Ensures first move is to A
  
    int compare_loop = loops;
    // Ensure A < B for consistency

    // if (B > A) {
    //   if(rightLimitPos + start_position < rightLimitPos + A)
    //   {
    //     int compare_loop = loops -1;
    //   }

    // }

    // else if (A > B)
    // {
    // if(rightLimitPos + start_position >  rightLimitPos + A)
    //   {
    //     int compare_loop = loops -1;
    //   }

    // }






    // Set target position (A or B, adjusted by rightLimitPos)
    long target = goingToB ? (rightLimitPos + B) : (rightLimitPos + A);

    // Set speed based on direction
    stepper.setMaxSpeed(goingToB ? speedAtoB : speedBtoA);

    // Move to target and run stepper
    stepper.moveTo(target);
    stepper.run();

    // Check if target reached
    if (stepper.distanceToGo() == 0) {
        Serial.print("Arrived at point ");
        Serial.println(goingToB ? "B" : "A");

        // After first move to A, switch to B
        if (firstRun && !goingToB) {
        if (stepper.currentPosition() == (rightLimitPos + A)) {
                    firstRun = false;
                    goingToB = true;
                }
        } else {
            // Switch direction
            goingToB = !goingToB;

            // Count cycle after completing A-to-B-to-A
            if (goingToB) {
                loopsDone++;
                Serial.print("Completed loop ");
                Serial.print(loopsDone);
                Serial.print(" of ");
                Serial.println(loops);

                // Check if all loops are complete
                if (loopsDone >= compare_loop) {
                    modeCurrent = IDLE;
                    loopsDone = 0;
                    firstRun = true;
                    Serial.println("AB Shuttle complete.");
                }
            }
        }
    }
}