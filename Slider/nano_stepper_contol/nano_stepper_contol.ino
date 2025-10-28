/*********************************************************************
 *  ESP32 Dual-Core Stepper + Serial
 *  Core 0 → Serial parsing + logging
 *  Core 1 → Motor control (AccelStepper)
 *********************************************************************/

#include <Arduino.h>
#include <AccelStepper.h>
#include <soc/rtc_wdt.h>

// ---------------------------------------------------------------
// 1. Pin definitions
// ---------------------------------------------------------------
const uint8_t EN_PIN              = 21;
const uint8_t DIR_PIN             = 19;
const uint8_t STEP_PIN            = 18;
const uint8_t LIMIT_SWITCH_RIGHT  = 4;
const uint8_t LIMIT_SWITCH_LEFT   = 2;
const uint8_t BATTERY_PIN         = 15;

// ---------------------------------------------------------------
// 2. Motor constants
// ---------------------------------------------------------------
const uint16_t FULL_STEPS_PER_REV = 1000;
const uint16_t MICROSTEPS         = 4;
const long     STEPS_PER_REV      = FULL_STEPS_PER_REV * MICROSTEPS;

const float    DEFAULT_RPM        = 10000.0;
const float    CALIBRATION_SPEED  = 1000.0;
const float    DEFAULT_STEPS_PER_SEC = (DEFAULT_RPM * STEPS_PER_REV) / 60.0;
const float    DEFAULT_ACCELERATION  = 3200;
const long     LARGE_DISTANCE     = 200000L;
const unsigned long TIMEOUT_MS    = 1000000;

// ---------------------------------------------------------------
// 3. Global Stepper
// ---------------------------------------------------------------
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// ---------------------------------------------------------------
// 4. Mode enum (ONLY ONE)
// ---------------------------------------------------------------
enum Mode { IDLE, CALIBRATING, POSITIONING, JOGGING, AB_SHUTTLE };
Mode modeCurrent = IDLE;

// ---------------------------------------------------------------
// 5. Shared data (Core 0 writes, Core 1 reads)
// ---------------------------------------------------------------
struct SharedCmd {
    String  currentMode   = "";
    int     jogSpeed      = 200;
    char    jogCommand    = ' ';
    int     position      = 0;
    int     pointA        = 0;
    int     pointB        = 0;
    int     speed1        = 0;
    int     speed2        = 0;
    int     shuttleLoops  = 0;
    char    calCommand    = ' ';

    bool    calibrated    = false;
    bool    emergency     = false;
    int     battery_value = 80;
    long    start_position = 0;
} shared;

portMUX_TYPE sharedMutex = portMUX_INITIALIZER_UNLOCKED;

// ---------------------------------------------------------------
// 6. Calibration state (only used by motor core)
// ---------------------------------------------------------------
struct CalibrationState {
    int   phase            = 1;
    long  startPosition    = 0;
    long  rightSteps       = 0;
    long  leftSteps        = 0;
    long  rightPosition    = 0;
    long  leftPosition     = 0;
    long  centerPosition   = 0;
    unsigned long phaseStartTime = 0;
} calState;

long rightLimitPos = 0;
long leftLimitPos  = 0;
long totalStepsBetweenLimits = 0;

// ---------------------------------------------------------------
// 7. Helper functions
// ---------------------------------------------------------------
long getStepsFromRight() { return stepper.currentPosition() - rightLimitPos; }
long getStepsFromLeft()  { return leftLimitPos - stepper.currentPosition(); }

bool isRightLimitTriggered() { return digitalRead(LIMIT_SWITCH_RIGHT) == LOW; }
bool isLeftLimitTriggered()  { return digitalRead(LIMIT_SWITCH_LEFT)  == LOW; }

void configureStepper() {
    stepper.setMaxSpeed(DEFAULT_STEPS_PER_SEC);
    stepper.setAcceleration(DEFAULT_ACCELERATION);
    stepper.setEnablePin(EN_PIN);
    stepper.setPinsInverted(false, false, true);
    stepper.enableOutputs();
    stepper.setMinPulseWidth(4);
}

void stopMotor() {
    stepper.setMaxSpeed(CALIBRATION_SPEED * 1.5);
    stepper.stop();
    stepper.run();  // execute deceleration
}


// ------------------------------------------------------------------
// 11. Arduino setup() – create Core-0 task
// ------------------------------------------------------------------
void setup() {
    Serial.begin(921600);
    while (!Serial) { delay(10); }

    pinMode(LIMIT_SWITCH_RIGHT, INPUT_PULLUP);
    pinMode(LIMIT_SWITCH_LEFT,  INPUT_PULLUP);
    configureStepper();

    rtc_wdt_protect_off();
    rtc_wdt_disable();

    xTaskCreatePinnedToCore(
        serialCommTask,
        "SerialComm",
        4096,
        NULL,
        1,
        NULL,
        0);  // Core 0
}

// ------------------------------------------------------------------
// 12. Motor loop – Core 1 (default Arduino loop)
// ------------------------------------------------------------------
void loop() {
    // ---- Safety ----
    bool limit = isRightLimitTriggered() || isLeftLimitTriggered();
    if (limit) {
        stopMotor();
        portENTER_CRITICAL(&sharedMutex); shared.emergency = true;  portEXIT_CRITICAL(&sharedMutex);
    } else {
        portENTER_CRITICAL(&sharedMutex); shared.emergency = false; portEXIT_CRITICAL(&sharedMutex);
    }

    // ---- Read mode ----
    String cmd;
    portENTER_CRITICAL(&sharedMutex);
    cmd = shared.currentMode;
    portEXIT_CRITICAL(&sharedMutex);



    // ---- Execute mode ----
    if (!limit) {
        switch (modeCurrent) {
            case JOGGING:
                { int s; char d;
                  portENTER_CRITICAL(&sharedMutex); s = shared.jogSpeed; d = shared.jogCommand; portEXIT_CRITICAL(&sharedMutex);
                  if (d == 'R' || d == 'L' || d == 'I') handleJogging(s, d); }
                break;
            case POSITIONING:
                { int t; portENTER_CRITICAL(&sharedMutex); t = shared.position; portEXIT_CRITICAL(&sharedMutex);
                  handlePositioning(t); }
                break;
            case AB_SHUTTLE:
                { int a,b,s1,s2,l;
                  portENTER_CRITICAL(&sharedMutex);
                  a=shared.pointA; b=shared.pointB; s1=shared.speed1; s2=shared.speed2; l=shared.shuttleLoops;
                  portEXIT_CRITICAL(&sharedMutex);
                  handleABShuttle(a,b,s1,s2,l); }
                break;
            case CALIBRATING:
                handleCalibration();
                break;
            default:
                stepper.setSpeed(0);
                break;
        }
    }

    stepper.run();
    vTaskDelay(1);
}

