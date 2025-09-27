void handlePositioning(long stepsFromRightLimit) {
    long target = rightLimitPos + stepsFromRightLimit;
    stepper.moveTo(target);
    stepper.run();
    if (stepper.distanceToGo() == 0) {
        Serial.println("Position reached.");
        Serial.print("Current steps from right limit: ");
        Serial.println(getStepsFromRight());
        currentMode = IDLE;
    }
}