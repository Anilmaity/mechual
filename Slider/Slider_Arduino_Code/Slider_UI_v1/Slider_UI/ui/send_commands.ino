// Global variables for parsed logs (from Nano)


// Function to send Jog Mode command: "M JM , S 100 , C R ."
void sendJogMode(int speed, char c) {  // c = 'R', 'L', or 'I'
  Serial1.print("M JM , S ");
  Serial1.print(speed);
  Serial1.print(" , C ");
  Serial1.print(c);
  Serial1.println(" .");
  Serial.print("Giga: Sent Jog Mode - Speed: ");
  Serial.print(speed);
  Serial.print(", Command: ");
  Serial.println(c);
}

// Function to send Position Mode command: "M PM , P 0 ."
void sendPositionMode(int pos) {
  Serial1.print("M PM , P ");
  Serial1.print(pos);
  Serial1.println(" .");
  Serial.print("Giga: Sent Position Mode - Position: ");
  Serial.println(pos);
}

// Function to send AB Shuttle Mode command: "M AB , A 1000 , B 2000 , T 2 , L 2 ."
void sendABShuttleMode(int a, int b, int t, int l) {
  Serial1.print("M AB , A ");
  Serial1.print(a);
  Serial1.print(" , B ");
  Serial1.print(b);
  Serial1.print(" , T ");
  Serial1.print(t);
  Serial1.print(" , L ");
  Serial1.print(l);
  Serial1.println(" .");
  Serial.print("Giga: Sent AB Shuttle - A: ");
  Serial.print(a);
  Serial.print(", B: ");
  Serial.print(b);
  Serial.print(", T: ");
  Serial.print(t);
  Serial.print(", L: ");
  Serial.println(l);
}

// Function to send Calibration Mode command: "M CA , M C ."
void sendCalibrationMode(char c) {  // c = 'C', 'R', or 'L'
  Serial1.print("M CA , M ");
  Serial1.print(c);
  Serial1.println(" .");
  Serial.print("Giga: Sent Calibration - Command: ");
  Serial.println(c);
}


void sendcommand(){

  sendJogMode(100, 'R');  // Jog right at 100
  sendPositionMode(0);  // Position to 0
  sendABShuttleMode(1000, 2000, 2, 2);  // AB shuttle A=1000, B=2000, T=2s, L=2 loops
  sendCalibrationMode('C');  // Calibrate

}
