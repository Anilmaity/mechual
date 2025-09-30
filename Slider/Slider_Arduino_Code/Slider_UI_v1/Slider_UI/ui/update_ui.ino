// Global variable for battery level
int battery_level = 0;

// LVGL objects (assuming they are defined in a UI setup file)
extern lv_obj_t *uic_Header_battery;
extern lv_obj_t *uic_Header1_battery;
extern lv_obj_t *uic_Header2_battery;
extern lv_obj_t *uic_Header4_battery;

extern lv_obj_t *uic_PositionMPM;
extern lv_obj_t *uic_positionJM;
extern lv_obj_t *ui_positionCM;
extern lv_obj_t *ui_positionAB;

// LVGL button objects for different screens
// Manual Screen Object
extern lv_obj_t *ui_leftMove, *ui_rightMove, *ui_Button13;

//AB OBJECT
extern lv_obj_t *uic_SetA, *uic_SetB, *uic_SetAValue1, *uic_SetBValue1, *uic_speedSetAB, *uic_speedSetBA, *ui_SpeedValue3, *uic_Plusloop, *uic_Minusloop, *uic_PlusBA, *uic_MinusBA;
extern lv_obj_t *uic_SpeedValueBA, *uic_Start, *uic_Stop;
extern lv_obj_t *ui_BackA, *ui_BackA, *ui_save, *ui_SwitchCS, *ui_Panel4;

//CALIBRATION SCREEN OBJET
extern lv_obj_t *uic_homebtn, *ui_calibrate, *uic_left, *uic_right;
extern lv_obj_t *ui_rightlimit, *ui_leftlimit;

// LVGL slider objects for manual mode
extern lv_obj_t *speedSet_slider, *positionSlider;

int PointA = 0;
int PointB = 0;
int AtoBSpeed = 0;
int BtoASpeed = 0;
int sameSpeedAB = false;

int ABLoop = 2;

bool btn_rightPrev = true;
bool btn_leftPrev = true;
bool btn_idlePrev = false;  // track if idle was already printed
long int CA_buttonClick = 0;
long int AB_buttonClick = 0;
long int PM_buttonClick = 0;


void read_manualmode() {

  bool btn_rightNow = false;
  bool btn_leftNow = false;


  if (lv_obj_has_state(ui_leftMove, LV_STATE_PRESSED)) {
    btn_leftNow = true;
    if (btn_leftNow && btn_leftNow != btn_leftPrev) {

      Serial1.println(String("M JM , S ") + speedVal + " , C R.");
      Serial.println(String("M JM , S ") + speedVal + " , C R.");
      btn_idlePrev = false;
    }
    // Your code here (send command, toggle relay, etc.)
  } else if (lv_obj_has_state(ui_rightMove, LV_STATE_PRESSED)) {
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
    if (millis() - PM_buttonClick > 400) {
      sendPositionMode(posVal,speedVal);
      // Your code here (send command, toggle relay, etc.)
      PM_buttonClick = millis();
    }
  }



  // // Create Manual mode sliders

  speedVal = lv_slider_get_value(ui_speedSet) * DIVIDER_STEP_MM;
  posVal = int(lv_slider_get_value(ui_positionSlider) * DIVIDER_STEP_MM);
}




void read_ABMode() {


  // // Create AB shuttle screen

  if (lv_obj_has_state(uic_SetA, LV_STATE_PRESSED)) {
    if (millis() - AB_buttonClick > 200) {
      PointA = position;
      char buffer[16];
      snprintf(buffer, sizeof(buffer), "%d mm", (int)(position / DIVIDER_STEP_MM));
      lv_label_set_text(uic_SetAValue1, buffer);
      //lv_label_set_text(uic_SetAValue1, String(int(position / DIVIDER_STEP_MM) + " mm").c_str());
      AB_buttonClick = millis();
    }
  }
  if (lv_obj_has_state(uic_SetB, LV_STATE_PRESSED)) {
    if (millis() - AB_buttonClick > 200) {
      PointB = position;
      char buffer[16];
      snprintf(buffer, sizeof(buffer), "%d mm", (int)(position / DIVIDER_STEP_MM));
      lv_label_set_text(uic_SetBValue1, buffer);
      AB_buttonClick = millis();
    }


    // Your code here (send command, toggle relay, etc.)
  }


  if (lv_obj_has_state(ui_BackA, LV_STATE_PRESSED)) {
    if (millis() - AB_buttonClick > 200) {
      sendPositionMode(PointA,speedVal);

      AB_buttonClick = millis();
    }
  }

  if (lv_obj_has_state(ui_BackB, LV_STATE_PRESSED)) {
    if (millis() - AB_buttonClick > 200) {
      sendPositionMode(PointB,speedVal);

      AB_buttonClick = millis();
    }
  }



  if (lv_obj_has_state(uic_Start, LV_STATE_PRESSED)) {
    // Serial1.println(String("M AB , A ") + PointA + " B " + PointB + " , ");

    if (millis() - AB_buttonClick > 500) {
      if (sameSpeedAB) {

        BtoASpeed = AtoBSpeed;
      }

      sendABShuttleMode(PointA, PointB, AtoBSpeed, BtoASpeed, ABLoop);

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
  }



  if (lv_obj_has_state(uic_PlusBA, LV_STATE_PRESSED)) {

    if (millis() - AB_buttonClick > 200) {
      BtoASpeed = BtoASpeed + 1;
      char buffer[16];
      lv_slider_set_value(uic_speedSetBA, int(BtoASpeed / DIVIDER_STEP_MM), LV_ANIM_ON);
      snprintf(buffer, sizeof(buffer), "%d mm/s", (int)(BtoASpeed / DIVIDER_STEP_MM));
      lv_label_set_text(uic_SpeedValueBA, buffer);

      //lv_label_set_text(uic_SpeedValueBA, (String(BtoASpeed) + " mm/s").c_str());

      AB_buttonClick = millis();
    }

    // Your code here (send command, toggle relay, etc.)
  }
  if (lv_obj_has_state(uic_MinusBA, LV_STATE_PRESSED)) {
    if (millis() - AB_buttonClick > 200) {
      if (int(BtoASpeed / DIVIDER_STEP_MM) - 1 >= 1) {

        BtoASpeed = BtoASpeed - 1;
      char buffer[16];

        lv_slider_set_value(uic_speedSetBA, int(BtoASpeed / DIVIDER_STEP_MM), LV_ANIM_ON);
        snprintf(buffer, sizeof(buffer), "%d mm/s", (int)(BtoASpeed / DIVIDER_STEP_MM));
        lv_label_set_text(uic_SpeedValueBA, buffer);
      }
      AB_buttonClick = millis();
    }

    // Your code here (send command, toggle relay, etc.)
  }





  // LOOPS
  if (lv_obj_has_state(uic_Plusloop, LV_STATE_PRESSED)) {

    if (millis() - AB_buttonClick > 200) {
      ABLoop = ABLoop + 1;
      lv_label_set_text(ui_SpeedValue3, String(ABLoop).c_str());

      AB_buttonClick = millis();
    }

    // Your code here (send command, toggle relay, etc.)
  }
  if (lv_obj_has_state(uic_Minusloop, LV_STATE_PRESSED)) {
    if (millis() - AB_buttonClick > 200) {
      if (ABLoop - 1 >= 1) {
        ABLoop = ABLoop - 1;
        lv_label_set_text(ui_SpeedValue3, String(ABLoop).c_str());
      }
      AB_buttonClick = millis();
    }

    // Your code here (send command, toggle relay, etc.)
  }


  // // Create Manual mode sliders
  AtoBSpeed = lv_slider_get_value(uic_speedSetAB) * DIVIDER_STEP_MM;
  BtoASpeed = lv_slider_get_value(uic_speedSetBA) * DIVIDER_STEP_MM;
}

void read_calibrate() {

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
        sendPositionMode(int(TS / 2),speedVal);
        CA_buttonClick = millis();
    }

    // Your code here (send command, toggle relay, etc.)
  }
  if (lv_obj_has_state(uic_left, LV_STATE_PRESSED)) {

    if (millis() - CA_buttonClick > 200) {
        sendPositionMode(0,speedVal);
      CA_buttonClick = millis();
    }

    // Your code here (send command, toggle relay, etc.)
  }
  if (lv_obj_has_state(uic_right, LV_STATE_PRESSED)) {
    if (millis() - CA_buttonClick > 200) {

        sendPositionMode(TS,speedVal);

      CA_buttonClick = millis();
    }

    // Your code here (send command, toggle relay, etc.)
  }








  bool temp_sameSpeedAB = lv_obj_has_state(ui_SwitchCS, LV_STATE_CHECKED);

  if (temp_sameSpeedAB && !sameSpeedAB) {
    if (millis() - CA_buttonClick > 200) {
      lv_obj_add_flag(ui_speedSetBA, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(uic_PlusBA, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(uic_MinusBA, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_Label5, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_Panel4, LV_OBJ_FLAG_HIDDEN);

      Serial.println("Switch on ");
      sameSpeedAB = true;
      CA_buttonClick = millis();
    }
  } else if (!temp_sameSpeedAB && sameSpeedAB) {
    if (millis() - CA_buttonClick > 200) {
      lv_obj_clear_flag(ui_speedSetBA, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(uic_PlusBA, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(uic_MinusBA, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_Label5, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_Panel4, LV_OBJ_FLAG_HIDDEN);

      Serial.println("Switch off ");
      sameSpeedAB = false;
      CA_buttonClick = millis();
    }
  }
}



void updatebattery() {
  battery_level = Battery;                                    // Returns 0–1023
  int scaled_battery = map(battery_level, 18.5, 21, 0, 100);  // Map to 0–100

  lv_slider_set_value(uic_Header_battery, scaled_battery, LV_ANIM_ON);
  lv_slider_set_value(uic_Header1_battery, scaled_battery, LV_ANIM_ON);
  lv_slider_set_value(uic_Header2_battery, scaled_battery, LV_ANIM_ON);
  lv_slider_set_value(uic_Header4_battery, scaled_battery, LV_ANIM_ON);
}

void update_ui() {

  setSlider();
  updatebattery();
  read_manualmode();
  read_ABMode();
  read_calibrate();

  lv_timer_handler();
}

void setSlider() {


  if (TS > 0) {
    lv_slider_set_range(uic_PositionMPM, 0, int(TS / DIVIDER_STEP_MM));
    lv_slider_set_range(ui_positionJM, 0, int(TS / DIVIDER_STEP_MM));
    lv_slider_set_range(ui_positionCM, 0, int(TS / DIVIDER_STEP_MM));
    lv_slider_set_range(ui_positionAB, 0, int(TS / DIVIDER_STEP_MM));
    lv_slider_set_range(ui_positionSlider, 0, int(TS / DIVIDER_STEP_MM));

    lv_slider_set_value(uic_positionJM, int(position / DIVIDER_STEP_MM), LV_ANIM_ON);
    lv_slider_set_value(uic_PositionMPM, int(position / DIVIDER_STEP_MM), LV_ANIM_ON);
    lv_slider_set_value(uic_positionCM, int(position / DIVIDER_STEP_MM), LV_ANIM_ON);
    lv_slider_set_value(uic_positionAB, int(position / DIVIDER_STEP_MM), LV_ANIM_ON);


    lv_label_set_text(ui_SliderPositionJM, String(int(position / DIVIDER_STEP_MM)).c_str());
    lv_label_set_text(ui_SliderPositionPM, String(int(position / DIVIDER_STEP_MM)).c_str());
    lv_label_set_text(ui_SliderPositionCA, String(int(position / DIVIDER_STEP_MM)).c_str());
    lv_label_set_text(ui_SliderPositionAB, String(int(position / DIVIDER_STEP_MM)).c_str());
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

  //6BB7B1
}





void logs() {
  Serial.println("UI Updated");
}
