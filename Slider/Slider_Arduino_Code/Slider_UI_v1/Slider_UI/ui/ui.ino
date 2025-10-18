#include "Arduino_H7_Video.h"
#include "Arduino_GigaDisplayTouch.h"
#include "lvgl.h"
#include <ui.h>
#include <math.h> // Include math.h for round()

#include <Arduino_GigaDisplay.h>

//Create backlight object
GigaDisplayBacklight backlight;

#include "Arduino_GigaDisplay_GFX.h"





Arduino_H7_Video Display(480, 800, GigaDisplayShield);  //( 800, 480, GigaDisplayShield );
Arduino_GigaDisplayTouch TouchDetector;

  GigaDisplay_GFX display;


#define RIGHT_BTN A0
#define LEFT_BTN A1
#define STEPPER_SPEED 4166   // 1200. // max 4800
#define DIVIDER_STEP_MM 33.33 // 400 /(3*20)



// LENGTH  840  mm    20 thread pully , 3 mm pitch  400 microstepping  = 400 /(3*20)   // 0.15 mm precision

long position = 50;
float Battery = 20;
bool emergency = false;
bool limitright = false;
bool limitleft = false;
long TS = 0;


int speedVal = 10;
int posVal = 0;
bool calibrated = false;

extern lv_obj_t *ui_Spinner2, *ui_Label9;


void calibrate(){

  lv_obj_add_flag(ui_Spinner2, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_Label9, LV_OBJ_FLAG_HIDDEN);
  update_ui();

  delay(1000);
  Serial1.println("M CA , M C.");
  delay(100);
  Serial1.println("M CA , M C.");
  delay(100);
  Serial1.println("M CA , M C.");
  delay(100);

  Serial.println("M CA , M C.");
  Serial.println("Calibration started");


  while(!calibrated)
  {

  lv_obj_clear_flag(ui_Spinner2, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_Label9, LV_OBJ_FLAG_HIDDEN);

  update_ui();
  readlogs();
  }
  Serial.println("Calibration done");

  lv_scr_load(ui_Home); // Replace ui_Screen1 with the desired screen name from ui.h

}

void setup() {
  Serial.begin(921600);
  Serial1.begin(921600);


  backlight.set(0);
// Initialize display
  Display.begin();
  display.fillScreen(0);

// Initialize LVGL early
  lv_init();

 
  // Initialize touch
   TouchDetector.begin();
  // Initialize LVGL and UI

    ui_init();
  //lv_scr_load(ui_Blank); // Replace ui_Screen1 with the desired screen name from ui.h



  pinMode(RIGHT_BTN, INPUT_PULLUP);  // Right button
  pinMode(LEFT_BTN, INPUT_PULLUP);   // Left button
  lv_slider_set_range(ui_speedSet, 10, (int)round((float)STEPPER_SPEED / DIVIDER_STEP_MM));
  lv_slider_set_range(uic_speedSetAB, 10, (int)round((float)STEPPER_SPEED / DIVIDER_STEP_MM));
  lv_slider_set_range(uic_speedSetBA, 10, (int)round((float)STEPPER_SPEED / DIVIDER_STEP_MM));

  lv_slider_set_range(uic_Header_battery, 129, 142); // 950 - 850. 525 - 583
  lv_slider_set_range(uic_Header1_battery, 129, 142);
  lv_slider_set_range(uic_Header2_battery, 129, 142);
  lv_slider_set_range(uic_Header4_battery, 129, 142);


  char buffer[16];
  snprintf(buffer, sizeof(buffer), "%d mm", 0);
  lv_label_set_text(uic_SetAValue1, buffer);

  snprintf(buffer, sizeof(buffer), "%d mm", 0);
  lv_label_set_text(uic_SetBValue1, buffer);



  calibrate();


  //
}

void loop() {
  update_ui();
  delay(5);
  readlogs();
  pushbtn();
}
