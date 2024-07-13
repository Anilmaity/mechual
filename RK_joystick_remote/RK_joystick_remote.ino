int joystick_pin = A1;
int throttle_pin = 3;
int joystick_value = 0;


// throttle
float speed_increase_rate_forward = 0.002;   // if you want to increase bot acceleration in forward direction
float speed_increase_rate_backward = 0.002; // if you want to increase bot acceleration in reverse direction
float speed_decrease_rate = 0.03;          // if you want to decrease bot acceleration in both direction

int initial_throttle = 88;
int initial_throttle_backward = 88;
int max_limit = 100; // max speed limit
int throttle = 0;
float input_throttle = 0;



// will excecute one time
void setup() {

  Serial.begin(115200); // for communication between PC and arduino
  joystick_setup();
  throttle_setup();
}

void joystick_setup() {

  pinMode(joystick_pin, INPUT);


}

void send_data() {

  Serial.print(throttle); // 1500 1500 1500 1500 1500 0 0 85 548 0 0 1
  Serial.print(" ");
  Serial.print(input_throttle); // 1500 1500 1500 1500 1500 0 0 85 548 0 0 1
  Serial.print(" ");
  Serial.print(joystick_value); // 1500 1500 1500 1500 1500 0 0 85 548 0 0 1
  Serial.println(" ");
}

void get_joystick_input(){
  joystick_value = analogRead(joystick_pin);

  throttle = map(joystick_value, 320, 705, initial_throttle, max_limit);
  if (joystick_value < 320){
    throttle = 0;
  }

}

void throttle_setup(){

  //TCCR3B = TCCR3B & B11111000 | B00000010;     //WGM30 = set Timer mode to PWM
  //COM3B/C1 = clear Output on compare match
 //TCCR0B = TCCR0B & B11111000 | B00000010; // for PWM frequency of 7000 Hz
// TCCR0B = TCCR0B & B11111000 | B00000001; // for PWM frequency of 62500 Hz
    TCCR2B = TCCR2B & B11111000 | B00000010;

    pinMode(throttle_pin, OUTPUT);

}

void throttling() {



  if (throttle > 0) {
    if (throttle > input_throttle) {
      // if ( input_throttle < 0)
      // {
      //   input_throttle = input_throttle + speed_decrease_rate;

      // }
      // else

      if (input_throttle < initial_throttle) {
        input_throttle = initial_throttle;
      } else if (input_throttle < 0) {
        input_throttle = 0;
      } else if (input_throttle < 78) {
        input_throttle = input_throttle + speed_increase_rate_forward;
      } else {
        input_throttle = input_throttle + speed_increase_rate_forward;

      }

    } else if (throttle <= input_throttle) {
      input_throttle = input_throttle - speed_decrease_rate;
    } else {
      input_throttle = 0;
    }

  } else if (throttle < 0) {
    // if(input_throttle > 0 )
    // {
    //   input_throttle = input_throttle - speed_decrease_rate;

    // }
    // else

    if (input_throttle > -initial_throttle_backward) {
      input_throttle = -initial_throttle_backward;
    } else if (throttle < input_throttle) {
      if (input_throttle > 0) {
        input_throttle = 0;
      } else if (input_throttle > -90) {
        input_throttle = input_throttle - speed_increase_rate_backward;  //

      } else {
        input_throttle = input_throttle - speed_increase_rate_backward;  //
      }

    } else if (throttle >= input_throttle) {
      input_throttle = input_throttle + speed_decrease_rate;
    }
  } else {
    if (input_throttle > 0) {
      input_throttle = input_throttle - speed_decrease_rate;
    } else if (input_throttle < 0) {
      input_throttle = input_throttle + speed_decrease_rate;
    } else {
      input_throttle = 0;
    }
  }


  if (abs(throttle) > initial_throttle_backward) {

    analogWrite(throttle_pin,abs(int(input_throttle)));
    // analogWrite(throttle_pin, abs(input_throttle));

  } else {

    analogWrite(throttle_pin,abs(int(input_throttle)));

  }
}



void loop() {
  get_joystick_input();
  throttling();
  send_data();
  delay(1);

}
