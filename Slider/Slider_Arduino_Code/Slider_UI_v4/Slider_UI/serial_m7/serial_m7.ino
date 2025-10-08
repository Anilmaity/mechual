#include <stm32h7xx_ll_rcc.h>

// Shared struct with 2 variables
struct SharedData {
  int int1;  // Updated by M7
  int int2;  // Updated by M4
};

SharedData data;

void setup() {
  Serial.begin(115200);
  while (!Serial) ;
  Serial.println("M7: Starting setup");

  // Boot M4 core
  LL_RCC_ForceCM4Boot();
  Serial.println("M7: M4 boot initiated");

  // Initialize Serial2 (PA2 TX, PA3 RX) at 2 Mbps
  Serial2.begin(2000000);
  Serial.println("M7: Serial2 initialized at 2 Mbps");

  // Initialize struct
  data.int1 = data.int2 = 0;
  Serial.println("M7: Shared data initialized");

  delay(2000);  // Wait for M4
}

void loop() {
  // Update M7 field
  data.int1++;

  // Send binary data to M4
  uint8_t buffer[8];
  memcpy(buffer, &data.int1, 4);
  memcpy(buffer + 4, &data.int2, 4);
  Serial2.write(buffer, 8);
  Serial.println("M7: Sent: int1=" + String(data.int1) + ", int2=" + String(data.int2));

  // Receive from M4
  if (Serial2.available() >= 8) {
    uint8_t recv[8];
    Serial2.readBytes(recv, 8);
    int int2;
    memcpy(&int2, recv + 4, 4);
    data.int2 = int2;  // Update int2 from M4
    Serial.println("M7: Received: int2=" + String(data.int2));
  }

  delay(1000);  // Update every 1s
}