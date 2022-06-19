void drive_mode(){

  if(Drive_mode== 'N'){

        digitalWrite(Neutral_pin,HIGH);
            digitalWrite(Reverse_pin,LOW);
  }
  else if(Drive_mode == 'F'){
        digitalWrite(Neutral_pin,HIGH);
            digitalWrite(Reverse_pin,HIGH);


  }
  else{
            
    digitalWrite(Neutral_pin,LOW);
        digitalWrite(Reverse_pin,HIGH);


  }
}



void lightup_all(int r, int g, int b){
   for (int i = 0; i <= (NUM_LEDS - 1); i++) {
         leds[i] = CRGB(r, g, b);
   }
             FastLED.show();
   
}

void light_setup(){
    delay(2000);
     FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
     lightup_all(100,100,100);
     delay(100);
     
      
for(int h =0; h<=1; h++){
     pinMode(Head_light_pin, OUTPUT);
   pinMode(back_light_pin, OUTPUT);
   pinMode(side_light_pin, OUTPUT);
}
  
}

void lights()
{
//------------------back light---------------------------
if(back_light == 'F'){
        digitalWrite(back_light_pin[0],LOW);
  
}
else{
  digitalWrite(back_light_pin[0],HIGH);

}
//------------------------------Side light----------------------------
if(left_side_light == 'F'){
        digitalWrite(side_light_pin[0],LOW);
  
}
else{
  digitalWrite(side_light_pin[0],LOW);

}
if(right_side_light == 'F'){
        digitalWrite(side_light_pin[1],LOW);
  
}
else{
  digitalWrite(side_light_pin[1],LOW);

}

//-----------Head Light --------------------
if(Head_light_mode == 'U'){
        digitalWrite(Head_light_pin[0],LOW);
        digitalWrite(Head_light_pin[0],HIGH);

  
}
else if(Head_light_mode == 'D'){
        digitalWrite(Head_light_pin[0],LOW);
        digitalWrite(Head_light_pin[0],HIGH);  
}
else if(Head_light_mode == 'N'){
          digitalWrite(Head_light_pin[0],HIGH);
        digitalWrite(Head_light_pin[0],HIGH);
}
else{
            digitalWrite(Head_light_pin[0],LOW);
        digitalWrite(Head_light_pin[0],LOW);
}

  
}

void brake_animation()
{ 
  
  lightup_all(0,abs(int(Brake_Speed/15)),0);
  
  
}


void forward_animation()
{
  //----------------Green light for normal mode -----------
     
     if(drive_mode_change == true){
     for (int i = 0; i <= (NUM_LEDS - 1); i++) {
      for(int j = 0; j <= i; j++){
      leds[j] = CRGB(throttle,0,0 );
      }
          FastLED.show();
          delay(100);

    }
     }
     else{
        
      lightup_all(throttle,0,0);
    }  

    //---------------- blue and Green light for fast mode -----------
//     if(drive_mode_change == true){
//     for (int i = 0; i <= (NUM_LEDS - 1); i++) {
//      for(int j = 0; j <= i; j++){
//      leds[j] = CRGB(0, 0, throttle);
//      }
//          FastLED.show();
//          delay(5);
//
//    }
//     }
//     else{
//        
//           lightup_all(0,0,throttle);
//
//    }
}


void neutral_animation()
{
value = (millis() - neutral_light_start)/1000;
     
     if(drive_mode_change == true){
     for (int i = 0; i <= (NUM_LEDS - 1); i++) {
      for(int j = 0; j <= i; j++){
      leds[j] = CRGB(100, 100,100 );
      }
          FastLED.show();
          delay(100);

    }
     }
     else{
        if(fadding){
           lightup_all(int(value*3),int(value*1.00),int(value*2.0));
        }
        else{
           lightup_all(int(240-(value*3)),int(80-(value*1.00)),int(160-(value*2.0)));
        }
    }  

    if(value >= 80){
      neutral_light_start = millis();
      fadding = !fadding;
    }

   
}



void reverse_animation()
{
  //----------------Green light for normal mode -----------
     
     if(drive_mode_change == true){
     for (int i = 0; i <= (NUM_LEDS - 1); i++) {
      for(int j = 0; j <= i; j++){
      leds[j] = CRGB(0, 0,throttle );
      }
          FastLED.show();
          delay(100);

    }
     }
     else{
        
       lightup_all(0,0,throttle);

    }  

    //---------------- blue and Green light for fast mode -----------
//     if(drive_mode_change == true){
//     for (int i = 0; i <= (NUM_LEDS - 1); i++) {
//      for(int j = 0; j <= i; j++){
//      leds[j] = CRGB(throttle, 0, throttle);
//      }
//          FastLED.show();
//          delay(5);
//
//    }
//     }
//     else{
//        
//        lightup_all(throttle,throttle,0);
//
//    }
}


  
