// #include <SoftwareSerial.h>

// // Debug serial on pins 2 (RX, unused) and 3 (TX)
// SoftwareSerial debugSerial(8, 9);  // RX, TX



// Function prototypes for modular parsing
bool parseCommand(String& cmd);
bool parseJogMode(String& cmd);
bool parsePositionMode(String& cmd);
bool parseABShuttleMode(String& cmd);
bool parseCalibrationMode(String& cmd);
void printParsedValues();  // For debug


void readlogs() {
  if (Serial.available() > 0) {
    // Read full command until newline
    String received = Serial.readStringUntil('\n');
    Serial.println(received);

    received.trim();
    
    // Parse if it matches expected format (starts with "M", ends with ".")
    if (received.startsWith("M") && received.endsWith(".")) {
      if (parseCommand(received)) {
        printParsedValues();  // Output to debug for verification
        // TODO: Add your logic here, e.g., act on variables (move motors, etc.)
      } else {
        Serial.println("Error: Failed to parse command");
      }
    } else {
      Serial.println("Error: Invalid command format");
    }
  }
}

// Main parser: Determines mode and calls specific parser
bool parseCommand(String& cmd) {
  // Extract mode (after "M ", before first comma)
  int firstComma = cmd.indexOf(',');
  if (firstComma < 0) return false;
  
  String modeStr = cmd.substring(2, firstComma);  // Skip "M "
  modeStr.trim();
  currentMode = modeStr;
  
  // Call mode-specific parser
  if (currentMode == "JM") {
    modeCurrent = JOGGING;
    return parseJogMode(cmd);
  } else if (currentMode == "PM") {
    modeCurrent = POSITIONING;
    return parsePositionMode(cmd);
  } else if (currentMode == "AB") {
    modeCurrent = AB_SHUTTLE;
    return parseABShuttleMode(cmd);
  } else if (currentMode == "CA") {
    modeCurrent = CALIBRATING;
    return parseCalibrationMode(cmd);
  } else {
    Serial.print("Error: Unknown mode - ");
    Serial.println(currentMode);
    return false;
  }
}

// Parser for Jog Mode: "M JM , S 100 , C R ."
bool parseJogMode(String& cmd) {

  int sComma = cmd.indexOf("S", 0);
  int cComma = cmd.indexOf("C", sComma);
  if (sComma < 0 || cComma < 0) return false;
  
  // Extract S (speed)
  String speedStr = cmd.substring(sComma + 2, cComma - 2);  // Skip "S "
  speedStr.trim();
  jogSpeed = speedStr.toInt();
  
  // Extract C (command)
  String cmdStr = cmd.substring(cComma + 2, cmd.length() - 1);  // Skip "C ", exclude "."
  cmdStr.trim();


  if (cmdStr.length() == 1) {
    jogCommand = cmdStr[0];  // 'R', 'L', 'I'
    return true;
  }
  return false;
}

// Parser for Position Mode: "M PM , P 0 ."
// Global variable for position

// Parser for Position Mode: "M PM , P <position>."
bool parsePositionMode(String& cmd) {
  // Find the second "P" (after "M PM , P")
  int pComma = cmd.indexOf("P", cmd.indexOf("P") + 1);  // Start after first "P"
  if (pComma < 0) return false;

  // Extract position substring (after "P ", before ".")
  String posStr = cmd.substring(pComma + 2, cmd.length() - 1);  // Skip "P ", exclude "."
  posStr.trim();  // Remove leading/trailing whitespace

  // Check for empty string
  if (posStr.length() == 0) return false;

  // Validate numeric content
  for (char c : posStr) {
    if (!isdigit(c)) return false;  // Only allow digits (no negative numbers)
  }

  // Convert to long to handle large numbers safely
  long posValue = posStr.toInt();  // toInt() returns 0 for invalid numbers
  if (posValue < 0 || posValue > 99999) return false;  // Enforce range 0-99999

  // Store valid position
  position = (int)posValue;  // Safe cast since we checked range
  // Serial.println(position);  // Debug: print extracted string
  return true;
}

bool parseABShuttleMode(String &cmd) {
  int aPos = cmd.indexOf("X");
  int bPos = cmd.indexOf("Y", aPos);
  int tPos = cmd.indexOf("T", bPos);
  int lPos = cmd.indexOf("L", tPos);

  if (aPos < 0 || bPos < 0 || tPos < 0 || lPos < 0) return false;

  // Extract A
  String aStr = cmd.substring(aPos + 1, bPos);  // from after 'A' to before 'B'
  aStr.replace(",", "");
  aStr.trim();
  pointA = aStr.toInt();

  // Extract B
  String bStr = cmd.substring(bPos + 1, tPos);  // from after 'B' to before 'T'
  bStr.replace(",", "");
  bStr.trim();

  pointB = bStr.toInt();

  // Extract T
  String tStr = cmd.substring(tPos + 1, lPos);
  tStr.replace(",", "");
  tStr.trim();
  shuttleTime = tStr.toInt();

  // Extract L
  String lStr = cmd.substring(lPos + 1);  // till end
  lStr.replace(".", "");
  lStr.replace(",", "");
  lStr.trim();
  shuttleLoops = lStr.toInt();

  return true;
}


// Parser for Calibration Mode: "M CA , M C ."
bool parseCalibrationMode(String& cmd) {
  int mComma = cmd.indexOf("M", 5);  // Skip first "M CA , " (start after position 5)
  if (mComma < 0) return false;
  
  // Extract M (command)
  String cmdStr = cmd.substring(mComma + 2, cmd.length() - 1);  // Skip "M ", exclude "."
  cmdStr.trim();
  if (cmdStr.length() == 1) {
    calCommand = cmdStr[0];  // 'C', 'R', 'L'
    return true;
  }
  return false;
}

// Debug function to print current variables
void printParsedValues() {
  Serial.print("Mode: ");
  Serial.println(currentMode);
  
  if (currentMode == "JM") {
    Serial.print("Jog Speed: ");
    Serial.println(jogSpeed);
    Serial.print("Jog Command: ");
    Serial.println(jogCommand);

    // handleJogging(jogSpeed , jogCommand);

  } else if (currentMode == "PM") {
    Serial.print("Position: ");
    Serial.println(position);
  } else if (currentMode == "AB") {
    Serial.print("Point A: ");
    Serial.println(pointA);
    Serial.print("Point B: ");
    Serial.println(pointB);
    Serial.print("Shuttle Time (s): ");
    Serial.println(shuttleTime);
    Serial.print("Shuttle Loops: ");
    Serial.println(shuttleLoops);
  } else if (currentMode == "CA") {
    Serial.print("Calibration Command: ");
    Serial.println(calCommand);
  }
  Serial.println("---");
}


