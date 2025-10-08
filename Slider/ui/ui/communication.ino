

// void readlogs() {
//   if (Serial1.available() > 0) {
//     String received = Serial1.readStringUntil('\n');
//     received.trim();
//     // Optionally remove trailing dot
//     if (received.endsWith(".")) {
//       received = received.substring(0, received.length()-1);
//     }

//     // Expect it to start with "P"
//     if (!received.startsWith("P")) {
//       Serial.println("Error: does not start with P");
//       return;
//     }

//     // Split by commas
//     // We'll loop, finding comma separators
//     // Or we can use a simpler method using String::indexOf in a loop
//     int start = 0;
//     while (start < received.length()) {
//       int commaPos = received.indexOf(',', start);
//       String token;
//       if (commaPos < 0) {
//         token = received.substring(start);
//         start = received.length();
//       } else {
//         token = received.substring(start, commaPos);
//         start = commaPos + 1;
//       }
//       token.trim();  // remove leading/trailing whitespace

//       // Now parse token, of form "P 1000", or "B 20.6", or "E 0", etc.
//       if (token.length() < 2) continue;
//       // Find first space
//       int sp = token.indexOf(' ');
//       if (sp < 0) {
//         // malformed
//         continue;
//       }
//       String key = token.substring(0, sp);
//       String val = token.substring(sp + 1);
//       val.trim();

//       if (key == "P") {
//         position = val.toInt();
//       } else if (key == "B") {
//         Battery = val.toFloat();
//       } else if (key == "E") {
//         emergency = (val == "1");
//       } else if (key == "LR") {
//         limitright = (val == "1");
//       } else if (key == "LL") {
//         limitleft = (val == "1");
//       } else if (key == "TS") {
//         TS = val.toInt();
//       } else {
//         // unknown key — ignore or log
//         Serial.print("Unknown key: "); Serial.println(key);
//       }
//     }

//   }
// }

void readlogs() {
  if (Serial1.available() > 0) {
    String received = Serial1.readStringUntil('\n');
    received.trim();
    // Remove trailing dot
    if (received.endsWith(".")) {
      received = received.substring(0, received.length() - 1);
    }

    // Expect it to start with "P"
    if (!received.startsWith("P")) {
      Serial.println(received);
      Serial.println("Error: does not start with P");
      return;
    }

    // Split by commas
    int start = 0;
    while (start < received.length()) {
      int commaPos = received.indexOf(',', start);
      String token;
      if (commaPos < 0) {
        token = received.substring(start);
        start = received.length();
      } else {
        token = received.substring(start, commaPos);
        start = commaPos + 1;
      }
      token.trim();  // Remove whitespace

      // Parse token (e.g., "P 1000", "B 20.6")
      if (token.length() < 2) continue;
      int sp = token.indexOf(' ');
      if (sp < 0) continue;
      String key = token.substring(0, sp);
      String val = token.substring(sp + 1);
      val.trim();

      if (key == "P") {
        // Validate position range (0–99999)
        bool isValid = true;
        for (int i = 0; i < val.length(); i++) {
          char c = val[i];
          if (!(isdigit(c) || (i == 0 && c == '-'))) {
            isValid = false;
            break;
          }
        }
        long posValue = val.toInt();
        if (isValid && (posValue) >= -99999 && (posValue) <= 99999) {
          position = posValue;

        } else {
          Serial.println(isValid);
          Serial.println(posValue);
          Serial.println("Error: Position out of range (0-99999)");
          continue;
        }
      } else if (key == "B") {
        Battery = val.toFloat();
      } else if (key == "E") {
        emergency = (val == "1");
      } else if (key == "LR") {
        limitright = (val == "1");
      } else if (key == "LL") {
        limitleft = (val == "1");
      } else if (key == "TS") {
        TS = val.toInt();
        calibrated = true;
      } else {
        Serial.print("Unknown key: "); Serial.println(key);
      }
    }
  }

  


}


