#include <Arduino.h>
#include <M5Unified.h>
#include "peripheral/DistanceSensor.h"
#include "core/DoorOpenDetector.h"

DistanceSensor ds;
DoorOpenDetector doorOpenDetector(&ds);

void setup() {
  M5.begin();
  ds.init();
}

void loop() {
  static unsigned long lastUpdateTime = 0;
  static unsigned long lastSensorUpdateTime = 0;
  unsigned long currentTime = millis();

  // ds.update()
  if (currentTime - lastSensorUpdateTime >= 10) {
    lastSensorUpdateTime = currentTime;
    ds.update();
  }

  // doorOpenDetector.update()
  if (currentTime - lastUpdateTime >= 1000) {
    lastUpdateTime = currentTime;
    doorOpenDetector.update();

    float distance = ds.getFilteredValue();
    bool isOpen = ds.isAboveThreshold();

    //M5.Lcd.clear();
    M5.Lcd.setCursor(0, 10);
    M5.Lcd.printf("Distance: %.1f mm", distance);
    M5.Lcd.setCursor(0, 40);
    M5.Lcd.printf("Door: %s", isOpen ? "Open" : "Closed");
    // ドア開放時間表示
    unsigned long openMs = doorOpenDetector.getOpenDurationMillis();
    M5.Lcd.setCursor(0, 70);
    M5.Lcd.printf("Open Time: %lu s", openMs / 1000);
  }

  M5.update();
}
