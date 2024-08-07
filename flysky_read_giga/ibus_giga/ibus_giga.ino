#include <iBus.h>

#define MAX_CHANNELS 10

iBus receiver(Serial3, MAX_CHANNELS);  // Serial2 pins in arduino giga
                                     
void setup() {
  receiver.begin();
}

void loop() {
  receiver.process();

  digitalWrite(LED_BUILTIN, receiver.available());  // turn led on if connected

  for(byte i = 1; i <= MAX_CHANNELS; i++){  // get channel values starting from 1
    Serial.print(receiver.get(i));
    Serial.print('\t');
  }
  Serial.println(receiver.get(0));  // received packet quality. If 0 -> there are some corrupted values
  delay(10);
}