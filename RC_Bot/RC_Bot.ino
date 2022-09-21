long int sterring_value = 0;
long int throttle = 0; 
//------------------------------------
//int Neutral_pin = 6;
int Right_Reverse_pin = 5;
int Left_Reverse_pin = 5;

//int Right_Brake_pin = 7;
int Right_throttle_pin = 3;
int Right_throttle = 0;

int Left_throttle_pin = 4;
int Left_throttle = 0;

long int Right_motor_value = 0;
long int Left_motor_value = 0;
 
int channel_data[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

bool flysky_connected = true;
long int Current_Speed;
long int loopstart = 0;
long int loop_time = 0;



// Variables for brake
int B_DIR_pin[2] = {9, 10};
int B_PWM_pin = 8;
int B_min_LS_pin = A0;
int B_min_LS_val = 0;
int B_max_LS_val = 0;
int B_max_LS_pin = A1;
int Brake = 0;
int last_Brake = 0;


void setup() {

  Serial.begin(115200);
  Serial.setTimeout(100);
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
       Serial.print(last_Brake);
    Serial.print(" ");
   Serial.print(throttle);
    Serial.print(" ");
       Serial.print(B_min_LS_val);
    Serial.print(" ");
       Serial.print(B_max_LS_val);
    Serial.print(" ");
    Serial.println();

}
void motor_setup() {

    pinMode(Right_Reverse_pin, OUTPUT);
    pinMode(Left_Reverse_pin, OUTPUT);
    
  pinMode(Left_throttle_pin, OUTPUT);
  pinMode(Right_throttle_pin, OUTPUT);
  TCCR0B = TCCR0B & B11111000 | B00000010; // for PWM frequency of 7000 Hz
  //TCCR0B = TCCR0B & B11111000 | B00000001; // for PWM frequency of 62500 Hz

}

void drive() {
Right_motor_value = throttle + sterring_value;
Left_motor_value = throttle + sterring_value ;
if(abs(Right_motor_value)>15){
if(Right_motor_value > 0){
  analogWrite(Right_throttle_pin,Right_motor_value);
  digitalWrite(Right_Reverse_pin, LOW);
}
else{
    analogWrite(Right_throttle_pin,Right_throttle_pin);
  digitalWrite(Right_Reverse_pin, LOW);

}
}
else{
  analogWrite(Right_throttle_pin,0);
  digitalWrite(Right_Reverse_pin, LOW);
}

if(abs(Left_motor_value)>15){

if(Left_motor_value > 0){
  analogWrite(Left_throttle_pin,Left_motor_value);
  digitalWrite(Left_Reverse_pin, LOW);
}
else{
    analogWrite(Left_throttle_pin,Left_motor_value);
  digitalWrite(Left_Reverse_pin, LOW);

}
}
else{
  analogWrite(Right_throttle_pin,0);
  digitalWrite(Right_Reverse_pin, LOW);
}

}

void brake_setup() {
  pinMode(B_min_LS_pin, INPUT_PULLUP);
  pinMode(B_max_LS_pin, INPUT_PULLUP);
  pinMode(B_DIR_pin[0],OUTPUT);
  pinMode(B_DIR_pin[1],OUTPUT);
  pinMode(B_PWM_pin,OUTPUT);
}
void brake() {
  B_min_LS_val = digitalRead(B_min_LS_pin);
  B_max_LS_val = digitalRead(B_max_LS_pin);
  if (last_Brake == 0 && Brake == 1){
    if(B_max_LS_val != 0){
    digitalWrite( B_DIR_pin[0],HIGH);
    digitalWrite( B_DIR_pin[1],LOW);
    }
    else{
    digitalWrite( B_DIR_pin[0],LOW);
    digitalWrite( B_DIR_pin[1],LOW); 
        last_Brake = 1;
     
    }
  }
  else if(last_Brake == 1 && Brake == 0){
    if(B_min_LS_val != 0){
    digitalWrite( B_DIR_pin[0],LOW);
    digitalWrite( B_DIR_pin[1],HIGH);
    }
     else{
    digitalWrite( B_DIR_pin[0],LOW);
    digitalWrite( B_DIR_pin[1],LOW);  
        last_Brake = 0;
    
    }
  }
    else{
    digitalWrite( B_DIR_pin[0],LOW);
    digitalWrite( B_DIR_pin[1],LOW);      
    }


}
