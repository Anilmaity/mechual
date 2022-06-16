                  


int channel_data[10] = {0,0,0,0,0,0,0,0,0,0};

int Current_position_A =0;
int Current_position_B =0;
int Current_position = 0;

bool M_D_connected = true;
bool flysky_connected = true; 
long int Current_Speed;
long int loopstart = 0;
long int loop_time = 0;
long int motor_driver_start_time = 0;
long int Sterring_input = 0;

int throttle_pin = 4;
int throttle= 0;
void setup(){
           
   Serial.begin(115200);
   Serial.setTimeout(100);
   throttle_setup();
   motor_setup();
   ibus_setup();
   
  
}
void send_data(){
  Serial.print("@@#"+String(throttle)+"#"+String(channel_data[4])+"#"+String(channel_data[5])
  +"#"+String(Sterring_input)+"#"+String(Current_position_A)+" @ "+String(Current_position)+" @ "+String(Current_position_B)+"#"+String(flysky_connected)
  +"#"+String(M_D_connected)+"#"+String(loop_time)+"#@@");
  Serial.println();
  
}

void loop(){
loopstart = millis();
ibus_loop();
motor_loop();
send_data();
throttling();
loop_time = millis() - loopstart;
}
