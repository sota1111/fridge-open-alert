#pragma once

// Distance sensor communication mode
enum class CommunicationMode {
    I2C,
    SERIAL_COMM
};

// Global configuration
struct Config {
    // Change this to switch between I2C and Serial modes
    static constexpr CommunicationMode COMMUNICATION_MODE = CommunicationMode::I2C;
    
    static constexpr unsigned long SERIAL_BAUDRATE = 115200;
    static constexpr unsigned long SERIAL_SEND_INTERVAL_MS = 1000;
    static constexpr float DISTANCE_THRESHOLD_MM = 50.0f;
    static constexpr unsigned long ALERT_THRESHOLD_MS = 60000; // 60 seconds
};