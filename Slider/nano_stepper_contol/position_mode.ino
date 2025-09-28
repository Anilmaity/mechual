void handlePositioning(long stepsFromRightLimit) {
    long target = rightLimitPos + stepsFromRightLimit;
    stepper.setMaxSpeed(200);

    stepper.moveTo(target);
    stepper.run();
    if (stepper.distanceToGo() == 0) {
        stepper.setMaxSpeed(DEFAULT_STEPS_PER_SEC);

        // Serial.println("Position reached.");
        // Serial.print("Current steps from right limit: ");
        // Serial.println(getStepsFromRight());
        currentMode = IDLE;
    }
}