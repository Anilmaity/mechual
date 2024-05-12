void sterring_setup() {
  pinMode(S_SEN, INPUT);

  pinMode(S_DIR, OUTPUT);
  // pinMode(S_PWM, OUTPUT);

  sterring_pwm->period_us(200);    // Set PWM period to 1ms (1kHz)
  sterring_pwm->pulsewidth_us(0);  // Initialize PWM pulse width to 0
}


void sterring_input() {
  sensorValue = analogRead(S_SEN);
  if(abs(sterring_value - sensorValue) > 30 && noise_count <= 2)
    {
      noise_count = noise_count +1;
      error_sterring = error_sterring;

    }
    else {
      noise_count = 0;
      error_sterring = 0.04 * (sterring_value - sensorValue) + 0.96 * error_sterring;
    
    }
}

void sterring_loop() {

  if (abs(error_sterring) > 4) {
    if (error_sterring > 0) {
      digitalWrite(S_DIR, HIGH);  // direction right or left
    } else {
      digitalWrite(S_DIR, LOW);
    }

    //sterring_pwm_speed +=  20;


    // if(sterring_pwm_speed <= 200)
    //   {
    //     sterring_pwm_speed = 200;
    //   }
    // digitalWrite(S_PWM, HIGH);
    sterring_pwm->pulsewidth_us(200);



    // analogWrite(S_PWM,sterring_pwm); // max speed for motor

    // if(abs(error_sterring) < 40 ){
    //     sterring_pwm ->pulsewidth_us(100);
    //     delayMicroseconds(abs(error_sterring)*100);
    //   }
    //   else
    //   {
    //     sterring_pwm ->pulsewidth_us(100);
    //     delayMicroseconds(1000);
    //   }

  } else {
    sterring_pwm->pulsewidth_us(0);
    // digitalWrite(S_PWM,LOW); // 0 pwm signal
    // sterring_pwm_speed = 100;
  }
}



void throttle_setup() {

  pwm->period_us(200);    // Set PWM period to 1ms (1kHz)
  pwm->pulsewidth_us(0);  // Initialize PWM pulse width to 0


  // pinMode(throttle_pin, OUTPUT);
}

void throttling() {

  if (input_throttle > int(initial_throttle / 2)) {
    digitalWrite(Reverse_pin, HIGH);
  } else if (input_throttle < int(-initial_throttle_backward / 2)) {
    digitalWrite(Reverse_pin, LOW);
  }


  if (throttle > 0) {
    if (throttle > input_throttle) {
      // if ( input_throttle < 0)
      // {
      //   input_throttle = input_throttle + speed_decrease_rate;

      // }
      // else

      if (input_throttle < initial_throttle) {
        input_throttle = initial_throttle;
      } else if (input_throttle < 0) {
        input_throttle = 0;
      } else if (input_throttle < 78) {
        input_throttle = input_throttle + speed_increase_rate_forward;
      } else {
        input_throttle = input_throttle + speed_increase_rate_forward;
      }

    } else if (throttle <= input_throttle) {
      input_throttle = input_throttle - speed_decrease_rate;
    } else {
      input_throttle = 0;
    }

  } else if (throttle < 0) {
    // if(input_throttle > 0 )
    // {
    //   input_throttle = input_throttle - speed_decrease_rate;

    // }
    // else

    if (input_throttle > -initial_throttle_backward) {
      input_throttle = -initial_throttle_backward;
    } else if (throttle < input_throttle) {
      if (input_throttle > 0) {
        input_throttle = 0;
      } else if (input_throttle > -90) {
        input_throttle = input_throttle - speed_increase_rate_backward;  //

      } else {
        input_throttle = input_throttle - speed_increase_rate_backward;  //
      }

    } else if (throttle >= input_throttle) {
      input_throttle = input_throttle + speed_decrease_rate;
    }
  } else {
    if (input_throttle > 0) {
      input_throttle = input_throttle - speed_decrease_rate;
    } else if (input_throttle < 0) {
      input_throttle = input_throttle + speed_decrease_rate;
    } else {
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
