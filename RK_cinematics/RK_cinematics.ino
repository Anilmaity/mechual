#include <Servo.h>


int Reverse_pin = 26;
int Brake_pin = 22;

int channel_pins[4] = { 2, 3, 18, 19 };  // ch 1 , ch3 , ch5 , ch6

volatile int cha1, cha2, cha3, cha4, cha5, cha6;
volatile int tempcha1, tempcha2, tempcha3, tempcha4, tempcha5, tempcha6;

volatile int CT1, CT2, CT3, CT4, CT5, CT6;


bool flysky_connected = true;
long int Current_Speed;
long int loopstart = 0;
long int loop_time = 0;
long int motor_driver_start_time = 0;
long int Sterring_input = 0;




// Variables for brake
int Brake = 0;
int brake_signal_pin = 2;
Servo brakeservo;



// sterring_value
int S_DIR = 6;
int S_PWM = 7;
int S_SEN = A0;
long int sterring_value = 512;
long int error = 0;
long int sensorValue = 0;


// throttle
int initial_throttle = 50;
int max_limit = 147;
int throttle_pin = 5;
int throttle = 0;






void setup() {

  Serial.begin(115200);
  pinMode(Reverse_pin, OUTPUT);
  pinMode(Brake_pin, OUTPUT);
  digitalWrite(Brake_pin, HIGH);
  intrupptsetup();
  throttle_setup();
  brake_setup();
  sterring_setup();
}
void send_data() {


  // Serial.println());
}

void loop() {
  loopstart = millis();
  evaluteinputs();
  braking();
  drive_mode();
  send_data();
  sterring_loop();
  throttling();
  loop_time = millis() - loopstart;
}
