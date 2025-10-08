

const uint8_t STEP_PIN = 3;              // Step pin
const uint8_t LIMIT_SWITCH_RIGHT = A1;   // Right limit switch (CW direction)
const uint8_t LIMIT_SWITCH_LEFT = A0;    // Left limit switch (CCW direction)
const uint8_t BATTERY_PIN = A2;          // Battery voltage analog pin
bool calibrated = false;

long int lastPrint = 0;

// Initialize hardware pins and peripherals
void initializeHardware() {
    Serial.begin(2000000);               // High baud rate for serial
    while (!Serial) {}                   // Wait for serial to initialize
    
    pinMode(LIMIT_SWITCH_RIGHT, INPUT_PULLUP);
    pinMode(LIMIT_SWITCH_LEFT, INPUT_PULLUP);
}

// SECTION 4: Limit Switch Functions
// Check if right limit switch is triggered
bool isRightLimitTriggered() {
    bool triggered = digitalRead(LIMIT_SWITCH_RIGHT) == LOW;
    return triggered;
}

// Check if left limit switch is triggered
bool isLeftLimitTriggered() {
    bool triggered = digitalRead(LIMIT_SWITCH_LEFT) == LOW;
    return triggered;
}





// Get battery voltage
float getBatteryVoltage() {
    float adc = analogRead(BATTERY_PIN);
    return (adc);  // Adjust ratio for actual voltage
}

void sendlogs() {
    float bat = getBatteryVoltage() ;

    int lr = isRightLimitTriggered() ? 1 : 0;
    int ll = isLeftLimitTriggered() ? 1 : 0;

    Serial.print("B ");
    Serial.print(bat, 1);
    Serial.print(" , LR ");
    Serial.print(lr);
    Serial.print(" , LL ");
    Serial.print(ll);
    Serial.println(" .");

}


// SECTION 9: Setup and Loop
void setup() {
    initializeHardware();
        sendlogs();

}

void loop() {
    
    if (millis() - lastPrint >= 500) {
        sendlogs();
        lastPrint = millis();
    }

}


