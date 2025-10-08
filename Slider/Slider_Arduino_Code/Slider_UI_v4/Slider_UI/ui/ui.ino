#include "Arduino_H7_Video.h"
#include "Arduino_GigaDisplayTouch.h"
#include "lvgl.h"
#include <ui.h>
#include <math.h> // Include math.h for round()
//Create backlight object


Arduino_H7_Video Display(480, 800, GigaDisplayShield);  //( 800, 480, GigaDisplayShield );
Arduino_GigaDisplayTouch TouchDetector;

#define STEPPER_SPEED 1000   // 1200. // max 4800
#define DIVIDER_STEP_MM 6.67 // 400 /(3*20)



// LENGTH  840  mm    20 thread pully , 3 mm pitch  400 microstepping  = 400 /(3*20)   // 0.15 mm precision

long position = 50;
float Battery = 20;
bool emergency = false;
bool limitright = false;
bool limitleft = false;
long TS = 0;


int speedVal = 10;
int posVal = 0;
bool calibrated = true;

extern lv_obj_t *ui_Spinner2, *ui_Label9;


void setup() {
  Serial.begin(2000000);
  Serial.println("M7 init");

  display_Setup();

  rpc_setup();  // Initialize shared memory
  //
}

void loop() {
  update_ui();
  delay(5);

 read_data();

}
