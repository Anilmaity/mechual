#include <IBusBM.h>


IBusBM IBus; // IBus object

void ibus_setup() {
  // Serial.begin(115200);   // remove comment from this line if you change the Serial port in the next line

  IBus.begin(Serial3);    // iBUS connected to Serial0 - change to Serial1 or Serial2 port when required


  Serial.println("Start IBus2PWM");
}


void ibus_loop() {
  // Getting values form all channel
  for (int i = 0; i <= 9 ; i++)
  {
    channel_data[i] = IBus.readChannel(i); // get latest value for servo channel 1

    //Serial.print(Serial3.available());
    //Serial.print(" ");
  }
  //--------------------Sterring input mapping ------------------------------
  if (channel_data[0] <= 2000 && channel_data[0] >= 1000) {

    //if (channel_data[6] < 1500){
    if (true) {

      Sterring_input = map(channel_data[0], 1000, 2000 , -1000 , 1000);
    }
    else {
      Sterring_input = map(channel_data[0], 1000, 2000 , 1000 , -1000);
    }
  }

  // ---------------Brake input mapping------------------------
  if (channel_data[4] <= 2000 && channel_data[4] >= 1000) {
    Brake_Speed = map(channel_data[4], 1000, 1500 , 2800 , 00);
  }


  //-------------------------Throttle---------------------
  if (channel_data[9] < 1500) {
    if (channel_data[2] <= 2000 && channel_data[2] >= 1000) {
      throttle = map(channel_data[2], 1000, 2000 , 50 , 104);
    }
  }
  else {
    if (channel_data[2] <= 2000 && channel_data[2] >= 1000) {
      throttle = map(channel_data[2], 1000, 2000 , 50 , 184);
    }
  }

  //------------------ encoder mode selection---------------------------

  if (channel_data[6] < 1500 ) {
    enable_encoder = true;
  }
  else {
    enable_encoder = false;

  }


  // ------------------------Drive mode mapping --------------------------
  //if(channel_data[6]< 1500 ){
  if (true) {
    if (channel_data[5] > 900 && channel_data[5] < 1400 ) {
      Drive_mode = 'N';

      if (previous_drive_mode != Drive_mode) {
        //drive_mode_change = true;
      }
      if(abs(Brake_Speed) <20){
      neutral_animation();
      }
     
    }
    else  if (channel_data[5] > 1400 && channel_data[5] < 1600) {
      Drive_mode = 'F';
      if (previous_drive_mode != Drive_mode) {
        //drive_mode_change = true;
      }
      if(abs(Brake_Speed) <20){
      forward_animation();
      }
       neutral_light_start = millis();
    }
    else
    {
      Drive_mode = 'R';
      if (previous_drive_mode != Drive_mode) {
        //drive_mode_change = true;
      }
        if(abs(Brake_Speed) <20){
      reverse_animation();
      }
       neutral_light_start = millis();
    }
  }
  else {
    if (channel_data[5] > 900 && channel_data[5] < 1400 ) {
      Drive_mode = 'N';
      if (previous_drive_mode != Drive_mode) {
        drive_mode_change = true;
      }
      neutral_animation();
    }
    else  if (channel_data[5] > 1400 && channel_data[5] < 1600) {
      Drive_mode = 'R';
      if (previous_drive_mode != Drive_mode) {
        drive_mode_change = true;
      }
      reverse_animation();
    }
    else
    {
      Drive_mode = 'F';
      if (previous_drive_mode != Drive_mode) {
        drive_mode_change = true;
      }
      forward_animation();
    }

  }
  previous_drive_mode = Drive_mode;
  drive_mode_change = false;
  
  //-----------------------------------Light mapping----------------
  if (channel_data[9] <= 2000 && channel_data[9] >= 1000) {
    left_side_light = 'N';
  }
  else {
    left_side_light = 'F';
  }

  if (channel_data[8] <= 2000 && channel_data[8] >= 1000) {
    right_side_light = 'N';
  }
  else {
    right_side_light = 'F';
  }

  if (channel_data[7] <= 2000 && channel_data[7] >= 1600) {
    Head_light_mode = 'U';
  }
  else if (channel_data[7] <= 1600 && channel_data[7] >= 1400) {
    Head_light_mode = 'D';
  }
  else {
    Head_light_mode = 'F';

  }

  if (abs(Brake_Speed) > 20) {
    back_light = 'N';
    brake_animation();
  }
  else {
    back_light = 'F';
    
  }


  //Serial.println();
  //delay(50);
}
