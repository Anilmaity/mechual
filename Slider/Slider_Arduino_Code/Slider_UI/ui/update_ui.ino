int battery_level = 0 ;

void update_ui() {
  
  battery_level = analogRead(A0);

  lv_slider_set_value(uic_Header_battery, battery_level/255, LV_ANIM_ON);  // Set slider to 30 with smooth animation


  



  lv_timer_handler();
  delay(50);
}



void logs() {

  Serial.println();

}
