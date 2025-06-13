#include <Arduino.h>
#include "peripheral/DistanceSensor.h"

hw_timer_t * timer0 = NULL;
DistanceSensor ds;
void IRAM_ATTR onTimer() {
  ds.update();
}

// put function declarations here:
int myFunction(int, int);

void setup() {
  Serial.begin(115200);
  ds.init();
  // Configure hardware timer for 10ms interrupts
  timer0 = timerBegin(0, 80, true);
  timerAttachInterrupt(timer0, &onTimer, true);
  timerAlarmWrite(timer0, 10000, true);
  timerAlarmEnable(timer0);
}

void loop() {
  // put your main code here, to run repeatedly:
}
