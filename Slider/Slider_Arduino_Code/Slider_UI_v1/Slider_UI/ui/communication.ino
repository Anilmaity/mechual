// Define variables to store parsed values
float Battery = 0.0;
bool emergency = false;
bool limitleft = false;
bool limitright = false;
int position = 0;


void readlogs() {
  // Check for incoming data
  if (Serial.available() > 0) {
    // Read the full string until newline
    String received = Serial1.readStringUntil('\n');
    received.trim();  // Remove any whitespace or carriage returns

    // Verify the string starts with "P" and ends with "."
    if (received.startsWith("P") && received.endsWith(".")) {
      // Split the string by commas
      int firstComma = received.indexOf(',');
      int secondComma = received.indexOf(',', firstComma + 1);
      int thirdComma = received.indexOf(',', secondComma + 1);
      int fourthComma = received.indexOf(',', thirdComma + 1);

      // Extract and parse each field
      if (firstComma > 0 && secondComma > firstComma && thirdComma > secondComma && fourthComma > thirdComma) {
        // Extract position (P <int>)
        String posStr = received.substring(2, firstComma);  // Skip "P "
        posStr.trim();
        position = posStr.toInt();

        // Extract Battery (B <float>)
        String batStr = received.substring(firstComma + 3, secondComma);  // Skip ", B "
        batStr.trim();
        Battery = batStr.toFloat();

        // Extract emergency (E <0/1>)
        String emergStr = received.substring(secondComma + 3, thirdComma);  // Skip ", E "
        emergStr.trim();
        emergency = (emergStr == "1");

        // Extract limitright (LR <0/1>)
        String lrStr = received.substring(thirdComma + 4, fourthComma);  // Skip ", LR "
        lrStr.trim();
        limitright = (lrStr == "1");

        // Extract limitleft (LL <0/1>)
        String llStr = received.substring(fourthComma + 4, received.length() - 1);  // Skip ", LL ", exclude "."
        llStr.trim();
        limitleft = (llStr == "1");

        // Print parsed values to USB Serial for verification
        Serial.println("Parsed values:");
        Serial.print("Position: "); Serial.println(position);
        Serial.print("Battery: "); Serial.println(Battery, 1);  // 1 decimal place
        Serial.print("Emergency: "); Serial.println(emergency ? "true" : "false");
        Serial.print("Limit Right: "); Serial.println(limitright ? "true" : "false");
        Serial.print("Limit Left: "); Serial.println(limitleft ? "true" : "false");
        Serial.println("---");
      } else {
        Serial.println("Error: Invalid string format");
      }
    } else {
      Serial.println("Error: String does not match expected format");
    }
  }
}


