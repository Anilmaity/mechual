#include <IBusBM.h>


IBusBM IBus; // IBus object

void ibus_setup() {
  // Serial.begin(115200);   // remove comment from this line if you change the Serial port in the next line

  IBus.begin(Serial3);    // iBUS connected to Serial0 - change to Serial1 or Serial2 port when required


  Serial.println("Start IBus2PWM");
}


void ibus_loop() {
 
  for(int i =0; i<= 9 ; i++)
  {
  channel_data[i] = IBus.readChannel(i); // get latest value for servo channel 1
 
  //Serial.print(Serial3.available());
  //Serial.print(" ");
  }
  
  if(channel_data[0] <= 2000 && channel_data[0] >= 1000){

    //if (channel_data[6] < 1500){
    if(true){

Sterring_input = map(channel_data[0],1000,2000 , -1000 , 1000);
    }
    else{
      Sterring_input = map(channel_data[0],1000,2000 , 1000 , -1000);
      }
    }
    

  if(channel_data[4] <= 2000 && channel_data[4] >= 1000){
Brake_Speed = map(channel_data[4],1000,1500 , 2800 , 00);
  }

if(channel_data[9]< 1500){
    if(channel_data[2] <= 2000 && channel_data[2] >= 1000){
    throttle = map(channel_data[2],1000,2000 , 50 , 104);
  }
}
else{
      if(channel_data[2] <= 2000 && channel_data[2] >= 1000){
    throttle = map(channel_data[2],1000,2000 , 50 , 184);
  }
}

if(channel_data[6]< 1500 ){
enable_encoder = true;
}
else{
enable_encoder = false;
 
}



//if(channel_data[6]< 1500 ){
if(true){
if(channel_data[5]> 900 && channel_data[5] <1400 ){
    Drive_mode = 'N';
}
else  if(channel_data[5]> 1400 && channel_data[5] <1600){
        Drive_mode = 'F';
        }
  else
{
          Drive_mode = 'R';
}
}
else{
 if(channel_data[5]> 900 && channel_data[5] <1400 ){
    Drive_mode = 'N';
}
else  if(channel_data[5]> 1400 && channel_data[5] <1600){
        Drive_mode = 'R';
        }
  else
{
          Drive_mode = 'F';
} 
}

Serial.println();
  //delay(50);
}
