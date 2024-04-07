

void brake_setup() {
  brakeservo.attach(brake_signal_pin);  // attaches the servo on pin 9 to the servo object
}

void braking() {

  if (abs(Brake) < 85) {
    input_throttle = 0;
    digitalWrite(Brake_pin, LOW); // Motor drive realy switching
  } else {
    digitalWrite(Brake_pin, HIGH);  // Motor drive realy switching
  }

  brakeservo.write(Brake); // give input to servo in degrees
}
