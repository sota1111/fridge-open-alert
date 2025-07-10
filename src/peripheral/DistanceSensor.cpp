#include "DistanceSensor.h"
#include <Wire.h>
#include <M5Unified.h> // Include for M5.Lcd and Serial if needed
#include "../Config.h"

DistanceSensor::DistanceSensor(float ts, float tau)
    : execution_cycle_ts(ts),
      filter_time_constant_tau(tau),
      sensor_output(0),
      sensor_filtered_value(0.0f),
      sensor_filtered_value_prev(0.0f),
      serialBuffer("") {
    lpf_alpha = execution_cycle_ts / (filter_time_constant_tau + execution_cycle_ts);
}

void DistanceSensor::init() {
    if (Config::COMMUNICATION_MODE == CommunicationMode::I2C) {
        Wire.begin(0, 26, 100000UL); // Refer to TOF.ino for HAT connection settings
        sensor.setTimeout(500);
        if (!sensor.init()) {
            // Error handling: Display on M5StickC LCD, etc.
            // Example of serial output and LCD display
            Serial.println("Failed to detect and initialize VL53L0X sensor!");
            M5.Lcd.fillScreen(BLACK);
            M5.Lcd.setCursor(10, 10);
            M5.Lcd.setTextSize(2);
            M5.Lcd.setTextColor(RED);
            M5.Lcd.print("Sensor Fail");
            // Consider stopping processing with an infinite loop, etc.
            while(1);
        }
        sensor.startContinuous();
    } else if (Config::COMMUNICATION_MODE == CommunicationMode::SERIAL_COMM) {
        // Initialize with default distance for serial mode
        sensor_output = 100; // Default distance in mm
        sensor_filtered_value = 100.0f;
        sensor_filtered_value_prev = 100.0f;
        Serial.println("Distance sensor initialized in Serial mode");
    }
}

void DistanceSensor::update() {
    if (Config::COMMUNICATION_MODE == CommunicationMode::I2C) {
        sensor_output = sensor.readRangeContinuousMillimeters();
        if (sensor.timeoutOccurred()) {
            Serial.println("VL53L0X TIMEOUT");
            // Consider how to handle the value in case of timeout (e.g., maintain previous value, set as error value, etc.)
            // Here, the filter calculation is not performed, and the previous value is maintained.
            return;
        }
        lowPassFilter();
    } else if (Config::COMMUNICATION_MODE == CommunicationMode::SERIAL_COMM) {
        checkSerialInput();
        lowPassFilter();
    }
}

void DistanceSensor::lowPassFilter() {
    sensor_filtered_value = lpf_alpha * sensor_output + (1.0f - lpf_alpha) * sensor_filtered_value_prev;
    sensor_filtered_value_prev = sensor_filtered_value;
}

float DistanceSensor::getFilteredValue() const {
    return sensor_filtered_value;
}

uint16_t DistanceSensor::getRawValue() const {
    return sensor_output;
}
bool DistanceSensor::isAboveThreshold() const {
    return sensor_filtered_value >= FILTERED_VALUE_THRESHOLD_MM;
}

void DistanceSensor::setSerialDistance(float distance) {
    sensor_output = (uint16_t)distance;
}

void DistanceSensor::checkSerialInput() {
    while (Serial.available() > 0) {
        char inChar = Serial.read();
        if (inChar == '\n') {
            if (serialBuffer.startsWith("DISTANCE:")) {
                String distanceStr = serialBuffer.substring(9);
                float distance = distanceStr.toFloat();
                if (distance > 0) {
                    setSerialDistance(distance);
                }
            }
            serialBuffer = "";
        } else {
            serialBuffer += inChar;
        }
    }
}