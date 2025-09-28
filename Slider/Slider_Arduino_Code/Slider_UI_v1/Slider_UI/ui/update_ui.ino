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
// Manual Screen Object
extern lv_obj_t *ui_leftMove, *ui_rightMove , *ui_Button13;

//AB OBJECT
extern lv_obj_t *uic_leftMoveAB, *uic_rightMoveAB, *ui_SetA, *ui_SetB, *ui_Label29, *ui_Label32;
extern lv_obj_t *ui_time_btn, *ui_loop, *uic_Start, *uic_Stop;
extern lv_obj_t *ui_BackA, *ui_BackA, *ui_save;

//CALIBRATION SCREEN OBJET
extern lv_obj_t *uic_homebtn, *ui_calibrate, *uic_left, *uic_right;
extern lv_obj_t *ui_rightlimit, *ui_leftlimit;

// LVGL slider objects for manual mode
extern lv_obj_t *speedSet_slider, *positionSlider;

int PointA = 0;
int PointB = 0;
int ABTime = 1;
int ABLoop = 1;

bool btn_rightPrev = true;
bool btn_leftPrev = true;
bool btn_idlePrev = false;  // track if idle was already printed
long int CA_buttonClick = 0;
long int AB_buttonClick = 0;
long int PM_buttonClick = 0;



void read_value() {
  // Create Jog mode screen

  bool btn_rightNow = false;
  bool btn_leftNow = false;


  if (lv_obj_has_state(ui_leftMove, LV_STATE_PRESSED) || lv_obj_has_state(uic_leftMoveAB, LV_STATE_PRESSED)) {
    btn_leftNow = true;
    if (btn_leftNow && btn_leftNow != btn_leftPrev) {

      Serial1.println(String("M JM , S ") + speedVal + " , C R.");
      Serial.println(String("M JM , S ") + speedVal + " , C R.");
      btn_idlePrev = false;
    }
    // Your code here (send command, toggle relay, etc.)
  } else if (lv_obj_has_state(ui_rightMove, LV_STATE_PRESSED) || lv_obj_has_state(uic_rightMoveAB, LV_STATE_PRESSED)) {
    btn_rightNow = true;
    if (btn_rightNow && btn_rightNow != btn_rightPrev) {

      Serial.println(String("M JM , S ") + speedVal + " , C L.");
      Serial1.println(String("M JM , S ") + speedVal + " , C L.");
      btn_idlePrev = false;
    }
    // Your code here (send command, toggle relay, etc.)
  } else if (!btn_rightNow && !btn_leftNow && !btn_idlePrev) {
    Serial.println(String("M JM , S ") + speedVal + " , C I.");
    Serial1.println(String("M JM , S ") + speedVal + " , C I.");
    btn_idlePrev = true;
  }

  btn_leftPrev = btn_leftNow;
  btn_rightPrev = btn_rightNow;

  // Create Position mode screen

  if (lv_obj_has_state(ui_Button13, LV_STATE_PRESSED)) {
    if(millis() - PM_buttonClick > 400){
    Serial1.println(String("M PM , P ") + posVal + ".");
    Serial.println(String("M PM , P ") + posVal + ".");
          // Your code here (send command, toggle relay, etc.)
    PM_buttonClick = millis();
    }

  }


  // lv_obj_add_event_cb(ui_leftMoveAB, button_event_cb, LV_EVENT_CLICKED, (void *)"Left Move AB");
  //lv_obj_add_event_cb(ui_SetA, button_event_cb, LV_EVENT_CLICKED, NULL);




  // // Create AB shuttle screen

  if (lv_obj_has_state(ui_SetA, LV_STATE_PRESSED)) {
    if (millis() - AB_buttonClick > 200) {
      PointA = position;
      lv_label_set_text(ui_Label29, String(position).c_str());
      AB_buttonClick = millis();
    }
  }
  if (lv_obj_has_state(ui_SetB, LV_STATE_PRESSED)) {
    if (millis() - AB_buttonClick > 200) {
      PointB = position;
      lv_label_set_text(ui_Label32, String(position).c_str());
      AB_buttonClick = millis();
    }


    // Your code here (send command, toggle relay, etc.)
  }


  if (lv_obj_has_state(ui_BackA, LV_STATE_PRESSED)) {
    if (millis() - AB_buttonClick > 200) {
      Serial.println(String("M PM , P ") + PointA + ".");

      AB_buttonClick = millis();
    }
  }
  if (lv_obj_has_state(ui_BackB, LV_STATE_PRESSED)) {
    if (millis() - AB_buttonClick > 200) {
      Serial.println(String("M PM , P ") + PointB + ".");
      AB_buttonClick = millis();
    }


  }



  if (lv_obj_has_state(uic_Start, LV_STATE_PRESSED)) {
    // Serial1.println(String("M AB , A ") + PointA + " B " + PointB + " , ");

  if (millis() - AB_buttonClick > 500) {
    sendABShuttleMode(PointA, PointB, ABTime, ABLoop);

      AB_buttonClick = millis();
    }


    // Your code here (send command, toggle relay, etc.)
  }

  if (lv_obj_has_state(uic_Stop, LV_STATE_PRESSED)) {

      if (millis() - AB_buttonClick > 500) {

    Serial.println(String("M JM , S ") + speedVal + " , C I.");
    Serial1.println(String("M JM , S ") + speedVal + " , C I.");

      AB_buttonClick = millis();
    }
    
    // Your code here (send command, toggle relay, etc.)
  }



  // lv_obj_add_event_cb(time_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Time");
  // lv_obj_add_event_cb(loop_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Loop");
  // lv_obj_add_event_cb(start_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Start");
  // lv_obj_add_event_cb(stop_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Stop");
  // lv_obj_add_event_cb(backA_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Back A");
  // lv_obj_add_event_cb(backB_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Back B");
  // lv_obj_add_event_cb(save_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Save");

  // // Create Calibrate screen

  if (lv_obj_has_state(ui_calibrate, LV_STATE_PRESSED)) {
    if (millis() - CA_buttonClick > 200) {
      Serial.println("M CA , M C.");
      Serial1.println("M CA , M C.");
      CA_buttonClick = millis();
    }

    // Your code here (send command, toggle relay, etc.)
  }
  if (lv_obj_has_state(uic_homebtn, LV_STATE_PRESSED)) {

    if (millis() - CA_buttonClick > 200) {
      Serial1.println(String("M PM , P ") + int(TS/2) + ".");
      Serial.println(String("M PM , P ") + int(TS/2) + ".");
      CA_buttonClick = millis();
    }

    // Your code here (send command, toggle relay, etc.)
  }
  if (lv_obj_has_state(uic_right, LV_STATE_PRESSED)) {

    if (millis() - CA_buttonClick > 200) {
      Serial1.println(String("M PM , P 0."));
      Serial.println(String("M PM , P 0."));
      CA_buttonClick = millis();
    }

    // Your code here (send command, toggle relay, etc.)
  }
  if (lv_obj_has_state(uic_left, LV_STATE_PRESSED)) {
    if (millis() - CA_buttonClick > 200) {
      Serial1.println(String("M PM , P ") + TS + ".");
      Serial.println(String("M PM , P ") + TS + ".");
      CA_buttonClick = millis();
    }

    // Your code here (send command, toggle relay, etc.)
  }




  // // Create Manual mode sliders
  speedVal = lv_slider_get_value(ui_speedSet);
  posVal = lv_slider_get_value(ui_positionSlider);
}

void update_ui() {
  battery_level = Battery;                                    // Returns 0–1023
  int scaled_battery = map(battery_level, 18, 20.4, 0, 100);  // Map to 0–100

  lv_slider_set_value(uic_Header_battery, scaled_battery, LV_ANIM_ON);
  lv_slider_set_value(uic_Header1_battery, scaled_battery, LV_ANIM_ON);
  lv_slider_set_value(uic_Header2_battery, scaled_battery, LV_ANIM_ON);
  lv_slider_set_value(uic_Header4_battery, scaled_battery, LV_ANIM_ON);


  lv_label_set_text(ui_SliderPositionJM, String(position).c_str());
  lv_label_set_text(ui_SliderPositionPM, String(position).c_str());
  lv_label_set_text(ui_SliderPositionCA, String(position).c_str());
  lv_label_set_text(ui_SliderPositionAB, String(position).c_str());

  setSlider();




  lv_timer_handler();
}

void setSlider() {
  if (TS > 0) {
    lv_slider_set_range(ui_positionPM, TS, 0);
    lv_slider_set_range(ui_positionJM, TS, 0);
    lv_slider_set_range(ui_positionCM, TS, 0);
    lv_slider_set_range(ui_positionAB, TS, 0);
    lv_slider_set_range(ui_positionSlider, TS, 0);

    lv_slider_set_value(uic_positionJM, position, LV_ANIM_ON);
    lv_slider_set_value(ui_Slider4, position, LV_ANIM_ON);
    lv_slider_set_value(uic_positionCM, position, LV_ANIM_ON);
    lv_slider_set_value(uic_positionAB, position, LV_ANIM_ON);
  }

  if (limitright) {
    lv_obj_set_style_bg_color(ui_rightlimit, lv_color_hex(0x00FF00), LV_PART_MAIN | LV_STATE_DEFAULT);

  } else {
    lv_obj_set_style_bg_color(ui_rightlimit, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  }
  if (limitleft) {
    lv_obj_set_style_bg_color(ui_leftlimit, lv_color_hex(0x00FF00), LV_PART_MAIN | LV_STATE_DEFAULT);

  } else {
    lv_obj_set_style_bg_color(ui_leftlimit, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  }
}

void logs() {
  Serial.println("UI Updated");
}
