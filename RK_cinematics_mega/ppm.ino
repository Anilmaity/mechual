
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
  x[count] = c;      //storing 15 value in  array

  count = count + 1;
  if (count == 15) {
    for (int j = 0; j < 15; j++) {
        ch1[j] = x[j];
    }
    count = 0;
  }
}


void read_rc() {
  int i, j, k = 0;
  for (k = 14; k > -1; k--) {
    if (ch1[k] > 6000) { j = k; }
  }  //detecting separation  space 10000us in that another array
  for (i = 1; i <= 6; i++) { 
    if(ch1[i] != 0)
    {
    ch[i] = ch1[i + j]; 
    }
    }
}






void evaluteinputs() {

  if (ch[5] >= 1600) {
    if (ch[3] <= 2100 && ch[3] >= 1520) {
      throttle = map(ch[3], 1500, 2000, initial_throttle, max_limit);
    } else if (ch[3] <= 1480 && ch[3] >= 900) {
      throttle = map(ch[3], 1000, 1500, -max_limit, -initial_throttle_backward );
    } else {
      throttle = 0;
    }


    if (ch[1] <= 2100 && ch[1] >= 1510) {
      sterring_value = map(ch[1], 1500, 2000, default_sterring_value, highest_sterring_value); // 912
    } else if (ch[1] <= 1490 && ch[1] >= 900) {
      sterring_value = map(ch[1], 1000, 1500, lowest_sterring_value, default_sterring_value); // 548 // 227
    } else {
      sterring_value = default_sterring_value;
    }


    if (ch[2] <= 1300 && ch[2] >= 900) {
      Brake = map(ch[2], 1300, 1000, 90, 90 - max_brake_angle); // max brake angle
    } 
    else{
      Brake = 90;
    }



  } else {

    throttle = 0;
  }
}

