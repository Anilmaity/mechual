

void brake_setup() {
  pinMode(brake_dir_pin, OUTPUT);
  pinMode(brake_pwm_pin, OUTPUT);
  pinMode(brake_in_pin, INPUT);



  brake_start_time = 0;
  Brake = initial_brake;
}


void braking() {
  brake_sensorValue = 0.1 * analogRead(A0) + 0.9 * brake_sensorValue;

  error_brake = (Brake - brake_sensorValue);

  if (Brake > initial_brake) {
    digitalWrite(Brake_pin, LOW);  // Motor drive realy switching
  } else {
    digitalWrite(Brake_pin, HIGH);  // Motor drive realy switching
  }


  if (abs(error_brake) > 10) {
    if (error_brake > 0) {
      input_throttle = 0;

      // if (abs(error_brake) < 30) {
      //   analogWrite(brake_pwm_pin, 220);
      // } else {
      //   digitalWrite(brake_pwm_pin, HIGH);
      // }

      digitalWrite(brake_pwm_pin, HIGH);
      digitalWrite(brake_dir_pin, HIGH);
    } else {

      // if (abs(error_brake) < 30) {
      //   analogWrite(brake_pwm_pin, 220);
      // } else {
      //   digitalWrite(brake_pwm_pin, HIGH);
      // }
      
      digitalWrite(brake_pwm_pin, HIGH);

      digitalWrite(brake_dir_pin, LOW);
    }




  } else {
    digitalWrite(brake_pwm_pin, LOW);
  }
}
