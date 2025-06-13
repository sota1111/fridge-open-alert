#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

#include <VL53L0X.h>

class DistanceSensor {
public:
    DistanceSensor(float ts = 0.01, float tau = 0.2); // ts: 10ms, tau: 200ms
    void init();
    void update();
    float getFilteredValue() const;
    uint16_t getRawValue() const;

private:
    void lowPassFilter();

    VL53L0X sensor; // Type: VL53L0X (TOF sensor library class)
    float execution_cycle_ts; // Type: float (Execution cycle)
    float filter_time_constant_tau; // Type: float (Filter time constant)
    float lpf_alpha; // Type: float (Low-pass filter coefficient)

    uint16_t sensor_output; // Type: uint16_t (Unsigned 16-bit integer, raw sensor value)
    float sensor_filtered_value; // Type: float (Filtered sensor value)
    float sensor_filtered_value_prev; // Type: float (Previous filtered sensor value)
}; // Missing closing brace for class

#endif // DISTANCE_SENSOR_H