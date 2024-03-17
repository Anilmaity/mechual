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



void throttle_setup() {

  TCCR3B = TCCR3B & B11111000 | B00000010;  // for PWM frequency of 31372.55 Hz
  TCCR2B = TCCR2B & B11111000 | B00000001;  // for PWM frequency of 31372.55 Hz
  TCCR1B = TCCR1B & B11111000 | B00000001;  // for PWM frequency of 31372.55 Hz
  TCCR4B = TCCR4B & B11111000 | B00000001;  // for PWM frequency of 31372.55 Hz

  pinMode(throttle_pin, OUTPUT);
}

void throttling() {

    if (input_throttle >= int(initial_throttle/2)) {
    digitalWrite(Reverse_pin, HIGH);
  } else if(input_throttle <= int(-initial_throttle/2)) {
    digitalWrite(Reverse_pin, LOW);
  }


  if (throttle >= 0) {
    if (throttle > input_throttle) {
      if(input_throttle < 0){
        input_throttle =  0 ;
      }
      else{
       input_throttle = input_throttle + 5;
      }

    } else if (throttle <= input_throttle) {
      input_throttle = throttle;
    } else {
      input_throttle = 0;
    }

  } else {
    
    if (throttle < input_throttle) {
      if(input_throttle > 0){
        input_throttle = 0 ;
      }
      else{
       input_throttle = input_throttle - 2;
      }

    } else if (throttle >= input_throttle) {
      input_throttle = throttle;
    } 
  }

  if (abs(throttle) > initial_throttle) {
    analogWrite(throttle_pin, abs(input_throttle));

  } else {
    analogWrite(throttle_pin, abs(input_throttle));
  }
}
