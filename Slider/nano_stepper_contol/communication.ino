#include <SoftwareSerial.h>

// Debug serial on pins 2 (RX, unused) and 3 (TX)
SoftwareSerial debugSerial(2, 3);  // RX, TX



// Function prototypes for modular parsing
bool parseCommand(String& cmd);
bool parseJogMode(String& cmd);
bool parsePositionMode(String& cmd);
bool parseABShuttleMode(String& cmd);
bool parseCalibrationMode(String& cmd);
void printParsedValues();  // For debug

void com_setup() {
  // Hardware Serial for receiving from Giga at 1 Mbps
  Serial.begin(1000000);
  
  // Debug Serial
  debugSerial.begin(115200);
  Serial.println("Nano: Ready to receive and parse commands from Giga...");
}

void readlogs() {
  if (Serial.available() > 0) {
    // Read full command until newline
    String received = Serial.readStringUntil('\n');
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
bool parsePositionMode(String& cmd) {
  int pComma = cmd.indexOf("P", 0);
  if (pComma < 0) return false;
  
  // Extract P (position)
  String posStr = cmd.substring(pComma + 2, cmd.length() - 1);  // Skip "P ", exclude "."
  posStr.trim();
  position = posStr.toInt();
  return true;
}

// Parser for AB Shuttle Mode: "M AB , A 1000 , B 2000 , T 2 , L 2 ."
bool parseABShuttleMode(String& cmd) {
  int aComma = cmd.indexOf("A", 0);
  int bComma = cmd.indexOf("B", aComma);
  int tComma = cmd.indexOf("T", bComma);
  int lComma = cmd.indexOf("L", tComma);
  if (aComma < 0 || bComma < 0 || tComma < 0 || lComma < 0) return false;
  
  // Extract A
  String aStr = cmd.substring(aComma + 2, bComma - 2);  // Skip "A "
  aStr.trim();
  pointA = aStr.toInt();
  
  // Extract B
  String bStr = cmd.substring(bComma + 2, tComma - 2);  // Skip "B "
  bStr.trim();
  pointB = bStr.toInt();
  
  // Extract T
  String tStr = cmd.substring(tComma + 2, lComma - 2);  // Skip "T "
  tStr.trim();
  shuttleTime = tStr.toInt();
  
  // Extract L
  String lStr = cmd.substring(lComma + 2, cmd.length() - 1);  // Skip "L ", exclude "."
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


void sendlogs(){


  
}
