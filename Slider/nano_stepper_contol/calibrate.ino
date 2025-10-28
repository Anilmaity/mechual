/*=====================================================================
   CALIBRATION – Core‑1 (motor) only
  =====================================================================*/
const long SAFETY_STEPS = 100;               // stop 100 steps before the physical limit

void initiateRecalibration()
{
    // ----- reset everything that belongs to calibration -----
    portENTER_CRITICAL(&sharedMutex);
    shared.calState.phase            = 1;
    shared.calState.phaseStartTime   = millis();
    shared.calState.rightPosition    = 0;
    shared.calState.leftPosition     = 0;
    shared.calState.centerPosition   = 0;
    shared.calState.rightSteps       = 0;
    shared.calState.leftSteps        = 0;
    shared.calibrated                = false;
    shared.calCommand                = ' ';          // clear command flag
    portEXIT_CRITICAL(&sharedMutex);

    Serial.println("Recalibration initiated.");
}

/*--------------------------------------------------------------------
   handleCalibration() – called every loop from Core‑1
   (the only place where stepper.run() is executed)
  --------------------------------------------------------------------*/
void handleCalibration()
{
    // ---- 1. Timeout check (shared data) ----
    {
        unsigned long now;
        int phase;
        portENTER_CRITICAL(&sharedMutex);
        now   = millis();
        phase = shared.calState.phase;
        portEXIT_CRITICAL(&sharedMutex);

        if (phase < 4 && (now - shared.calState.phaseStartTime > TIMEOUT_MS)) {
            Serial.println("Error: Timeout in phase " + String(phase) +
                           ". Check motor and limit switches!");
            portENTER_CRITICAL(&sharedMutex);
            shared.calState.phase = 4;          // go straight to “finished”
            portEXIT_CRITICAL(&sharedMutex);
            stopMotor();
            return;
        }
    }

    // ---- 2. Phase handling (all stepper calls are local) ----
    int phase;
    portENTER_CRITICAL(&sharedMutex);
    phase = shared.calState.phase;
    portEXIT_CRITICAL(&sharedMutex);

    if (phase == 1) {                     // ---- PHASE 1 : move CW to right limit ----
        stepper.moveTo(-LARGE_DISTANCE);   // large negative = CW
        stepper.setMaxSpeed(-CALIBRATION_SPEED);   // negative speed = CW
        stepper.run();

        if (isRightLimitTriggered()) {
            stepper.stop();                // immediate stop
            long curPos = stepper.currentPosition();

            portENTER_CRITICAL(&sharedMutex);
            shared.calState.rightPosition = curPos;
            shared.calState.rightSteps    = -curPos;               // steps from origin
            shared.calState.phase         = 2;
            shared.calState.phaseStartTime = millis();
            portEXIT_CRITICAL(&sharedMutex);

            Serial.print("Right limit hit! Steps to right: ");
            Serial.println(shared.calState.rightSteps);
            Serial.println("Switching to phase 2: Moving CCW to left limit...");

            // start moving toward left limit
            stepper.moveTo(LARGE_DISTANCE);
            return;
        }

    } else if (phase == 2) {               // ---- PHASE 2 : move CCW to left limit ----
        stepper.setMaxSpeed(CALIBRATION_SPEED);   // positive speed = CCW
        stepper.run();

        if (isLeftLimitTriggered()) {
            stepper.stop();
            long curPos = stepper.currentPosition();

            portENTER_CRITICAL(&sharedMutex);
            shared.calState.leftPosition = curPos;
            shared.calState.leftSteps    = curPos - shared.calState.rightPosition;
            shared.calState.centerPosition = (curPos + shared.calState.rightPosition) / 2;

            // ---- set global limits with safety margin ----
            rightLimitPos = shared.calState.rightPosition + SAFETY_STEPS;
            leftLimitPos  = shared.calState.leftPosition  - SAFETY_STEPS;
            totalStepsBetweenLimits = leftLimitPos - rightLimitPos;

            shared.calState.phase         = 3;
            shared.calState.phaseStartTime = millis();
            portEXIT_CRITICAL(&sharedMutex);

            Serial.print("Left limit hit! Steps from right to left: ");
            Serial.println(shared.calState.leftSteps);
            Serial.print("Center position calculated: ");
            Serial.println(shared.calState.centerPosition);

            // start moving to centre
            stepper.moveTo(shared.calState.centerPosition);
            return;
        }

    } else if (phase == 3) {               // ---- PHASE 3 : move to centre ----
        stepper.setMaxSpeed(-CALIBRATION_SPEED);   // direction does not matter – we use moveTo()
        stepper.run();

        if (stepper.distanceToGo() == 0) {
            // centre reached → calibration finished
            portENTER_CRITICAL(&sharedMutex);
            shared.calibrated = true;
            shared.calState.phase = 4;
            portEXIT_CRITICAL(&sharedMutex);

            sendlogs();                         // one‑time report
            Serial.println("Calibration complete. At centre position.");
        }
    }
    // phase 4 = idle – nothing to do
}