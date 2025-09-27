// Global variable for battery level
int battery_level = 0;

// LVGL objects (assuming they are defined in a UI setup file)
extern lv_obj_t *uic_Header_battery;
extern lv_obj_t *uic_Header1_battery;
extern lv_obj_t *uic_Header2_battery;
extern lv_obj_t *uic_Header4_battery;

extern lv_obj_t *uic_positionPM;
extern lv_obj_t *uic_positionJM;
extern lv_obj_t *uic_positionCM;
extern lv_obj_t *uic_positionAB;

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
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e); // Cast void* to lv_obj_t*
  const char *btn_name = (const char *)lv_obj_get_user_data(btn); // Cast void* to const char*
  Serial.print("Button Pressed: ");
  Serial.println(btn_name);
}

// Slider event callback
static void slider_event_cb(lv_event_t *e) {
  lv_obj_t *slider = (lv_obj_t *)lv_event_get_target(e); // Cast void* to lv_obj_t*
  const char *slider_name = (const char *)lv_obj_get_user_data(slider); // Cast void* to const char*
  int32_t value = lv_slider_get_value(slider);
  Serial.print("Slider ");
  Serial.print(slider_name);
  Serial.print(": ");
  Serial.print(value);
  Serial.println("%");
}

void screen_setup() {
  // Create Jog mode screen
  lv_obj_t *jog_screen = lv_obj_create(NULL);
  leftMove_btn = lv_btn_create(jog_screen);
  lv_obj_add_event_cb(leftMove_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Left Move");
  rightMove_btn = lv_btn_create(jog_screen);
  lv_obj_add_event_cb(rightMove_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Right Move");

  // Create AB shuttle screen
  lv_obj_t *ab_shuttle_screen = lv_obj_create(NULL);
  leftMoveAB_btn = lv_btn_create(ab_shuttle_screen);
  lv_obj_add_event_cb(leftMoveAB_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Left Move AB");
  rightMoveAB_btn = lv_btn_create(ab_shuttle_screen);
  lv_obj_add_event_cb(rightMoveAB_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Right Move AB");
  setA_btn = lv_btn_create(ab_shuttle_screen);
  lv_obj_add_event_cb(setA_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Set A");
  setB_btn = lv_btn_create(ab_shuttle_screen);
  lv_obj_add_event_cb(setB_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Set B");
  time_btn = lv_btn_create(ab_shuttle_screen);
  lv_obj_add_event_cb(time_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Time");
  loop_btn = lv_btn_create(ab_shuttle_screen);
  lv_obj_add_event_cb(loop_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Loop");
  start_btn = lv_btn_create(ab_shuttle_screen);
  lv_obj_add_event_cb(start_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Start");
  stop_btn = lv_btn_create(ab_shuttle_screen);
  lv_obj_add_event_cb(stop_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Stop");
  backA_btn = lv_btn_create(ab_shuttle_screen);
  lv_obj_add_event_cb(backA_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Back A");
  backB_btn = lv_btn_create(ab_shuttle_screen);
  lv_obj_add_event_cb(backB_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Back B");
  save_btn = lv_btn_create(ab_shuttle_screen);
  lv_obj_add_event_cb(save_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Save");

  // Create Calibrate screen
  lv_obj_t *calibrate_screen = lv_obj_create(NULL);
  home_btn = lv_btn_create(calibrate_screen);
  lv_obj_add_event_cb(home_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Home");
  calibrate_btn = lv_btn_create(calibrate_screen);
  lv_obj_add_event_cb(calibrate_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Calibrate");
  leftCal_btn = lv_btn_create(calibrate_screen);
  lv_obj_add_event_cb(leftCal_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Left Cal");
  rightCal_btn = lv_btn_create(calibrate_screen);
  lv_obj_add_event_cb(rightCal_btn, button_event_cb, LV_EVENT_CLICKED, (void *)"Right Cal");

  // Create Manual mode sliders
  lv_obj_t *manual_screen = lv_obj_create(NULL);
  speedSet_slider = lv_slider_create(manual_screen);
  lv_slider_set_range(speedSet_slider, 0, 100);
  lv_obj_add_event_cb(speedSet_slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, (void *)"Speed Set");
  positionSlider = lv_slider_create(manual_screen);
  lv_slider_set_range(positionSlider, 0, 100);
  lv_obj_add_event_cb(positionSlider, slider_event_cb, LV_EVENT_VALUE_CHANGED, (void *)"Position Slider");

  // Load the initial screen (e.g., Jog mode)
  lv_scr_load(jog_screen);
}

void update_ui() {
  battery_level = analogRead(A0); // Returns 0–1023
  int scaled_battery = map(battery_level, 0, 1023, 0, 100); // Map to 0–100

  lv_slider_set_value(uic_Header_battery, scaled_battery, LV_ANIM_ON);
  lv_slider_set_value(uic_Header1_battery, scaled_battery, LV_ANIM_ON);
  lv_slider_set_value(uic_Header2_battery, scaled_battery, LV_ANIM_ON);
  lv_slider_set_value(uic_Header4_battery, scaled_battery, LV_ANIM_ON);
}

void setSlider() {
  lv_slider_set_range(uic_positionPM, 0, 100);
  lv_slider_set_range(uic_positionJM, 0, 100);
  lv_slider_set_range(uic_positionCM, 0, 100);
  lv_slider_set_range(uic_positionAB, 0, 100);

  lv_slider_set_value(uic_positionPM, 50, LV_ANIM_ON);
  lv_slider_set_value(uic_positionJM, 50, LV_ANIM_ON);
  lv_slider_set_value(uic_positionCM, 50, LV_ANIM_ON);
  lv_slider_set_value(uic_positionAB, 50, LV_ANIM_ON);
}

void logs() {
  Serial.println("UI Updated");
}





