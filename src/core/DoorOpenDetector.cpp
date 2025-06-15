#include "DoorOpenDetector.h"
#include <M5Unified.h>

DoorOpenDetector::DoorOpenDetector(DistanceSensor* sensor)
    : sensor_(sensor),
      systemState_(SystemState::Monitoring),
      timerState_(TimerState::Stopped),
      openStartMillis_(0) {}

void DoorOpenDetector::update() {
    switch (systemState_) {
        case SystemState::Monitoring:
            handleMonitoring();
            break;
        case SystemState::Alert:
            handleAlert();
            break;
    }
}

void DoorOpenDetector::handleMonitoring() {
    switch (timerState_) {
        case TimerState::Stopped:
            if (sensor_->isAboveThreshold()) {
                transitionTo(SystemState::Monitoring, TimerState::Counting);
            } else {
                doAction(systemState_, timerState_);
            }
            break;
        case TimerState::Counting:
            if (!sensor_->isAboveThreshold()) {
                transitionTo(SystemState::Monitoring, TimerState::Stopped);
            } else {
                if (millis() - openStartMillis_ >= ALERT_THRESHOLD_MS) {
                    transitionTo(SystemState::Alert, TimerState::Stopped);
                } else {
                    doAction(systemState_, timerState_);
                }
            }
            break;
    }
}

void DoorOpenDetector::handleAlert() {
    if (!sensor_->isAboveThreshold()) {
        transitionTo(SystemState::Monitoring, TimerState::Stopped);
    } else {
        doAction(systemState_, timerState_);
    }
}

void DoorOpenDetector::transitionTo(SystemState newSystem, TimerState newTimer) {
    exit(systemState_, timerState_);
    systemState_ = newSystem;
    timerState_ = newTimer;
    entry(systemState_, timerState_);

}

void DoorOpenDetector::reset() {
    systemState_ = SystemState::Monitoring;
    timerState_ = TimerState::Stopped;
    openStartMillis_ = 0;
}

void DoorOpenDetector::entry(SystemState system, TimerState timer) {
    // Reset timer on monitoring entry
    if (system == SystemState::Monitoring && timer == TimerState::Counting) {
        openStartMillis_ = millis();
    }
}

void DoorOpenDetector::exit(SystemState system, TimerState timer) {

}

// alert????r?[?v??????
void DoorOpenDetector::doAction(SystemState system, TimerState timer) {
    if (system == SystemState::Alert) {
        M5.Speaker.tone(4000, 800);
    }
}

unsigned long DoorOpenDetector::getOpenDurationMillis() const {
    if (timerState_ == TimerState::Counting) {
        return millis() - openStartMillis_;
    }
    return 0;
}