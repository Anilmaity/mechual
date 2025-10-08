void setup() {
  // Initialize USB Serial for debugging
  Serial.begin(921600);
  while (!Serial);

  // // Initialize Serial2 for communication with Arduino Giga
  // // Pins: RX=16, TX=17 (custom for Serial2)
  // Serial.begin(9600);
  Serial.println("ESP32 ready! Listening for messages from Giga...");
}

void loop() {
  // Check for message from Arduino Giga
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    Serial.print("Received from Giga: ");
    Serial.println(message);

    // Echo back with addition
    String response = message + " - Echo from ESP32!";
    Serial.println(response);
    Serial.print("Sent back: ");
    Serial.println(response);
  }

  // Optional: Non-blocking delay or other tasks here
  delay(1000); // Small delay to avoid overwhelming the loop
}