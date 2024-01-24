// my_functions.cpp
#include "cinematica.h"
#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>

// Function to calculate motor speeds
std::array<int, 3> calculateMotorSpeeds(int vx, int vy, int w)
{
    // Constants for wheel angles and robot dimensions
    const double alpha0 = 0;       // angle for motor 1
    const double alpha1 = 2.0944;  // 120 degrees in radians for motor 2
    const double alpha2 = -2.0944; // -120 degrees in radians for motor 3
    const double beta = 0.523599;  // 30 degrees in radians
    const double R = 1;            // Assuming R = 1 for direct proportion

    // Calculate speeds for each motor
    double motor1Speed = vx * sin(alpha0 + beta) + vy * cos(alpha0 + beta) + R * w;
    double motor2Speed = vx * sin(alpha1 + beta) + vy * cos(alpha1 + beta) + R * w;
    double motor3Speed = vx * sin(alpha2 + beta) + vy * cos(alpha2 + beta) + R * w;

    // Find the maximum absolute speed
    double maxSpeed = std::max({std::abs(motor1Speed), std::abs(motor2Speed), std::abs(motor3Speed), 255.0});

    // Normalize speeds to -255 to 255 range
    motor1Speed = (motor1Speed / maxSpeed) * 255;
    motor2Speed = (motor2Speed / maxSpeed) * 255;
    motor3Speed = (motor3Speed / maxSpeed) * 255;

    // Cast to int and return as array
    return {static_cast<int>(motor1Speed), static_cast<int>(motor2Speed), static_cast<int>(motor3Speed)};
}
