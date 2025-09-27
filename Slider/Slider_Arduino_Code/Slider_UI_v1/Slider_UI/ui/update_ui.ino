// Global variable for battery level
int battery_level = 0;

// LVGL objects (assuming they are defined in a UI setup file)
extern lv_obj_t *uic_Header_battery;
extern lv_obj_t *uic_Header1_battery;
extern lv_obj_t *uic_Header2_battery;
extern lv_obj_t *uic_Header4_battery;

extern lv_obj_t *ui_Slider4;
extern lv_obj_t *uic_positionJM;
extern lv_obj_t *ui_positionCM;
extern lv_obj_t *ui_positionAB;

// LVGL button objects for different screens
extern lv_obj_t *ui_leftMove, *ui_rightMove;
extern lv_obj_t *ui_leftMoveAB, *ui_rightMoveAB_btn, *ui_SetA, *ui_SetB;
extern lv_obj_t *ui_time_btn, *ui_loop, *ui_start, *ui_stop;
extern lv_obj_t *ui_backA, *ui_backB_btn, *ui_save;
extern lv_obj_t *uic_homebtn, *ui_calibrate, *uic_left, *uic_right;

// LVGL slider objects for manual mode
extern lv_obj_t *speedSet_slider, *positionSlider;





void read_value() {
  // Create Jog mode screen
  if (lv_obj_has_state(ui_leftMove, LV_STATE_PRESSED)) {
      Serial.println("M JM , S 100 , C L.");
      Serial1.println("M JM , S 100 , C L.");
    
            // Your code here (send command, toggle relay, etc.)
    }
  else if (lv_obj_has_state(ui_rightMove, LV_STATE_PRESSED)) {
      Serial.println("M JM , S 100 , C R.");
      Serial1.println("M JM , S 100 , C R.");
            // Your code here (send command, toggle relay, etc.)
    }
  
  // lv_obj_add_event_cb(ui_leftMoveAB, button_event_cb, LV_EVENT_CLICKED, (void *)"Left Move AB");
  //lv_obj_add_event_cb(ui_SetA, button_event_cb, LV_EVENT_CLICKED, NULL);




  // // Create AB shuttle screen
  // lv_obj_add_event_cb(leftMoveAB_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Left Move AB");
  // lv_obj_add_event_cb(rightMoveAB_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Right Move AB");

  //lv_obj_add_event_cb(ui_SetA, button_event_cb, LV_EVENT_CLICKED, (void *)"Set A");
  //lv_obj_add_event_cb(ui_SetB, button_event_cb, LV_EVENT_CLICKED, (void *)"Set B");

  // lv_obj_add_event_cb(time_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Time");
  // lv_obj_add_event_cb(loop_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Loop");
  // lv_obj_add_event_cb(start_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Start");
  // lv_obj_add_event_cb(stop_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Stop");
  // lv_obj_add_event_cb(backA_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Back A");
  // lv_obj_add_event_cb(backB_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Back B");
  // lv_obj_add_event_cb(save_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Save");

  // // Create Calibrate screen
  
    if (lv_obj_has_state(ui_calibrate, LV_STATE_PRESSED)) {
        Serial.println("M CA , M C.");
        Serial1.println("M CA , M C.");
            // Your code here (send command, toggle relay, etc.)
    }
    if (lv_obj_has_state(uic_homebtn, LV_STATE_PRESSED)) {
        Serial.println("Calibrate button pressed");
        // Your code here (send command, toggle relay, etc.)
    }
    if (lv_obj_has_state(uic_right, LV_STATE_PRESSED)) {
        Serial.println("Calibrate button pressed");
        // Your code here (send command, toggle relay, etc.)
    }
    if (lv_obj_has_state(uic_left, LV_STATE_PRESSED)) {
        Serial.println("Calibrate button pressed");
        // Your code here (send command, toggle relay, etc.)
    }


  // // Create Manual mode sliders

  // // Create Manual mode sliders
  // lv_obj_add_event_cb(ui_speedSet, Button_evt_handler, LV_EVENT_VALUE_CHANGED, (void *)"Speed Set");
  // lv_obj_add_event_cb(ui_positionSlider, slider_event_cb, LV_EVENT_VALUE_CHANGED, (void *)"Position Slider");


}

void update_ui() {
  battery_level = Battery; // Returns 0–1023
  int scaled_battery = map(battery_level, 0, 1023, 0, 100); // Map to 0–100

  lv_slider_set_value(uic_Header_battery, scaled_battery, LV_ANIM_ON);
  lv_slider_set_value(uic_Header1_battery, scaled_battery, LV_ANIM_ON);
  lv_slider_set_value(uic_Header2_battery, scaled_battery, LV_ANIM_ON);
  lv_slider_set_value(uic_Header4_battery, scaled_battery, LV_ANIM_ON);

  setSlider();

  lv_timer_handler();
}

void setSlider() {
  if(TS> 0){
  lv_slider_set_range(ui_positionPM, 0, TS);
  lv_slider_set_range(ui_positionJM, 0, TS);
  lv_slider_set_range(ui_positionCM, 0, TS);
  lv_slider_set_range(ui_positionAB, 0, TS);

  lv_slider_set_value(uic_positionJM, position, LV_ANIM_ON);
  lv_slider_set_value(ui_Slider4, position, LV_ANIM_ON);
  lv_slider_set_value(uic_positionCM, position, LV_ANIM_ON);
  lv_slider_set_value(uic_positionAB, position, LV_ANIM_ON);
  }

}

void logs() {
  Serial.println("UI Updated");
}





