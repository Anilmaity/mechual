#include <mbed.h>


int Reverse_pin = 40;
int Brake_pin = 38;


bool flysky_connected = true;
long int Current_Speed;
long int loopstart = 0;
long int loop_time = 0;
long int motor_driver_start_time = 0;
long int Sterring_input = 0;

volatile long int ch[10];




// Variables for brake
float Brake = 0;
int initial_brake = 71;
int max_brake = 720;

long int brake_time = 0;
long int brake_start_time = 0;
int extented_brake = 32;
int brake_point = 35;
float brake_sensorValue = 0;
float error_brake = 0;
float error_brake_value = 0 ;

#define ANALOG_PIN(pin) pin

int brake_pwm_pin = 6;
int brake_dir_pin = 7;




// sterring_value
int S_DIR = 5;
int S_PWM = 4;
// int sterring_pwm_speed = 100;
PinName sterring_pin = digitalPinToPinName(D4);
mbed::PwmOut* sterring_pwm = new mbed::PwmOut(sterring_pin);
int noise_count = 0;
int str_noise_count = 0;
int thr_noise_count = 0;
int rev_noise_count = 0;
int emr_brk_noise_count = 0;
int brk_noise_count = 0;
bool corrected = true;
int S_SEN = A0;

//------------------- steering calibrations values ----------------------------//
/*
steps to get the values
1) remove the 3 pin connector from sterring motor driver
2) rotate to left most and right most using ma mb switch available on motor driver
3) note the values for the point from te serial monitor in arduino ide.

// 1500 1500 1500 1500 1500 0 0 85 (sterring value) "548"  0 0 1
*/

// for 100ms - 1 s do not take value from sensor

long default_sterring_value = 556;  // 0 1024 519 556  """""" centervalue """"""""
long highest_sterring_value = 785;  // 785 Turn the robot after fixing potentiometer, max left and max right. note heighest and lowest value.
                                    //for default value set steering to an obtainable straight wheels and the note down the value from serial monitor
long lowest_sterring_value = 327;   // 160 300  """" full right """"
                                    //----------------------------------------------------------------------------//

long int sterring_value = 563;  //556
int error_sterring = 0;
long int sensorValue = 0;
int clearance = 14;
long sterring_start_time = millis();

// throttle
float speed_increase_rate_forward = 0.002;   // if you want to increase bot acceleration in forward direction
float speed_increase_rate_backward = 0.002;  // if you want to increase bot acceleration in reverse direction
float speed_decrease_rate = 0.03;            // if you want to decrease bot acceleration in both direction

int initial_throttle = 68.8;
int initial_throttle_backward = 68.8;
int max_limit = 120;  // max speed limit
// int throttle_pin = 2;
int throttle = 0;
double input_throttle = 0;
PinName pin = digitalPinToPinName(D2);  //d2
mbed::PwmOut* pwm = new mbed::PwmOut(pin);

long int value_gone_time = 0;


// will excecute one time
void setup() {

  Serial.begin(115200);  // for communication between PC and arduino
  pinMode(Reverse_pin, OUTPUT);
  pinMode(Brake_pin, OUTPUT);
  digitalWrite(Brake_pin, HIGH);
  ibus_setup();
  throttle_setup();
  brake_setup();
  sterring_setup();
}

void send_data() {

  Serial.print(ch[1]);  // 1500 1500 1500 1500 1500 0 0 85 548 0 0 1
  Serial.print(" ");
  Serial.print(ch[2]);
  Serial.print(" ");
  Serial.print(ch[3]);
  Serial.print(" ");
  Serial.print(ch[4]);
  Serial.print(" ");
  Serial.print(ch[5]);
  Serial.print(" ");
  Serial.print(ch[6]);
  Serial.print(" ");
  Serial.print(ch[7]);
  Serial.print(" ");
  Serial.print(ch[8]);
  Serial.print(" ");
  Serial.print(Brake);
  Serial.print(" ");
  Serial.print(brake_sensorValue);
  Serial.print(" ");
  Serial.print(int(error_brake));
  Serial.print(" ");
  Serial.print(" ");
  Serial.print(sterring_value);
  Serial.print(" ");
  Serial.print(error_sterring);
  Serial.print(" ");
  Serial.print(sensorValue);
  Serial.print(" lpt ");
  Serial.print(loop_time);
  Serial.print(" ");
  Serial.print(input_throttle);
  Serial.print(" ");
  Serial.print(clearance);
  Serial.print(" ");
  Serial.print(throttle);
  Serial.print(" ");
  Serial.print(corrected);
  Serial.println(" ");
}

void loop() {


  loop_time = millis() - loopstart;
  loopstart = millis();

  sterring_input();  // read sterring sensor value (potentiometer)

  braking();  // apply brake
  throttling();



  send_data();

  ibus_loop();
  evaluteinputs();
  sterring_loop();
  sterring_start_time = millis();
  delay(2);
}
