#include "PIDController.h"

PIDController::PIDController(double Kp, double Ki, double Kd)
: _Kp(Kp), _Ki(Ki), _Kd(Kd), _integral(0), _lastError(0), _minOutput(-255), _maxOutput(255) {
    _lastTime = millis();
}

void PIDController::setSetpoint(double setpoint) {
    _setpoint = setpoint;
}

void PIDController::setLimits(double minOutput, double maxOutput) {
    _minOutput = minOutput;
    _maxOutput = maxOutput;
}

double PIDController::update(double currentMeasurement) {
    unsigned long now = millis();
    double deltaTime = (double)(now - _lastTime) / 1000.0; // time change in seconds
    double error = _setpoint - currentMeasurement;

    // Proportional term
    double Pout = _Kp * error;

    // Integral term
    _integral += error * deltaTime;
    double Iout = _Ki * _integral;

    // Derivative term
    double derivative = (error - _lastError) / deltaTime;
    double Dout = _Kd * derivative;

    // Calculate total output
    double output = Pout + Iout + Dout;

    // Restrict to set output limits
    output = constrain(output, _minOutput, _maxOutput);

    // Save the current error and time for the next update
    _lastError = error;
    _lastTime = now;

    return output;
}
