int joystick_pin = A1;
int throttle_pin = 3;
int joystick_value = 0;


// throttle
float speed_increase_rate_forward = 0.006;   // if you want to increase bot acceleration in forward direction
float speed_increase_rate_backward = 0.004; // if you want to increase bot acceleration in reverse direction
float speed_decrease_rate = 0.03;          // if you want to decrease bot acceleration in both direction

int initial_throttle = 68;
int initial_throttle_backward = 68;
int max_limit = 120; // max speed limit
// int throttle_pin = 2;
int throttle = 0;



// will excecute one time
void setup() {

  Serial.begin(115200); // for communication between PC and arduino
  joystick_setup();

}

void joystick_setup() {

  pinMode(joystick_pin, INPUT);


}

void send_data() {

  Serial.print(throttle); // 1500 1500 1500 1500 1500 0 0 85 548 0 0 1
  Serial.print(" ");
  Serial.print(joystick_value); // 1500 1500 1500 1500 1500 0 0 85 548 0 0 1
  Serial.print(" ");
  Serial.print(joystick_value); // 1500 1500 1500 1500 1500 0 0 85 548 0 0 1
  Serial.println(" ");
}

void get_joystick_input(){
  joystick_value = analogRead(joystick_pin);

  throttle = map(joystick_value, 320, 700, initial_throttle, max_limit);

}

void throttle_setup(){

  //  TCCR3B = TCCR3B & B11111000 | B00000010;     //WGM30 = set Timer mode to PWM
  //COM3B/C1 = clear Output on compare match
 //TCCR0B = TCCR0B & B11111000 | B00000010; // for PWM frequency of 7000 Hz
  //TCCR0B = TCCR0B & B11111000 | B00000001; // for PWM frequency of 62500 Hz
  
    pinMode(throttle_pin, OUTPUT);

}

void throttling(){
     analogWrite(throttle_pin, abs(throttle));
}


void loop() {
  get_joystick_input();
  throttling();
  send_data();

}
