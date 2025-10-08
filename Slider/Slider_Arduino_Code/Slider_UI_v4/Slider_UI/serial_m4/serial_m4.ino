// Shared struct with 2 variables
struct SharedData {
  int int1;  // Updated by M7
  int int2;  // Updated by M4
};

SharedData data;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial1.begin(115200);  // Debugging
  while (!Serial1) ;
  Serial1.println("M4: Starting setup");

  // Initialize Serial1 for communication (PA9 TX, PA10 RX) at 2 Mbps
  Serial1.begin(2000000);
  Serial1.println("M4: Serial1 initialized at 2 Mbps");

  // Initialize struct
  data.int1 = data.int2 = 0;
  Serial1.println("M4: Shared data initialized");

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
}



void loop() {
  Serial1.println("M4: Entering loop");
  // Receive from M7
  if (Serial1.available() >= 8) {
    Serial1.println("M4: Serial data available");
    uint8_t recv[8];
    Serial1.readBytes(recv, 8);
    memcpy(&data.int1, recv, 4);
    memcpy(&data.int2, recv + 4, 4);
    Serial1.println("M4: Received: int1=" + String(data.int1));

    // Update M4 field
    digitalWrite(LED_BUILTIN, data.int1 % 2 ? HIGH : LOW);
    data.int2++;

    // Send to M7
    uint8_t buffer[8];
    memcpy(buffer, &data.int1, 4);
    memcpy(buffer + 4, &data.int2, 4);
    Serial1.write(buffer, 8);
    Serial1.println("M4: Sent: int2=" + String(data.int2));
  } else {
    Serial1.println("M4: No data available");
  }

  delay(10);  // Check Serial frequently
}