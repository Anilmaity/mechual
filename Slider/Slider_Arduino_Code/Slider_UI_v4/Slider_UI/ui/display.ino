

void display_Setup(){

  // Initialize display
  Display.begin();
// Initialize LVGL early
  lv_init();

 
  // Initialize touch
   TouchDetector.begin();
  // Initialize LVGL and UI

    ui_init();


  lv_slider_set_range(ui_speedSet, 10, (int)round((float)STEPPER_SPEED / DIVIDER_STEP_MM));
  lv_slider_set_range(uic_speedSetAB, 10, (int)round((float)STEPPER_SPEED / DIVIDER_STEP_MM));
  lv_slider_set_range(uic_speedSetBA, 10, (int)round((float)STEPPER_SPEED / DIVIDER_STEP_MM));
  
  lv_slider_set_range(uic_Header_battery, 225, 250); // 
  lv_slider_set_range(uic_Header1_battery, 225, 250);
  lv_slider_set_range(uic_Header2_battery, 225, 250);
  lv_slider_set_range(uic_Header4_battery, 225, 250);


  char buffer[16];
  snprintf(buffer, sizeof(buffer), "%d mm", 0);
  lv_label_set_text(uic_SetAValue1, buffer);

  snprintf(buffer, sizeof(buffer), "%d mm", 0);
  lv_label_set_text(uic_SetBValue1, buffer);


  lv_obj_add_flag(ui_Spinner2, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_Label9, LV_OBJ_FLAG_HIDDEN);
  update_ui();

  delay(1000);


  while(!calibrated)
  {

  lv_obj_clear_flag(ui_Spinner2, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_Label9, LV_OBJ_FLAG_HIDDEN);
  update_ui();
  }

  lv_scr_load(ui_Home); // Replace ui_Screen1 with the desired screen name from ui.h

}