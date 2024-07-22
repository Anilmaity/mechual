#include <iBus.h>

#define MAX_CHANNELS 10

iBus receiver(Serial4, MAX_CHANNELS);  // Serial2 pins in arduino giga
                                     
void ibus_setup() {
  receiver.begin();
}

void ibus_loop() {
  receiver.process();

  for(byte i = 1; i <= MAX_CHANNELS; i++){  // get channel values starting from 1
  int value = receiver.get(i);
  if(value >= 1000 && value <= 2000)
  {
    ch[i] = value;
  }
    //Serial.print(receiver.get(i));
    //Serial.print('\t');
  }
  //Serial.println(receiver.get(0));  // received packet quality. If 0 -> there are some corrupted values

}




void evaluteinputs() {

  if (ch[7] >= 1200) {

    if (ch[6] >= 1500) {

      lowest_sterring_value = 805;
      highest_sterring_value = 321;

    } else {
      lowest_sterring_value = 321;
      highest_sterring_value = 805;
    }


    if (ch[3] <= 2100 && ch[3] >= 1520) {

      if (ch[6] >= 1500) {
        throttle = map(ch[3], 1500, 2000, -initial_throttle_backward, -max_limit);

      } else {
        throttle = map(ch[3], 1500, 2000, initial_throttle, max_limit);
      }


    } else if (ch[3] <= 1480 && ch[3] >= 900) {
      if (ch[6] >= 1500) {
        throttle = map(ch[3], 1000, 1500, max_limit, initial_throttle_backward);

      } else {
        throttle = map(ch[3], 1000, 1500, -max_limit, -initial_throttle_backward);
      }
    } else if (ch[3] <= 1510 && ch[3] >= 1480) {
      throttle = 0;
    }


    if (ch[1] <= 2100 && ch[1] >= 1520) {
      sterring_value = map(ch[1], 1500, 2000, default_sterring_value, lowest_sterring_value);  // 912
    } else if (ch[1] <= 1490 && ch[1] >= 900) {
      sterring_value = map(ch[1], 1000, 1500, highest_sterring_value, default_sterring_value);  // 548 // 227
    } else if (ch[1] <= 1510 && ch[1] >= 1490) {
      sterring_value = default_sterring_value;
    }



    if (ch[5] >= 980 && ch[5] < 1490) {
      Brake = map(ch[5], 1490, 1000, initial_brake, max_brake);  // max brake angle
    }
    else{
      Brake = initial_brake;
    }






  }else {
    throttle = 0;
  }
}
