#pragma once

#include "../peripheral/DistanceSensor.h"

class DoorOpenDetector {
public:
    enum class SystemState {
        Monitoring,
        Alert
    };

    enum class TimerState {
        Stopped,
        Counting
    };

    DoorOpenDetector(DistanceSensor* sensor);
    unsigned long getOpenDurationMillis() const;

    void update();
    void reset();

    void entry(SystemState system, TimerState timer);
    void exit(SystemState system, TimerState timer);
    void doAction(SystemState system, TimerState timer);
    
    SystemState getSystemState() const;
    TimerState getTimerState() const;
    void sendSerialStatus();

private:
    DistanceSensor* sensor_;
    SystemState systemState_;
    TimerState timerState_;
    unsigned long openStartMillis_;
    static constexpr unsigned long ALERT_THRESHOLD_MS = 60000; // 60 seconds

    void transitionTo(SystemState newSystem, TimerState newTimer);
    void handleMonitoring();
    void handleAlert();
};