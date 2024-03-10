

void intrupptsetup() {

  pinMode(channel_pins[0], INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(channel_pins[0]), ch1, RISING);

  pinMode(channel_pins[1], INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(channel_pins[1]), ch2, RISING);

  pinMode(channel_pins[2], INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(channel_pins[2]), ch3, RISING);

  pinMode(channel_pins[3], INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(channel_pins[3]), ch4, RISING);
}



void ch1() {
  attachInterrupt(digitalPinToInterrupt(channel_pins[0]), c1, FALLING);
  CT1 = micros();
}
void c1() {
  attachInterrupt(digitalPinToInterrupt(channel_pins[0]), ch1, RISING);
  tempcha1 = micros() - CT1;
  if (tempcha1 > 900 && tempcha1 < 2100) {
    cha1 = tempcha1;
  } else {
    cha1 = cha1;
  }
}


void ch2() {
  attachInterrupt(digitalPinToInterrupt(channel_pins[1]), c2, FALLING);
  CT2 = micros();
}
void c2() {
  attachInterrupt(digitalPinToInterrupt(channel_pins[1]), ch2, RISING);
  tempcha2 = micros() - CT2;
  if (tempcha2 > 900 && tempcha2 < 2100) {
    cha2 = tempcha2;
  } else {
    cha2 = cha2;
  }
}




void ch3() {
  attachInterrupt(digitalPinToInterrupt(channel_pins[2]), c3, FALLING);
  CT3 = micros();
}
void c3() {
  attachInterrupt(digitalPinToInterrupt(channel_pins[2]), ch3, RISING);
  tempcha3 = micros() - CT3;
  if (tempcha3 > 900 && tempcha3 < 2100) {
    cha3 = tempcha3;
  } else {
    cha3 = cha3;
  }
}


void ch4() {
  attachInterrupt(digitalPinToInterrupt(channel_pins[3]), c4, FALLING);
  CT4 = micros();
}
void c4() {
  attachInterrupt(digitalPinToInterrupt(channel_pins[3]), ch4, RISING);
  tempcha4 = micros() - CT4;
  if (tempcha4 > 900 && tempcha4 < 2100) {
    cha4 = tempcha4;
  } else {
    cha4 = cha4;
  }
}



void evaluteinputs() {

  if (cha4 >= 1600) {
    if (cha1 <= 2000 && cha1 >= 1510) {
      throttle = map(cha1, 1500, 2000, initial_throttle, max_limit);
    } else if (cha1 <= 1490 && cha1 >= 1000) {
      throttle = map(cha1, 1000, 1500, -max_limit, -initial_throttle);
    } else {
      throttle = 0;
    }


    if (cha2 <= 2000 && cha2 >= 1510) {
      sterring_value = map(cha2, 1500, 2000, 530, 860);
    } else if (cha2 <= 1490 && cha2 >= 1000) {
      sterring_value = map(cha2, 1000, 1500, 200, 530);
    } else {
      sterring_value = 530;
    }


    if (cha3 <= 2000 && cha3 >= 1000) {
      Brake = map(cha3, 1000, 200, 0, 90);
    } else {
      Brake = 0;
    }



  } else {

    throttle = 0;
  }
}
