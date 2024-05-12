
//specifing  arrays and variables to store values

void ppm_setup() {
  pinMode(18, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(18), read_me, FALLING);
  // enabling interrupt at pin 2
}


void read_me() {

  a = micros();  //store time value a when pin value falling
  c = a - b;     //calculating  time inbetween two peaks
  b = a;         //
  x[i] = c;      //storing 15 value in  array
  i = i + 1;
  if (i == 15) {
    for (int j = 0; j < 15; j++) {
      if (x[j] > 0){
        
        ch1[j] = x[j];

      }
    }
    i = 0;
    read_rc();
    evaluteinputs();

  }
}


void read_rc() {
  int i, j, k = 0;
  for (k = 14; k > -1; k--) {
    if (ch1[k] > 5500) { j = k; }
  }  //detecting separation  space 10000us in that another array
  for (i = 1; i <= 6; i++) { 
    if(ch1[i+j] > 980 && ch1[i+j] < 2020 )
    {
    ch[i] = ch1[i + j]; 
    }
    }
}






void evaluteinputs() {

  if (ch[5] >= 1600) {
    if (ch[3] <= 2100 && ch[3] >= 1520) {
      throttle = 0.8 * throttle + 0.2 * map(ch[3], 1500, 2000, initial_throttle, max_limit);
    } else if (ch[3] <= 1480 && ch[3] >= 900) {
      throttle = 0.8 * throttle + 0.2 * map(ch[3], 1000, 1500, -max_limit, -initial_throttle_backward );
    } 
    else if (ch[3] <= 1510 && ch[3] >= 1490) {
      throttle = 0;
    }


    if (ch[1] <= 2100 && ch[1] >= 1510) {
      sterring_value = 0.6 * sterring_value + 0.4 * map(ch[1], 1500, 2000, default_sterring_value, lowest_sterring_value); // 912
    } else if (ch[1] <= 1490 && ch[1] >= 900) {
      sterring_value = 0.6 * sterring_value + 0.4 * map(ch[1], 1000, 1500, highest_sterring_value, default_sterring_value); // 548 // 227
    } 
    else if(ch[1] <= 1510 && ch[1] >= 1490){
      sterring_value = default_sterring_value;
    }


    if (ch[2] <= 1300 && ch[2] >= 900) {
      Brake = 0.9 * Brake + 0.1 * map(ch[2], 1300, 1000, initial_brake, 60); // max brake angle
    } 
    else if (ch[2] != 0){
      Brake = initial_brake;
    }


    if(ch[6] > 980 && ch[6] < 1300){
      Brake = 60;
    }

    value_gone_time = millis();



  }
   else if(millis() - value_gone_time <= 100){

    throttle = throttle;
  }
  else{
      throttle = 0;
  }
}

