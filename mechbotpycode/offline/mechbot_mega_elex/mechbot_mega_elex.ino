                  
//---------------Sterring---------------------------------------
int DIR_pin = 8;
int PWM_pin = 9;
int ENC_A = 20;
int ENC_B = 21;
int A =0;
int B=0;
int position_error = 0;
int sterring_speed = 0;
//------------------------------------

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


// Variables for brake
int B_DIR_pin[2] = {4,5};
int B_PWM_pin[2] = {6,7};
int B_ENC_A[2] = {18,2};
int B_ENC_B[2] = {19,3};
int B_position_error[2] = {0,0};
int brake_speed = 0;
int B_Current_position_B[2] = {0,0};
int B_Current_position_A[2] = {0,0};
int Current_brake_position[2] = {0,0};


void setup(){
           
   Serial.begin(115200);
   Serial.setTimeout(100);
   throttle_setup();
   brake_setup();
   motor_setup();
   ibus_setup();
   
  
}
void send_data(){
  //Serial.print("@@#"+String(throttle)+"#"+String(channel_data[4])+"#"+String(channel_data[5])
  //+"#"+String(Sterring_input)+"# "+String(Current_brake_position[0])+" @ "+String(Current_position)+" @ "+String(Current_brake_position[1])+" #"+String(flysky_connected)
  //+"#"+String(M_D_connected)+"#"+String(loop_time)+"#@@");
  //Serial.println();
  
    Serial.println("@@# "+String(A)+" # "+String(Current_position)+" # "+String(B)+" #@@ ");

}

void loop(){
loopstart = millis();
ibus_loop();
motor_loop();
send_data();
throttling();
loop_time = millis() - loopstart;
}
