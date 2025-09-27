void handleABShuttle() {
    static int loopsDone = 0;
    static bool goingToB = true;
    static unsigned long arriveTime = 0;
    static bool waiting = false;

    // Ensure A < B for consistency (swap if needed)
    if (pointA > pointB) {
        int temp = pointA;
        pointA = pointB;
        pointB = temp;
    }

    long target = goingToB ? (rightLimitPos + pointB) : (rightLimitPos + pointA);

    if (!waiting) {
        stepper.moveTo(target);
        stepper.run();
        if (stepper.distanceToGo() == 0) {
            arriveTime = millis();
            waiting = true;
            Serial.print("Arrived at point ");
            Serial.println(goingToB ? "B" : "A");
        }
    } else {
        if (millis() - arriveTime >= (shuttleTime * 1000UL)) {
            waiting = false;
            goingToB = !goingToB;
            if (!goingToB) { // Completed a full cycle (A to B to A)
                loopsDone++;
                Serial.print("Completed loop ");
                Serial.print(loopsDone);
                Serial.print(" of ");
                Serial.println(shuttleLoops);
                if (loopsDone >= shuttleLoops) {
                    modeCurrent = IDLE;
                    loopsDone = 0;
                    Serial.println("AB Shuttle complete.");
                }
            }
        }
    }
}