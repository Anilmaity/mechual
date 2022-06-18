void drive_mode(){

  if(Drive_mode== 'N'){
    digitalWrite(Neutral_pin,LOW);
        digitalWrite(Reverse_pin,HIGH);

  }
  else if(Drive_mode == 'F'){
        digitalWrite(Neutral_pin,HIGH);
            digitalWrite(Reverse_pin,HIGH);


  }
  else{
        digitalWrite(Neutral_pin,HIGH);
            digitalWrite(Reverse_pin,LOW);


  }
}
