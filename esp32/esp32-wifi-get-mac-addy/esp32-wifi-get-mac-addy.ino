// Complete Instructions to Get and Change ESP MAC Address: https://RandomNerdTutorials.com/get-change-esp32-esp8266-mac-address-arduino/

#include <Arduino.h>
#include <Preferences.h>
#include <ArduinoNvs.h>


#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

const bool loopOutput = false;

Preferences prefs;

void setup(){
  Serial.begin(115200);
  
  outputData();

  delay(100);

  // Initialize NVS
  NVS.begin("my-test-app");
  
  if (!prefs.begin("my-test-app", true)) {
    Serial.println("Failed to open prefs");
  } else {
    Serial.print("Free entries: "); Serial.println(prefs.freeEntries());
  }
  
}

void loop(){
  if (!loopOutput) { return; }
  delay(5000);
  outputData();
  delay(5000);
}

void outputData() {
  Serial.println();
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  
  ps_malloc(550 * 1024);
  uint8_t * buf = (uint8_t*)malloc(500 * 1024);
  if (buf == NULL)
  {
    Serial.println("can't allocate memory with malloc\n");
  }

  Serial.print("ESP32 SDK: "); Serial.println(ESP.getSdkVersion());
  Serial.print("ESP32 CPU FREQ: "); Serial.print(getCpuFrequencyMhz()); Serial.println("MHz");
  Serial.print("ESP32 APB FREQ: "); Serial.print(getApbFrequency() / 1000000.0, 1); Serial.println("MHz");
  Serial.print("ESP32 getFlashChipSize FLASH SIZE: "); Serial.print(ESP.getFlashChipSize() / (1024.0 * 1024), 2); Serial.println("MB");
  Serial.print("ESP32 getHeapSize RAM SIZE: "); Serial.print(ESP.getHeapSize() / 1024.0, 2); Serial.println("KB");
  Serial.print("ESP32 getHeapSize FREE RAM: "); Serial.print(ESP.getFreeHeap() / 1024.0, 2); Serial.println("KB");
  Serial.print("ESP32 getMaxAllocHeap MAX RAM ALLOC: "); Serial.print(ESP.getMaxAllocHeap() / 1024.0, 2); Serial.println("KB");
  Serial.print("ESP32 getPsramSize PSRAM SIZE: "); Serial.print(ESP.getPsramSize() / 1024.0, 2); Serial.println("KB");
  Serial.print("ESP32 getFreePsram FREE PSRAM: "); Serial.print(ESP.getFreePsram() / 1024.0, 2); Serial.println("KB");
}
