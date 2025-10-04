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
    start_position = getStepsFromRight();
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
  // Check for "M PM , P" prefix
  if (!cmd.startsWith("M PM , P")) return false;

  // Find delimiters
  int pPos = cmd.indexOf("P", 6); // Start after "M PM ,"
  int sPos = cmd.indexOf("S", pPos);
  if (pPos < 0 || sPos < 0) return false;

  // Extract position substring (after "P ", before ", S")
  int commaPos = cmd.indexOf(",", pPos);
  if (commaPos < 0 || commaPos > sPos) return false;
  String posStr = cmd.substring(pPos + 2, commaPos);
  posStr.trim(); // Remove whitespace

  // Extract speed substring (after "S ", before ".")
  String speedStr = cmd.substring(sPos + 2, cmd.length() - 1);
  speedStr.trim(); // Remove whitespace

  // Check for empty strings
  if (posStr.length() == 0 || speedStr.length() == 0) return false;

  // Validate position (digits only, range 0-99999)
  for (char c : posStr) {
    if (!isdigit(c)) return false;
  }
  long posValue = posStr.toInt();
  if (posValue < 0 || posValue > 99999) return false;

  // Validate speed (digits only, range 0-1000)
  for (char c : speedStr) {
    if (!isdigit(c)) return false;
  }
  long speedValue = speedStr.toInt();
  if (speedValue < 0 || speedValue > 1000) return false;

  // Store valid values
  position = (int)posValue; // Safe cast due to range check
  jogSpeed = (int)speedValue;  // Store speed in global variable
  return true;
}



bool parseABShuttleMode(String &cmd) {
  // Check for "M AB , " prefix
  if (!cmd.startsWith("M AB , ")) return false;

  // Remove prefix and trailing period
  String cleanedCmd = cmd.substring(7); // Skip "M AB , "
  if (cleanedCmd.endsWith(".")) {
    cleanedCmd = cleanedCmd.substring(0, cleanedCmd.length() - 1); // Remove "."
  }

  // Find delimiters
  int xPos = cleanedCmd.indexOf("X");
  int yPos = cleanedCmd.indexOf("Y", xPos);
  int s1Pos = cleanedCmd.indexOf("S1", yPos);
  int s2Pos = cleanedCmd.indexOf("S2", s1Pos);
  int lPos = cleanedCmd.indexOf("L", s2Pos);

  // Validate delimiter positions
  if (xPos < 0 || yPos < 0 || s1Pos < 0 || s2Pos < 0 || lPos < 0) return false;

  // Extract X (pointA)
  String xStr = cleanedCmd.substring(xPos + 1, yPos);
  xStr.replace(",", "");
  xStr.trim();
  pointA = xStr.toInt();

  // Extract Y (pointB)
  String yStr = cleanedCmd.substring(yPos + 1, s1Pos);
  yStr.replace(",", "");
  yStr.trim();
  pointB = yStr.toInt();

  // Extract S1 (speed1)
  String s1Str = cleanedCmd.substring(s1Pos + 2, s2Pos);
  s1Str.replace(",", "");
  s1Str.trim();
  speed1 = s1Str.toInt();

  // Extract S2 (speed2)
  String s2Str = cleanedCmd.substring(s2Pos + 2, lPos);
  s2Str.replace(",", "");
  s2Str.trim();
  speed2 = s2Str.toInt();

  // Extract L (shuttleLoops)
  String lStr = cleanedCmd.substring(lPos + 1);
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
    if (calCommand == 'C');
      initiateRecalibration();
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
    Serial.print("Speed: ");
    Serial.println(jogSpeed);
  } else if (currentMode == "AB") {
    Serial.print("Point A: ");
    Serial.println(pointA);
    Serial.print("Point B: ");
    Serial.println(pointB);
    Serial.print("Shuttle Loops: ");
    Serial.println(shuttleLoops);
  } else if (currentMode == "CA") {
    Serial.print("Calibration Command: ");
    Serial.println(calCommand);
  }
  Serial.println("---");
}


