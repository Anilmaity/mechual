#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <Arduino.h>
#include <SDRAM.h>  // For SDRAM operations

#ifdef __CM7_CMSIS_VERSION
#include <core_cm7.h>  // For cache invalidation on M7
#endif

// Define shared struct at a fixed SDRAM address
#define SHARED_DATA_ADDRESS ((volatile SharedData*)0x60001000)  // SDRAM offset

// Shared struct with 5 variables
struct alignas(4) SharedData {  // Ensure 4-byte alignment
  int int1, int2 = 0;  // 2 integers
  float float1 = 0;    // 1 float
  bool flag1;      // 1 boolean
  char str1[8];    // 1 string
};

// Initialize global struct in SDRAM
void initSharedData() {
  volatile SharedData* data = SHARED_DATA_ADDRESS;
  Serial.println("M7: Initializing SDRAM...");
  if (!SDRAM.begin()) {
    Serial.println("M7: Error: SDRAM.begin() failed");
    return;
  }
  Serial.println("M7: SDRAM initialized");

  // Initialize struct fields
  data->int1 = data->int2 = 0;
  data->float1 = 0.0f;
  data->flag1 = false;
  strncpy((char*)data->str1, "INIT", sizeof(data->str1));  // Cast to char*
#ifdef __CM7_CMSIS_VERSION
  Serial.println("M7: Invalidating cache...");
  SCB_CleanInvalidateDCache_by_Addr((uint32_t*)data, sizeof(SharedData));
#endif
}

#endif