

void brake_setup() {
  pinMode(brake_dir_pin, OUTPUT);
  pinMode(brake_pwm_pin, OUTPUT);
  
  brake_start_time = 0;

}

void braking() {

  if(Brake < extented_brake && abs(input_throttle) > 80){
    if(braked == false){
    Brake = extented_brake;
 }

    //Brake = extented_brake;

    // if(brake_time_modified == false)
    // {
    // brake_time = brake_time + 150;
    // brake_time_modified = true;
    // }

   
  

  }

  if(brake_time > 4800 && braked == false){
    braked = true;
    //brake_time_modified = false;  // remove this 
  }
  // if(braked == true && Brake < brake_point){
  //   if(brake_time_modified == false)
  //   {
  //   brake_time = brake_time - 200;
  //   brake_time_modified == true;
  //   }
  // }


  if(brake_time < 10 && braked == true){
    braked = false;
    brake_time_modified = false;
  }

  
  if (abs(Brake) > brake_point) {
    input_throttle = 0;
    digitalWrite(Brake_pin, LOW); // Motor drive realy switching
  } else {
    digitalWrite(Brake_pin, HIGH);  // Motor drive realy switching
  }

  if(((Brake*100) - brake_time) > 0)
  {
    brake_time = brake_time + millis() - brake_start_time;
    digitalWrite(brake_pwm_pin, HIGH);  
    digitalWrite(brake_dir_pin, HIGH);  

  }
  else if(((Brake*100) - brake_time) < 0)
  {
    brake_time = brake_time - (millis() - brake_start_time);
    digitalWrite(brake_pwm_pin, HIGH);  
    digitalWrite(brake_dir_pin, LOW);  
  }
  else{

    digitalWrite(brake_pwm_pin, LOW);  
    digitalWrite(brake_dir_pin, LOW);  

  }

  brake_start_time = millis();


}
