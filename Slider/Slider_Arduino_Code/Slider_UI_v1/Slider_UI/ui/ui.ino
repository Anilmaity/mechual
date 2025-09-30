#include "Arduino_H7_Video.h"
#include "Arduino_GigaDisplayTouch.h"
#include "lvgl.h"
#include <ui.h>

Arduino_H7_Video Display(480, 800, GigaDisplayShield);  //( 800, 480, GigaDisplayShield );
Arduino_GigaDisplayTouch TouchDetector;


#define RIGHT_BTN A0
#define LEFT_BTN A1
#define STEPPER_SPEED 2000     // 120 mm/s
#define DIVIDER_STEP_MM 6.67  // 400 /(3*20)



// LENGTH  840  mm    20 thread pully , 3 mm pitch  400 microstepping  = 400 /(3*20)   // 0.15 mm precision

long position = 50;
float Battery = 20;
bool emergency = false;
bool limitright = false;
bool limitleft = false;
long TS = 0;


int speedVal = 10;
int posVal = 0;



void setup() {
  Serial.begin(1000000);
  Serial1.begin(1000000);

  Display.begin();
  ui_init();
  TouchDetector.begin();

  pinMode(RIGHT_BTN, INPUT_PULLUP);  // Right button
  pinMode(LEFT_BTN, INPUT_PULLUP);   // Left button
  lv_slider_set_range(ui_speedSet, 10, int((STEPPER_SPEED) / DIVIDER_STEP_MM));
  lv_slider_set_range(uic_speedSetAB, 10, int((STEPPER_SPEED) / DIVIDER_STEP_MM));
  lv_slider_set_range(uic_speedSetBA, 10, int((STEPPER_SPEED) / DIVIDER_STEP_MM));

  char buffer[16];
  snprintf(buffer, sizeof(buffer), "%d mm", 0);
  lv_label_set_text(uic_SetAValue1, buffer);

  snprintf(buffer, sizeof(buffer), "%d mm", 0);
  lv_label_set_text(uic_SetBValue1, buffer);

  //
}

void loop() {
  update_ui();
  delay(5);
  readlogs();
  pushbtn();
}
