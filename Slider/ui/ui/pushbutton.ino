
bool rightPrev = true;
bool leftPrev = true;
bool idlePrev = false;  // track if idle was already printed
bool both_on = false;




void pushbtn(){

  bool rightNow = !digitalRead(RIGHT_BTN);
  bool leftNow = !digitalRead(LEFT_BTN);


  if ((leftNow && rightNow )){
    both_on = true;
  }



 if (leftNow && leftNow != leftPrev) {

      Serial1.println(String("M JM , S ") + speedVal + " , C R.");
      Serial.println(String("M JM , S ") + speedVal + " , C R.");
      idlePrev = false;
    }
    // Your code here (send command, toggle relay, etc.)

  if (rightNow && rightNow != rightPrev) {

      Serial.println(String("M JM , S ") + speedVal + " , C L.");
      Serial1.println(String("M JM , S ") + speedVal + " , C L.");
      idlePrev = false;
    }



  if ((!rightNow && !leftNow) && !idlePrev) {
    Serial.println(String("M JM , S ") + speedVal + " , C I.");
    Serial1.println(String("M JM , S ") + speedVal + " , C I.");
    idlePrev = true;
  }

  if(both_on == true && (!rightNow || !leftNow)){
    both_on = false;
    Serial.println(String("M JM , S ") + speedVal + " , C I.");
    Serial1.println(String("M JM , S ") + speedVal + " , C I.");
    
  }



  leftPrev = leftNow;
  rightPrev = rightNow;

}
