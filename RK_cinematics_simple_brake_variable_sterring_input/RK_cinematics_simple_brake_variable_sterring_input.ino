#include <mbed.h>


int Reverse_pin = 40;
int Brake_pin = 38;


bool flysky_connected = true;
long int Current_Speed;
long int loopstart = 0;
long int loop_time = 0;
long int motor_driver_start_time = 0;
long int Sterring_input = 0;

unsigned long int a, b, c;
int x[15], ch1[15], ch[10], i;




// Variables for brake
int Brake = 0;
int initial_brake = 0;
long int brake_time = 0;
long int brake_start_time=0;
int extented_brake = 32;
int brake_point = 35;

int brake_pwm_pin = 6;
int brake_dir_pin = 7;



// sterring_value
int S_DIR = 5;
// int S_PWM = 4;
int sterring_pwm_speed = 100;  // 0 is 0 pwm 200 is full pwm
PinName sterring_pin = digitalPinToPinName(D4);
mbed::PwmOut* sterring_pwm = new mbed::PwmOut(sterring_pin);

int sterring_pwm_increment_speed = 20 ;  // on every loop increment by 20 pwm
int sterring_pwm_start_speed = 100 ; // will start with 100 and will increase by sterring_pwm_increment_speed // here 100 ist loop , 120  2nd loop , 3 rd loop
int sterring_loop_time = 10; // sterring get input envey 10 millisecond

int S_SEN = A0;

//------------------- steering calibrations values ----------------------------//
/*
steps to get the values
1) remove the 3 pin connector from sterring motor driver
2) rotate to left most and right most using ma mb switch available on motor driver
3) note the values for the point from te serial monitor in arduino ide.

// 1500 1500 1500 1500 1500 0 0 85 (sterring value) "548"  0 0 1
*/

long default_sterring_value = 577; // 0 1024 519 573
long highest_sterring_value = 834;  // 844 Turn the robot after fixing potentiometer, max left and max right. note heighest and lowest value.
                                    //for default value set steering to an obtainable straight wheels and the note down the value from serial monitor
long lowest_sterring_value = 310;  // 160 300
//----------------------------------------------------------------------------//

long int sterring_value = 577;
long int error_sterring = 0;
long int sensorValue = 0;

long sterring_start_time = millis();

// throttle
float speed_increase_rate_forward = 0.002;   // if you want to increase bot acceleration in forward direction
float speed_increase_rate_backward = 0.002; // if you want to increase bot acceleration in reverse direction
float speed_decrease_rate = 0.03;          // if you want to decrease bot acceleration in both direction

int initial_throttle = 68;
int initial_throttle_backward = 68;
int max_limit = 120; // max speed limit
// int throttle_pin = 2;
int throttle = 0;
double input_throttle = 0;
PinName pin = digitalPinToPinName(D2);
mbed::PwmOut* pwm = new mbed::PwmOut(pin);

long int value_gone_time = 0;


// will excecute one time
void setup() {

  Serial.begin(115200); // for communication between PC and arduino
  pinMode(Reverse_pin, OUTPUT);
  pinMode(Brake_pin, OUTPUT);
  digitalWrite(Brake_pin, HIGH);
  ppm_setup();  // RC Reciever SETUP
  //intrupptsetup();
  throttle_setup();
  brake_setup();
  sterring_setup();

}

void send_data() {

  Serial.print(ch[1]); // 1500 1500 1500 1500 1500 0 0 85 548 0 0 1
  Serial.print(" ");
  Serial.print(ch[2]);
  Serial.print(" ");
  Serial.print(ch[3]);
  Serial.print(" ");
  Serial.print(ch[4]);
  Serial.print(" ");
  Serial.print(ch[5]);
  Serial.print(" ");
  Serial.print(input_throttle);
  Serial.print(" ");
  Serial.print(throttle);
  Serial.print(" ");
  Serial.print(Brake);
  Serial.print(" ");
  Serial.print(brake_time);
  Serial.print(" ");
  Serial.print(sterring_value);
  Serial.print(" ");
  Serial.print(error_sterring);
  Serial.print(" ");
  Serial.print(sensorValue);
  Serial.print(" ");
  Serial.print(loop_time);
  Serial.println(" ");
}

void loop() {
  loopstart = millis();
  read_rc();  // get data from reciever
  braking();  // apply brake
  evaluteinputs();
  send_data();
  throttling();
  loop_time = millis() - loopstart;
  sterring_input();  // read sterring sensor value (potentiometer)
  delay(1); // delay 1 millisecond
  if( millis() - sterring_start_time > sterring_loop_time ){
  sterring_loop();
  sterring_start_time = millis();
  }



}
