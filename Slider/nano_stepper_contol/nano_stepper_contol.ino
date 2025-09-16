#include <TMCStepper.h>
#include <AccelStepper.h>
#include <SPI.h>

#define EN_PIN    4
#define DIR_PIN   3
#define STEP_PIN  2
#define CS_PIN    10

#define R_SENSE 0.075f

const uint16_t FULL_STEPS_PER_REV = 200;
const uint16_t MICROSTEPS = 2;
const long REV_STEPS = FULL_STEPS_PER_REV * MICROSTEPS;

const float TARGET_RPM = 800.0;
const float TARGET_STEPS_PER_SEC = (TARGET_RPM * REV_STEPS) / 60.0;
const float ACCELERATION = TARGET_STEPS_PER_SEC * 8.0;

TMC5160Stepper driver = TMC5160Stepper(CS_PIN, R_SENSE);
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

bool paused = false;
unsigned long pauseStart = 0;
const unsigned long PAUSE_DURATION = 1000; // 1 second

long targetPos = REV_STEPS * 20L; // current target
bool dir = false; // current direction

void setup() {
  SPI.begin();
  Serial.begin(1000000);
  while (!Serial) {}

  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

  // pinMode(A0, INPUT_PULLUP); // limit switch 1
  // pinMode(A1, INPUT_PULLUP); // limit switch 2

  driver.begin();
  driver.rms_current(2500);
  driver.en_pwm_mode(0);
  driver.pwm_autoscale(0);
  driver.pwm_ampl(255);
  driver.pwm_symmetric(true);
  driver.microsteps(MICROSTEPS);

  stepper.setMaxSpeed(TARGET_STEPS_PER_SEC);
  stepper.setAcceleration(ACCELERATION);
  stepper.setEnablePin(EN_PIN);
  stepper.setPinsInverted(false, false, true);
  stepper.enableOutputs();

  stepper.moveTo(targetPos);
}

void loop() {
  // Check limit switches
  if ((digitalRead(A0) == HIGH || digitalRead(A1) == HIGH) && !paused) {
    paused = true;
    pauseStart = millis();
    Serial.println("Limit switch triggered! Pausing 1 second...");

    // Do NOT disable outputs; just stop smooth motion
    // Save remaining distance
    targetPos = stepper.currentPosition() + stepper.distanceToGo();
    stepper.stop();
  }

  // Handle pause
  if (paused) {
    if (millis() - pauseStart >= PAUSE_DURATION) {
      paused = false;
      Serial.println("Resuming motion...");
      stepper.moveTo(targetPos); // resume motion smoothly with acceleration
    } else {
      stepper.run(); // continue calling run() to keep position updates correct
      return;
    }
  }

  stepper.run();

  // Command next move if finished
  if (stepper.distanceToGo() == 0 && !paused) {
    dir = !dir;
    if (dir) {
      Serial.println("Moving CW...");
      targetPos = stepper.currentPosition() - REV_STEPS * 20L;
    } else {
      Serial.println("Moving CCW...");
      targetPos = stepper.currentPosition() + REV_STEPS * 20L;
    }
    stepper.moveTo(targetPos);
  }
}
