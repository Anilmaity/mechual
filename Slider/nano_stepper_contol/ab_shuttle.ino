void handleABShuttle(int A, int B, int speedAtoB, int speedBtoA, int loops) {
    static int loopsDone = 0;           // Tracks completed A-to-B-to-A cycles
    static bool goingToB = false;       // False: move to A; True: move to B
    static bool firstRun = true;        // Ensures first move is to A
    static unsigned long arriveTime = 0;// Time of arrival at a point
    static bool waiting = false;        // True when waiting at a point

    // Ensure A < B for consistency
    if (A > B) {
        int temp = A;
        A = B;
        B = temp;
    }

    // Set target position (A or B, adjusted by rightLimitPos)
    long target = goingToB ? (rightLimitPos + B) : (rightLimitPos + A);

    if (!waiting) {
        // Set speed based on direction
        stepper.setMaxSpeed(goingToB ? speedAtoB : speedBtoA);

        // Move to target and run stepper
        stepper.moveTo(target);
        stepper.run();

        // Check if target reached
        if (stepper.distanceToGo() == 0) {
            arriveTime = millis();
            waiting = true;
            Serial.print("Arrived at point ");
            Serial.println(goingToB ? "B" : "A");

            // After first move to A, switch to B
            if (firstRun && !goingToB) {
                firstRun = false;
                goingToB = true;
            }
        }
    } else {
        // Check if wait time has elapsed
        if (millis() - arriveTime >= (shuttleTime * 1000UL)) {
            waiting = false;

            // Skip direction change on first run
            if (!firstRun) {
                goingToB = !goingToB;
            }

            // Count cycle after completing A-to-B-to-A
            if (!goingToB && !firstRun) {
                loopsDone++;
                Serial.print("Completed loop ");
                Serial.print(loopsDone);
                Serial.print(" of ");
                Serial.println(loops);

                // Check if all loops are complete
                if (loopsDone >= loops) {
                    modeCurrent = IDLE;
                    loopsDone = 0;
                    firstRun = true;
                    Serial.println("AB Shuttle complete.");
                }
            }
        }
    }
}