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
void sendPositionMode(int pos) {
  Serial.print("M PM , P ");
  Serial.print(pos);
  Serial.println(" .");
  Serial.print("Giga: Sent Position Mode - Position: ");
  Serial.println(pos);
}

// Function to send AB Shuttle Mode command: "M AB , A 1000 , B 2000 , T 2 , L 2 ."
void sendABShuttleMode(int a, int b, int t, int l) {
  Serial.print("M AB , A ");
  Serial.print(a);
  Serial.print(" , B ");
  Serial.print(b);
  Serial.print(" , T ");
  Serial.print(t);
  Serial.print(" , L ");
  Serial.print(l);
  Serial.println(" .");
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
  Serial.print("M CA , M ");
  Serial.print(c);
  Serial.println(" .");
  Serial.print("Giga: Sent Calibration - Command: ");
  Serial.println(c);
}


void sendcommand(){

  sendJogMode(100, 'R');  // Jog right at 100
  sendPositionMode(0);  // Position to 0
  sendABShuttleMode(1000, 2000, 2, 2);  // AB shuttle A=1000, B=2000, T=2s, L=2 loops
  sendCalibrationMode('C');  // Calibrate

}
