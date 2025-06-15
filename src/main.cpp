#include <Arduino.h>
#include "peripheral/DistanceSensor.h"
#include "core/DoorOpenDetector.h"

hw_timer_t * timer0 = NULL;
DistanceSensor ds;
DoorOpenDetector doorOpenDetector(&ds); // DoorOpenDetector??C???X?^???X????

void IRAM_ATTR onTimer() {
  ds.update();
}

void setup() {
  Serial.begin(115200);
  ds.init();
  // doorOpenDetector.reset(); // ?K?v??????????????????????o??
  // Configure hardware timer for 10ms interrupts
  timer0 = timerBegin(0, 80, true);
  timerAttachInterrupt(timer0, &onTimer, true);
  timerAlarmWrite(timer0, 10000, true);
  timerAlarmEnable(timer0);
}

void loop() {
  static unsigned long lastUpdateTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastUpdateTime >= 1000) {
    lastUpdateTime = currentTime;
    doorOpenDetector.update();
  }
}
