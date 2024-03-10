

void intrupptsetup(){

  pinMode( 2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), ch1, RISING);

  pinMode( 3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), ch2, RISING);

  pinMode( 18, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(18), ch3, RISING);

  pinMode( 19, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(19), ch4, RISING);
  
}



void ch1() { attachInterrupt(digitalPinToInterrupt(2), c1, FALLING); CT1 = micros();} 
void c1() {
  attachInterrupt(digitalPinToInterrupt(2), ch1, RISING);
  tempcha1 = micros()-CT1;  
  if (tempcha1 > 900 && tempcha1 < 2100 ) {cha1 = tempcha1 ;}
  else {cha1 = cha1 ; }}


void ch2() { attachInterrupt(digitalPinToInterrupt(3), c2, FALLING); CT2 = micros();} 
void c2() {
  attachInterrupt(digitalPinToInterrupt(3), ch2, RISING); 
  tempcha2 = micros()-CT2;  
  if (tempcha2 > 900 && tempcha2 < 2100 ) {cha2 = tempcha2 ;}
  else {cha2 = cha2 ; }}




void ch3() {attachInterrupt(digitalPinToInterrupt(18), c3, FALLING);CT3 = micros();} 
void c3() {
  attachInterrupt(digitalPinToInterrupt(18), ch3, RISING);
  tempcha3 = micros()-CT3;
  if (tempcha3 > 900 && tempcha3 < 2100 ) {cha3 = tempcha3 ;}
  else {cha3 = cha3 ; }

  }


void ch4() { attachInterrupt(digitalPinToInterrupt(19), c4, FALLING); CT4 = micros();} 
void c4() {
  attachInterrupt(digitalPinToInterrupt(19), ch4, RISING);
  tempcha4 = micros()-CT4;
   if (tempcha4 > 900 && tempcha4 < 2100 ) {cha4 = tempcha4 ;}
  else {cha4 = cha4 ; }}


