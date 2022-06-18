int DIR_pin = 2;
int PWM_pin = 3;
int ENC_A = 20;
int ENC_B = 21;
int A =0;
int B=0;
int Current_position =0;
int position_error = 0;
void setup()
{
  Serial.begin(115200);

  pinMode(DIR_pin, OUTPUT);
  pinMode(PWM_pin, OUTPUT);
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENC_A), decode_rotation, RISING);

}

void loop()
{
  delay(100);
}

void decode_rotation() {
   A = digitalRead(ENC_A);
   B = digitalRead(ENC_B);
   Serial.println(String(Current_position)+" "+ String(A)+" "+String(B));


  if (A == 1 && B == 0) {
    Current_position += 1;

  }
  else if (A == 1 && B == 1) {
    Current_position -= 1;

  }

}
