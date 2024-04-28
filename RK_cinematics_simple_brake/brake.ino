

void brake_setup() {
  pinMode(brake_dir_pin, OUTPUT);
  pinMode(brake_pwm_pin, OUTPUT);
  
  brake_start_time = 0;

}

void braking() {


  if (abs(Brake) > brake_point) {
    input_throttle = 0;
    digitalWrite(Brake_pin, LOW); // Motor drive realy switching

     digitalWrite(brake_pwm_pin, HIGH);
    digitalWrite(brake_dir_pin, HIGH);

  } else {
    digitalWrite(Brake_pin, HIGH);  // Motor drive realy switching

    digitalWrite(brake_pwm_pin, HIGH);
    digitalWrite(brake_dir_pin, LOW);
  }


}
