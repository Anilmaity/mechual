#include <RPC.h>

// Shared struct with 2 variables
struct SharedData {
  int int1;  // Updated by M7
  int int2;  // Updated by M4
  long int slider_position;
  
};

SharedData data;

void rpc_setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial1.begin(200000);
  while (!Serial1) ;
  Serial1.println("M4: Starting setup");

  // Blink LED to confirm M4 is running
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial1.println("M4: LED ON, blink " + String(i + 1));
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    Serial1.println("M4: LED OFF, blink " + String(i + 1));
    delay(200);
  }
  Serial1.println("M4: LED blinked 3 times");

  // Initialize RPC
  Serial1.println("M4: Attempting RPC.begin()");
  RPC.begin();
  Serial1.println("M4: RPC initialized");

  // Initialize struct
  data.int1 = data.int2 = 0;
  Serial1.println("M4: Shared data initialized");
}

void rpc_readdata() {
  Serial1.println("M4: Entering loop");
  // Receive from M7
  if (RPC.available()) {
    Serial1.println("M4: RPC data available");
    String recv = RPC.readStringUntil('\n');
    Serial1.println("M4: Received string: " + recv);
    int int1, int2;
    if (sscanf(recv.c_str(), "%d,%d", &int1, &int2) == 2) {
      data.int1 = int1;
      data.int2 = int2;
      Serial1.println("M4: Parsed: int1=" + String(data.int1));
    } else {
      Serial1.println("M4: Failed to parse RPC data");
    }

    // Update M4 field
    digitalWrite(LED_BUILTIN, data.int1 % 2 ? HIGH : LOW);
    data.int2++;

    // Send to M7
    String msg = String(data.int1) + "," + String(data.int2);
    RPC.println(msg);
    Serial1.println("M4: Sent: int2=" + String(data.int2));
  } else {
    Serial1.println("M4: No RPC data available");
  }

  delay(100);  // Check RPC frequently
}