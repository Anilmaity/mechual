//https://arduinoinfo.mywikis.net/wiki/Arduino-PWM-Frequency 
long int sterring_value = 0;
long int throttle = 10; //2826 RPM
//------------------------------------

float diff_pwm = 0.78;
int left_const = 7;
int increment_value = 20;
int cutoff = 80;
int initial_throttle = 46;
int max_limit = 170;
int Right_max_limit = 147;

//int Neutral_pin = 6;
int Right_Reverse_pin = 2; //1500 RPM //1760,1525
int Left_Reverse_pin = 4;

int Right_throttle_pin = 3;
int Right_throttle = 0;

int Left_throttle_pin = 6;
int Left_throttle = 0;

int Brake_pin = 7;

 int Right_motor_value = 0;
 int last_Right_motor_value = 0;
 int Left_motor_value = 0;
 int last_Left_motor_value = 0;

int channel_data[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

bool flysky_connected = true;
long int Current_Speed;
long int loopstart = 0;
long int loop_time = 0;



// Variables for brake
#include <Servo.h>
Servo BrakeServo[2];
int B_pin[2] = {9, 10};
int Brake = 0;
long int last_signal = 0;
void setup() {

  Serial.begin(115200);
  brake_setup();
  motor_setup();
  ibus_setup();

}

void loop() {
  loopstart = millis();
  ibus_loop();
  drive();
  brake();
  raw_print();
  loop_time = millis() - loopstart;
}

void raw_print(){
     Serial.print(sterring_value);
    Serial.print(" ");
   Serial.print(Brake);
    Serial.print(" ");
   Serial.print(throttle);
    Serial.print(" ");
       Serial.print(Right_motor_value);
    Serial.print(" ");
       Serial.print(Left_motor_value);
    Serial.print(" ");
    Serial.println();

}
void motor_setup() {

    pinMode(Right_Reverse_pin, OUTPUT);
    pinMode(Left_Reverse_pin, OUTPUT);

  pinMode(Left_throttle_pin, OUTPUT);
  pinMode(Right_throttle_pin, OUTPUT);
  pinMode(Brake_pin, OUTPUT);
  digitalWrite(Brake_pin,HIGH);
  TCCR2B = TCCR2B & B11111000 | B00000001;    // D3,D11 set timer 3 divisor to     1 for PWM frequency of 31372.55 Hz
  //TCCR0B = TCCR0B & B11111000 | B00000001; //D5,D6 62500 Hz
  TCCR0B = TCCR0B & B11111000 | B00000010;    // set timer 0 divisor to     8 for PWM frequency of  7812.50 Hz
}

void drive() {
 if(abs(throttle)>46){
Right_motor_value = throttle + (10*((throttle )/abs(throttle )+1)) ;
Left_motor_value = throttle  ;
 }else{
Right_motor_value = - sterring_value ;
Left_motor_value = + sterring_value ;
 }

if(abs(Right_motor_value) > Right_max_limit) Right_motor_value = Right_motor_value/abs(Right_motor_value)*Right_max_limit;
if(abs(Left_motor_value) > Right_max_limit)  Left_motor_value = Left_motor_value/abs(Left_motor_value)*Right_max_limit;

if(abs(throttle)>cutoff){
    if(throttle>=0){
  digitalWrite(Right_Reverse_pin, LOW);
  digitalWrite(Left_Reverse_pin, LOW);
}

else{
  digitalWrite(Right_Reverse_pin, HIGH);
  digitalWrite(Left_Reverse_pin, HIGH);
}
}
else{
  if(Right_motor_value>=0){

      digitalWrite(Right_Reverse_pin, LOW);
  }
  else{
      digitalWrite(Right_Reverse_pin, HIGH);
  }

  if(Left_motor_value>=0){
      digitalWrite(Left_Reverse_pin, LOW);
  }
  else{
      digitalWrite(Left_Reverse_pin, HIGH);
  }
}

if(abs(Right_motor_value)>initial_throttle){

if(abs(Right_motor_value - last_Right_motor_value > increment_value))
{
analogWrite(Right_throttle_pin,abs(last_Right_motor_value) + increment_value);
last_Right_motor_value = int(0.8*last_Right_motor_value + 0.2*Right_motor_value);
}
else{
  analogWrite(Right_throttle_pin,abs(Right_motor_value));

}
}
else{
analogWrite(Right_throttle_pin,initial_throttle);
  }


if(abs(Left_motor_value)>initial_throttle){
  if(abs(Left_motor_value - last_Left_motor_value)>increment_value){
analogWrite(Left_throttle_pin,abs(last_Left_motor_value)+increment_value+left_const);
  last_Left_motor_value= int(0.8*last_Left_motor_value + 0.2*Left_motor_value);
  }
  else{
analogWrite(Left_throttle_pin,abs(Left_motor_value)+left_const);
  }

}
else{
analogWrite(Left_throttle_pin,initial_throttle);
  }


}

void brake_setup() {
  BrakeServo[0].attach(B_pin[0]);
  BrakeServo[1].attach(B_pin[1]);
  delay(100);
  BrakeServo[0].write(0);
  BrakeServo[1].write(0);
  last_signal = millis();
}
void brake() {
  if(millis()- last_signal > 15)
  {

  BrakeServo[0].write(Brake);
  BrakeServo[1].write(Brake);

  last_signal = millis();
  if(Brake > 10){
      digitalWrite(Brake_pin,HIGH);
  }
  else{
      digitalWrite(Brake_pin,LOW);

  }
  }
}