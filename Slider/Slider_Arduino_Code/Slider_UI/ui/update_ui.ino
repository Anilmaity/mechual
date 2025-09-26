
// Global variable for battery level
int battery_level = 0;

// LVGL objects (assuming they are defined in a UI setup file)
extern lv_obj_t *uic_Header_battery;
extern lv_obj_t *uic_Header1_battery;
extern lv_obj_t *uic_Header2_battery;
extern lv_obj_t *uic_Header4_battery;

// LVGL button objects for different screens
static lv_obj_t *leftMove_btn, *rightMove_btn;
static lv_obj_t *leftMoveAB_btn, *rightMoveAB_btn, *setA_btn, *setB_btn;
static lv_obj_t *time_btn, *loop_btn, *start_btn, *stop_btn;
static lv_obj_t *backA_btn, *backB_btn, *save_btn;
static lv_obj_t *home_btn, *calibrate_btn, *leftCal_btn, *rightCal_btn;

// LVGL slider objects for manual mode
static lv_obj_t *speedSet_slider, *positionSlider;


// Button event callback
static void button_event_cb(lv_event_t *e) {
  lv_obj_t *btn = lv_event_get_target(e);
  const char *btn_name = lv_obj_get_user_data(btn); // Assuming button names stored as user data
  Serial.print("Button Pressed: ");
  Serial.println(btn_name);
}

// Slider event callback
static void slider_event_cb(lv_event_t *e) {
  lv_obj_t *slider = lv_event_get_target(e);
  const char *slider_name = lv_obj_get_user_data(slider); // Assuming slider names stored as user data
  int32_t value = lv_slider_get_value(slider);
  Serial.print("Slider ");
  Serial.print(slider_name);
  Serial.print(": ");
  Serial.print(value);
  Serial.println("%");
}


void screen_setup(){
  // Setup LVGL buttons with event callbacks (example, assuming buttons are created)
  // Jog mode screen buttons
  leftMove_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(leftMove_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Left Move");
  rightMove_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(rightMove_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Right Move");

  // AB shuttle screen buttons
  leftMoveAB_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(leftMoveAB_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Left Move AB");
  rightMoveAB_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(rightMoveAB_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Right Move AB");
  setA_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(setA_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Set A");
  setB_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(setB_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Set B");
  time_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(time_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Time");
  loop_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(loop_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Loop");
  start_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(start_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Start");
  stop_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(stop_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Stop");
  backA_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(backA_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Back A");
  backB_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(backB_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Back B");
  save_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(save_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Save");

  // Calibrate screen buttons
  home_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(home_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Home");
  calibrate_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(calibrate_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Calibrate");
  leftCal_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(leftCal_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Left Cal");
  rightCal_btn = lv_btn_create(lv_scr_act());
  lv_obj_add_event_cb(rightCal_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Right Cal");

  // Setup LVGL sliders with event callbacks
  speedSet_slider = lv_slider_create(lv_scr_act());
  lv_slider_set_range(speedSet_slider, 0, 100);
  lv_obj_add_event_cb(speedSet_slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, (void *)"Speed Set");
  positionSlider = lv_slider_create(lv_scr_act());
  lv_slider_set_range(positionSlider, 0, 100);
  lv_obj_add_event_cb(positionSlider, slider_event_cb, LV_EVENT_VALUE_CHANGED, (void *)"Position Slider");
}




void update_ui() {
  
  battery_level = analogRead(A0);

  lv_slider_set_value(uic_Header_battery, battery_level/255, LV_ANIM_ON);  // Set slider to 30 with smooth animation
  lv_slider_set_value(uic_Header1_battery, battery_level/255, LV_ANIM_ON);  // Set slider to 30 with smooth animation
  lv_slider_set_value(uic_Header2_battery, battery_level/255, LV_ANIM_ON);  // Set slider to 30 with smooth animation
  lv_slider_set_value(uic_Header4_battery, battery_level/255, LV_ANIM_ON);  // Set slider to 30 with smooth animation


  



  lv_timer_handler();
  delay(50);
}

// void readButtonValue(){

// //Jog mode screen Buttons
// //leftMove
// //rightMove

// //AB shuttle Screen Buttons
// //leftMoveAB
// //rightMoveAB
// //SetA
// //SetB
// //Time
// //Loop
// //Start
// //Stop
// //BackA
// //BackB
// //Save


// // Calibrate Screen
// // Home
// // Calibrate
// // Left
// // Right


// }


// void readSliderValue(){

// //ManualMode
// //speedSet
// //positionSlider


// }

void setSlider(){

//Manual Mode
//positionPM
//positionJM
//positionCM
//positionAB

 lv_slider_set_value(positionPM, 50, LV_ANIM_ON);
  lv_slider_set_value(positionJM, 50, LV_ANIM_ON);
  lv_slider_set_value(positionCM, 50, LV_ANIM_ON);
  lv_slider_set_value(positionAB, 50, LV_ANIM_ON);


}




void logs() {

  Serial.println();

}
