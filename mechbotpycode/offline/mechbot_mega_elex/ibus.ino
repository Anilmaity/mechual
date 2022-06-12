#include <IBusBM.h>


IBusBM IBus; // IBus object

void ibus_setup() {
  // Serial.begin(115200);   // remove comment from this line if you change the Serial port in the next line

  IBus.begin(Serial1);    // iBUS connected to Serial0 - change to Serial1 or Serial2 port when required


  Serial.println("Start IBus2PWM");
}


void ibus_loop() {
  for(int i =0; i<= 5 ; i++)
  {
  channel_data[i] = IBus.readChannel(i); // get latest value for servo channel 1
 // Serial.print(channel_data[i]);
  //Serial.print(" ");
  }
  
  if(channel_data[0] <= 2000 && channel_data[0] >= 1000){
Sterring_input = map(channel_data[0],1000,2000 , -800 , 800);
  }
//Serial.println();
  delay(2);
}
