#include "SharedData.h"
#include <stm32h7xx_ll_rcc.h>


void setup() {
  Serial.begin(115200);
  while (!Serial) ;  // Wait for Serial
  Serial.println("M7: Starting setup");
  Serial.println("M7: SharedData size: " + String(sizeof(SharedData)));

  // Boot M4 core
  LL_RCC_ForceCM4Boot();
  Serial.println("M7: M4 boot initiated");
  delay(2000);  // Wait for M4

  // Initialize shared data
  Serial.println("M7: Attempting initSharedData()");
  initSharedData();
  Serial.println("M7: Shared data initialized");

  // Test SDRAM stability
  Serial.println("M7: Testing SDRAM...");
  if (SDRAM.test()) {
    Serial.println("M7: SDRAM test passed");
  } else {
    Serial.println("M7: SDRAM test failed");
    while (1);
  }
}

void loop() {
  volatile SharedData* data = SHARED_DATA_ADDRESS;
  data->int1++;
  data->float1 += 0.1f;
  data->flag1 = !data->flag1;
  strncpy((char*)data->str1, "M7", sizeof(data->str1));  // Cast to char*
#ifdef __CM7_CMSIS_VERSION
  SCB_CleanInvalidateDCache_by_Addr((uint32_t*)data, sizeof(SharedData));
#endif
  // Fixed Serial.println with cast for str1
  Serial.println("M7: int1=" + String(data->int1) + " int2 =" + String(data->int2) + ", float1=" + String(data->float1) + ", str1=" + String((char*)data->str1));
  
  delay(1000);  // Delay to avoid M4 conflict
}