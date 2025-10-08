void setup() {
  // Initialize USB Serial for debugging
  Serial.begin(921600);
  while (!Serial); // Wait for Serial Monitor to open

  // Initialize Serial1 for communication with ESP32
  Serial1.begin(921600); // RX=19, TX=18 (default for Serial1)
  Serial.println("Giga ready! Sending messages to ESP32...");
}

void loop() {
  // Send a message to ESP32
  Serial1.println("Hello from Giga!");
  Serial.println("Sent: Hello from Giga!");

  // Check for response from ESP32
  if (Serial1.available()) {
    String response = Serial1.readStringUntil('\n');
    Serial.print("Received from ESP32: ");
    Serial.println(response);
  }

  delay(1000); // Send every 2 seconds
}