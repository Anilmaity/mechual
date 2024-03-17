#include <Servo.h>


int Reverse_pin = 38;
int Brake_pin = 40;


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
int brake_signal_pin = 10;
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
int max_limit = 140;
int throttle_pin = 2;
int throttle = 0;
int input_throttle = 0;





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
  delay(40);
}
