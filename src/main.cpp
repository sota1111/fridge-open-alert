#include <Arduino.h>
#include <M5Unified.h>
#include "peripheral/DistanceSensor.h"
#include "core/DoorOpenDetector.h"
#include "Config.h"

DistanceSensor ds;
DoorOpenDetector doorOpenDetector(&ds);

void setup() {
  M5.begin();
  Serial.begin(Config::SERIAL_BAUDRATE);
  ds.init();
}

void loop() {
  static unsigned long lastUpdateTime = 0;
  static unsigned long lastSensorUpdateTime = 0;
  static unsigned long lastSerialSendTime = 0;
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
    M5.Lcd.printf("Distance: %6.2f mm\n", distance);
    M5.Lcd.setCursor(0, 40);
    M5.Lcd.printf("Door: %s", isOpen ? "Open  " : "Closed");
    
    // Display communication mode
    M5.Lcd.setCursor(0, 55);
    M5.Lcd.printf("Mode: %s", Config::COMMUNICATION_MODE == CommunicationMode::I2C ? "I2C" : "Serial");
    
    // Display buzzer status for Serial mode
    if (Config::COMMUNICATION_MODE == CommunicationMode::SERIAL_COMM) {
      M5.Lcd.setCursor(0, 70);
      M5.Lcd.printf("Buzzer: %s", doorOpenDetector.getSystemState() == DoorOpenDetector::SystemState::Alert ? "ON " : "OFF");
    }
    
    // Door open time display
    unsigned long openMs = doorOpenDetector.getOpenDurationMillis();
    M5.Lcd.setCursor(0, 85);
    M5.Lcd.printf("Open Time: %lu s", openMs / 1000);
  }

  // Serial command processing
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command.startsWith("DISTANCE:")) {
      float distance = command.substring(9).toFloat();
      ds.setSerialDistance(distance);
    }
    else if (command == "RESET") {
      // Reset M5StickC device
      Serial.println("RESET:OK");
      Serial.flush();
      delay(100);
      ESP.restart();
    }
  }

  // Serial communication
  if (currentTime - lastSerialSendTime >= Config::SERIAL_SEND_INTERVAL_MS) {
    lastSerialSendTime = currentTime;
    doorOpenDetector.sendSerialStatus();
  }

  M5.update();
}
