void sterring_setup() {
  pinMode(S_SEN, INPUT);
  pinMode(S_DIR, OUTPUT);
  pinMode(S_PWM, OUTPUT);
}


void sterring_loop() {
  sensorValue = analogRead(S_SEN);
  error = 0.5 * (sterring_value - sensorValue) + 0.5 * error;

  if (abs(error) > 8) {
    if (error > 0) {
      digitalWrite(S_DIR, HIGH);
      digitalWrite(S_PWM, HIGH);
    } else {

      digitalWrite(S_DIR, LOW);
      digitalWrite(S_PWM, HIGH);
    }

  } else {
    digitalWrite(S_PWM, LOW);
  }
}

void drive_mode() {

  if (Drive_mode == 'N') {
    digitalWrite(Neutral_pin, HIGH);
    digitalWrite(Reverse_pin, HIGH);
  } else if (Drive_mode == 'F') {
    digitalWrite(Neutral_pin, LOW);
    digitalWrite(Reverse_pin, HIGH);
  } else {
    digitalWrite(Neutral_pin, LOW);
    digitalWrite(Reverse_pin, LOW);
  }
}


void throttle_setup() {

  TCCR3B = TCCR3B & B11111000 | B00000010; // for PWM frequency of 31372.55 Hz
  TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
  TCCR1B = TCCR1B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
  TCCR4B = TCCR4B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz

  // TCCR2B = TCCR2B & B11111000 | B00000010; // for PWM frequency of 3921.16 Hz

  pinMode(throttle_pin,OUTPUT);

  //Code for Available PWM frequency for D3 & D11:
  //TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz


// TCCR2B = TCCR2B & B11111000 | B00000011; // for PWM frequency of 980.39 Hz

// TCCR2B = TCCR2B & B11111000 | B00000100; // for PWM frequency of 490.20 Hz (The DEFAULT)

// TCCR2B = TCCR2B & B11111000 | B00000101; // for PWM frequency of 245.10 Hz

// TCCR2B = TCCR2B & B11111000 | B00000110; // for PWM frequency of 122.55 Hz

// TCCR2B = TCCR2B & B11111000 | B00000111; // for PWM frequency of 30.64 Hz
}

void throttling() {

  if (throttle > initial_throttle) {
    analogWrite(throttle_pin, throttle);
    
  } else {
    analogWrite(throttle_pin, 0);
  }
}
