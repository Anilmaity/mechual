

void brake_setup() {
  brakeservo.attach(brake_signal_pin);  // attaches the servo on pin 9 to the servo object
}

void braking() {

  if (abs(Brake) < 85) {
    input_throttle = 0;
    digitalWrite(Brake_pin, LOW);
  } else {
    digitalWrite(Brake_pin, HIGH);
  }

  brakeservo.write(Brake);
}
