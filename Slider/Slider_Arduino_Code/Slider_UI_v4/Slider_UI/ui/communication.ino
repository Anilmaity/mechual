#include <RPC.h>
#include <stm32h7xx_ll_rcc.h>

// Shared struct with 2 variables
struct SharedData {
  int int1;  // Updated by M7
  int int2;  // Updated by M4
  

};

SharedData data;


void rpc_setup() {
  Serial.begin(2000000);
  while (!Serial) ;
  Serial.println("M7: Starting setup");

  // Boot M4 core
  LL_RCC_ForceCM4Boot();
  Serial.println("M7: M4 boot initiated");
  delay(2000);  // Wait for M4

  // Initialize RPC
  RPC.begin();
  Serial.println("M7: RPC initialized");

  // Initialize struct
  data.int1 = data.int2 = 0;
  Serial.println("M7: Shared data initialized");
}

void read_data() {
  // Update M7 field
  data.int1++;

  // Send to M4
  String msg = String(data.int1) + "," + String(data.int2);
  RPC.println(msg);
  Serial.println("M7: Sent: int1=" + String(data.int1) + ", int2=" + String(data.int2));

  // Receive from M4
  if (RPC.available()) {
    String recv = RPC.readStringUntil('\n');
    int int1, int2;
    sscanf(recv.c_str(), "%d,%d", &int1, &int2);
    data.int2 = int2;  // Update int2 from M4
    Serial.println("M7: Received: int2=" + String(data.int2));
  }

  delay(1000);  // Update every 1s
}