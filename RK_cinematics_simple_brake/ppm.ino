
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
      if (x[j] > 0) {
        ch1[j] = x[j];
      }
    }
    i = 0;
  }
}


void read_rc() {
  int j, k = 0;
  for (k = 14; k > -1; k--) {
    if (ch1[k] > 5500 && ch1[k]  < 20000 ) { j = k; }
  }  //detecting separation  space 10000us in that another array
  for (int ai = 1; ai <= 8; ai++) {
    if (ch1[ai] >= 980 && ch1[ai] <= 2020) {
      ch[ai] = ch1[ai + j];
    }
  }
}






void evaluteinputs() {
  valid_inputs = true;
  for (int temp = 1; temp <= 8; temp++) {
    if (ch[temp] <= 980 && ch[temp] >= 2020) {
      valid_inputs = false;
    }
  }


  if (ch[5] >= 1300 && valid_inputs) {

    if (ch[5] >= 1700) {
      Mode = "RWD";
      mode_change_time = millis();

    } else if (millis() - mode_change_time > 200) {
      Mode = "FWD";
    }



    if (ch[3] <= 2100 && ch[3] >= 1520) {

      if (Mode == "FWD") {
        throttle = 0.8 * throttle + 0.2 * map(ch[3], 1500, 2000, initial_throttle, max_limit);
      } else {
        throttle = 0.8 * throttle + 0.2 * map(ch[3], 1500, 2000, -initial_throttle_backward, -max_limit);
      }


    } else if (ch[3] <= 1480 && ch[3] >= 900) {
      if (Mode == "FWD") {
        throttle = 0.8 * throttle + 0.2 * map(ch[3], 1000, 1500, -max_limit, -initial_throttle_backward);
      } else {
        throttle = 0.8 * throttle + 0.2 * map(ch[3], 1000, 1500, max_limit, initial_throttle_backward);
      }
    } else if (ch[3] <= 1510 && ch[3] >= 1490) {
      throttle = 0;
    }


    if (ch[1] <= 2100 && ch[1] >= 1510) {
      if (Mode == "FWD") {
        sterring_value = 0.6 * sterring_value + 0.4 * map(ch[1], 1500, 2000, default_sterring_value, lowest_sterring_value);  // 912
      } else {
        sterring_value = 0.6 * sterring_value + 0.4 * map(ch[1], 1500, 2000, default_sterring_value, highest_sterring_value);  // 548 // 227
      }

    } else if (ch[1] <= 1490 && ch[1] >= 980) {
      if (Mode == "FWD") {
        sterring_value = 0.6 * sterring_value + 0.4 * map(ch[1], 1000, 1500, highest_sterring_value, default_sterring_value);  // 548 // 227
      } else {
        sterring_value = 0.6 * sterring_value + 0.4 * map(ch[1], 1000, 1500, lowest_sterring_value, default_sterring_value);  // 912
      }

    } else if (ch[1] <= 1510 && ch[1] >= 1490) {
      sterring_value = default_sterring_value;
    }


    if (ch[2] <= 1300 && ch[2] >= 980) {
      Brake = 0.9 * Brake + 0.1 * map(ch[2], 1300, 1000, initial_brake, 60);  // max brake angle
    } else if (ch[2] >= 1300 && ch[2] <= 2100) {
      Brake = initial_brake;
    }


    if (ch[6] >= 980 && ch[6] < 1300) {
      Brake = 60;
    }


    value_gone_time = millis();



  } else if (millis() - value_gone_time <= 700) {

    throttle = throttle;

  } else {
    Mode = "STOP";
    throttle = 0;
  }
}
