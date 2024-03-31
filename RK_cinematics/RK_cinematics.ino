#include <Servo.h>
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
int Brake = 90;
int brake_signal_pin = 10;
Servo brakeservo;



// sterring_value
int S_DIR = 5;

PinName sterring_pin = digitalPinToPinName(D4);
mbed::PwmOut* sterring_pwm = new mbed::PwmOut(sterring_pin);


int S_SEN = A0;

long int sterring_value = 548;
long int error_sterring = 0;
long int sensorValue = 0;


// throttle
int initial_throttle = 76;
int initial_throttle_backward = 70;
int max_limit = 120;
// int throttle_pin = 2;
int throttle = 0;
double input_throttle = 0;
PinName pin = digitalPinToPinName(D2);
mbed::PwmOut* pwm = new mbed::PwmOut(pin);





void setup() {

  Serial.begin(115200);
  pinMode(Reverse_pin, OUTPUT);
  pinMode(Brake_pin, OUTPUT);
  digitalWrite(Brake_pin, HIGH);
  ppm_setup();
  //intrupptsetup();
  throttle_setup();
  brake_setup();
  sterring_setup();
  
}
void send_data() {

  Serial.print(ch[1]);
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
  read_rc();
  braking();
  //drive_mode();
  evaluteinputs();
  send_data();
  sterring_loop();
  throttling();
  loop_time = millis() - loopstart;
}
