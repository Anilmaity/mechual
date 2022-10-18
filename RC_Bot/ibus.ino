#include <IBusBM.h>


IBusBM IBus; // IBus object

void ibus_setup() {
  // Serial.begin(115200);   // remove comment from this line if you change the Serial port in the next line

  IBus.begin(Serial);    // iBUS connected to Serial0 - change to Serial1 or Serial2 port when required


  Serial.println("Start IBus2PWM");
}


void ibus_loop() {
  // Getting values form all channel
  for (int i = 0; i <= 5 ; i++)
  {
    channel_data[i] = IBus.readChannel(i); // get latest value for servo channel 1
   Serial.print(channel_data[i]);
    //Serial.print(Serial3.available());
   Serial.print(" ");
  }
  //--------------------Sterring input mapping ------------------------------
     if (channel_data[3] <= 2000 && channel_data[3] >= 1510) {
      sterring_value = map(channel_data[3], 1500, 2000 , 0 , 120);
    }
      else if (channel_data[3] <= 1490 && channel_data[3] >= 1000) {
      sterring_value = map(channel_data[3], 1000, 1500 , -120 , 0);
    }
    else{
      sterring_value =0;
    }
  



  //-------------------------Throttle---------------------
    if (channel_data[1] <= 2000 && channel_data[1] >= 1600) {
      throttle = map(channel_data[1], 1600, 2000 , initial_throttle , 147);
     // throttle = map(channel_data[1], 1600, 2000 , initial_throttle  , max_limit);
    }
      else if (channel_data[1] <= 1400 && channel_data[1] >= 1000) {
      throttle = map(channel_data[1], 1000, 1400 , -max_limit , -initial_throttle);
    }
     else if (channel_data[1] <= 1600 && channel_data[1] > 1510) {
      throttle = initial_throttle;
    }
     else if (channel_data[1] <= 1490 && channel_data[1] >= 1400) {
      throttle = -initial_throttle;
    }
    else{
      throttle=0;
    }

    
  

  

  //-------------------------Brake---------------------
    if (channel_data[5] <= 2000 && channel_data[5] >= 1500) {
      Brake = map(channel_data[5], 1500, 2000 , 1 , 180);
    }
    else{
      Brake = 1;
    }
  


  delay(10);
}
