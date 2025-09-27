#include "Arduino_H7_Video.h"
#include "Arduino_GigaDisplayTouch.h"
#include "lvgl.h"
#include <ui.h>

Arduino_H7_Video Display( 480, 800, GigaDisplayShield ); //( 800, 480, GigaDisplayShield );
Arduino_GigaDisplayTouch TouchDetector;


#define RIGHT_BTN A0
#define LEFT_BTN  A1


long position = 0;
float Battery = 0.0;
bool emergency = false;
bool limitright = false;
bool limitleft = false;
long TS = 0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  Display.begin();
  TouchDetector.begin();
  ui_init();

  pinMode(RIGHT_BTN, INPUT_PULLUP); // Right button
  pinMode(LEFT_BTN, INPUT_PULLUP);  // Left button


}

void loop()
{
  update_ui();
  delay(5);
  readlogs();
  read_value();
  pushbtn();

}



