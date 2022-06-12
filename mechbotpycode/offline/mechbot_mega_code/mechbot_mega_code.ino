
#include<RMCS2303drive.h>

RMCS2303 rmcs;                      

//Parameter Settings "Refer datasheet for details" - 
byte slave_id=7;                    //Choose the slave id of connected drive.
int INP_CONTROL_MODE=513;           //IMPORTANT: refer datasheet and set value(integer) according to application 
int PP_gain=32;
int PI_gain=16;
int VF_gain=32;
int LPR=334;
int acceleration=10000;
int speed=1000;

int channel_data[6] = {0,0,0,0,0,0};


bool M_D_connected = true;
bool flysky_connected = true; 
long int Current_position;
long int Current_Speed;
long int loopstart = 0;
long int loop_time = 0;
long int motor_driver_start_time = 0;
long int Sterring_input = 0;


void setup(){
           
   Serial.begin(115200);
   Serial.setTimeout(50);
   
   motor_setup();
   ibus_setup();
   
  
}
void send_data(){
  Serial.print("@@#"+String(channel_data[2])+"#"+String(channel_data[4])+"#"+String(channel_data[5])
  +"#"+String(Sterring_input)+"#"+String(Current_position)+"#"+String(flysky_connected)
  +"#"+String(M_D_connected)+"#"+String(loop_time)+"#@@");
  Serial.println();
  
}

void loop(){
loopstart = millis();
ibus_loop();
if((millis() - motor_driver_start_time) >= 300 ){
motor_loop();
motor_driver_start_time = millis();
}
send_data();
loop_time = millis() - loopstart;

}
