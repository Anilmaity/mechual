// Global variables for parsed logs (from Nano)


// Function to send Jog Mode command: "M JM , S 100 , C R ."
void sendJogMode(int speed, char c) {  // c = 'R', 'L', or 'I'
  Serial.print("M JM , S ");
  Serial.print(speed);
  Serial.print(" , C ");
  Serial.print(c);
  Serial.println(" .");
  Serial.print("Giga: Sent Jog Mode - Speed: ");
  Serial.print(speed);
  Serial.print(", Command: ");
  Serial.println(c);
}

// Function to send Position Mode command: "M PM , P 0 ."
void sendPositionMode(int pos, int speed) {
  Serial1.print("M PM , P ");
  Serial1.print(pos);
  Serial1.print(" , S ");
  Serial1.print(speed);
  Serial1.println(" .");
}

// Function to send AB Shuttle Mode command: "M AB , A 1000 , B 2000 , T 2 , L 2 ."
void sendABShuttleMode(int a, int b, int s1, int s2, int l) {
  Serial1.print("M AB , X ");
  Serial1.print(a);
  Serial1.print(" , Y ");
  Serial1.print(b);
  Serial1.print(" , S1 ");
  Serial1.print(s1);
  Serial1.print(" , S2 ");
  Serial1.print(s2);
  Serial1.print(" , L ");
  Serial1.print(l);
  Serial1.println(" .");

  Serial.print("M AB , X ");
  Serial.print(a);
  Serial.print(" , Y ");
  Serial.print(b);
  Serial.print(" , S1 ");
  Serial.print(s1);
  Serial.print(" , S2 ");
  Serial.print(s2);
  Serial.print(" , L ");
  Serial.print(l);
  Serial.println(" .");
}

// Function to send Calibration Mode command: "M CA , M C ."
void sendCalibrationMode(char c) {  // c = 'C', 'R', or 'L'
  Serial1.print("M CA , M ");
  Serial1.print(c);
  Serial1.println(" .");
}
