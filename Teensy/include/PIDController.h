#pragma once
#define PIDController_h

#include "Arduino.h"

class PIDController {
public:
    PIDController(double Kp, double Ki, double Kd);
    void setSetpoint(double setpoint);
    void setLimits(double minOutput, double maxOutput);
    double update(double currentMeasurement);

private:
    double _Kp, _Ki, _Kd;
    double _setpoint;
    double _integral, _lastError;
    unsigned long _lastTime;
    double _minOutput, _maxOutput;
};
