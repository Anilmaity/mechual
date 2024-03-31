void sterring_setup() {
  pinMode(S_SEN, INPUT);

  pinMode(S_DIR, OUTPUT);

  sterring_pwm->period_us(100); // Set PWM period to 1ms (1kHz)
  sterring_pwm->pulsewidth_us(0); // Initialize PWM pulse width to 0

  
}


void sterring_loop() {
  sensorValue = analogRead(S_SEN);
  error_sterring = 0.1 * (sterring_value - sensorValue) + 0.9 * error_sterring;

  if (abs(error_sterring) > 6) {
    if (error_sterring > 0) {
      digitalWrite(S_DIR, HIGH);
    } 
    else {
      digitalWrite(S_DIR, LOW);
    }

    if(abs(error_sterring) < 40 ){
        sterring_pwm ->pulsewidth_us(100);
        delayMicroseconds(abs(error_sterring)*100);


        
      }

      else
      {
        sterring_pwm ->pulsewidth_us(100);
        delayMicroseconds(1000);
      }

  } else {
    sterring_pwm ->pulsewidth_us(0);
  }
}



void throttle_setup() {

   pwm->period_us(200); // Set PWM period to 1ms (1kHz)
   pwm->pulsewidth_us(0); // Initialize PWM pulse width to 0


  // pinMode(throttle_pin, OUTPUT);
}

void throttling() {

    if (input_throttle >= int(initial_throttle/2)) {
    digitalWrite(Reverse_pin, HIGH);
  } else if(input_throttle <= int(-initial_throttle_backward/2)) {
    digitalWrite(Reverse_pin, LOW);
  }


  if (throttle > 0) {
    if (throttle > input_throttle ) {
      if(input_throttle < initial_throttle){
        input_throttle = initial_throttle;
      }
      else if(input_throttle < 0){
        input_throttle =  0 ;
      }
      else{
       input_throttle = input_throttle + 0.004;
      }

    } else if (throttle <= input_throttle) {
      input_throttle = input_throttle - 0.008;
    } else {
      input_throttle = 0;
    }

  } else if(throttle < 0){
    if(input_throttle > -initial_throttle_backward){
        input_throttle = -initial_throttle_backward;
      }
    else if (throttle < input_throttle) {
      if(input_throttle > 0){
        input_throttle = 0 ;
      }
      else{
       input_throttle = input_throttle - 0.008; // 
      }

    } else if (throttle >= input_throttle) {
      input_throttle = input_throttle + 0.004;
    } 
  }
  else{
    if(input_throttle > 0){
      input_throttle = input_throttle - 0.03;
    }
    else if(input_throttle < 0)
    {
      input_throttle = input_throttle + 0.03;
    }
    else{
      input_throttle = 0;
    }
  }
  

  if (abs(throttle) > initial_throttle_backward) {

    pwm->pulsewidth_us(abs(int(input_throttle)));
    // analogWrite(throttle_pin, abs(input_throttle));

  } else {
    
    pwm->pulsewidth_us(abs(int(input_throttle)));

  }
}
