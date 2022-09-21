#include <IBusBM.h>

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

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
  // Serial.print(channel_data[i]);
    //Serial.print(Serial3.available());
 //  Serial.print(" ");
  }
  //--------------------Sterring input mapping ------------------------------
  if (channel_data[0] <= 2000 && channel_data[0] >= 1000) {

    //if (channel_data[0] < 1500){
    if (true) {

      sterring_value = map(channel_data[0], 1000, 2000 , -150 , 150);
    }
    else {
      sterring_value = map(channel_data[0], 1000, 2000 , 150 , -150);
    }
  }



  //-------------------------Throttle---------------------
  if (channel_data[2] < 1500) {
    if (channel_data[2] <= 2000 && channel_data[2] >= 1000) {
      throttle = map(channel_data[2], 1000, 2000 , 0 , 104);
    }
  }
  else {
    if (channel_data[2] <= 2000 && channel_data[2] >= 1000) {
      throttle = map(channel_data[2], 1000, 2000 , 0 , 184);
    }
  }

  

  //-------------------------Brake---------------------
    if (channel_data[4] <= 2000 && channel_data[2] >= 1000) {
      Brake = map(channel_data[4], 1000, 2000 , 0 , 1);
    }
  


  //delay(50);
}
