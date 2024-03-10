

void brake_setup() {
  brakeservo.attach(brake_signal_pin);  // attaches the servo on pin 9 to the servo object
}

void braking() {

  if (abs(Brake) > 200) {
    digitalWrite(Brake_pin, LOW);
  } else {
    digitalWrite(Brake_pin, HIGH);
  }

  brakeservo.write(Brake);
}
