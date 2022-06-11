


#include<RMCS2303drive.h>

RMCS2303 rmcs;                      

//Parameter Settings "Refer datasheet for details" - 
byte slave_id=7;                    //Choose the slave id of connected drive.
int INP_CONTROL_MODE=513;           //IMPORTANT: refer datasheet and set value(integer) according to application 
int PP_gain=32;
int PI_gain=16;
int VF_gain=32;
int LPR=334;
int acceleration=60000;
int speed=8000;

long int Current_position;
long int Current_Speed;

void setup(){
           
   Serial.begin(115200);
   Serial.setTimeout(50);
   motor_setup();
   ibus_setup();
   
  
}

void loop(){
ibus_loop();
 motor_loop();
}
