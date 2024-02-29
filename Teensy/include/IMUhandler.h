#pragma once

#include <Arduino.h>
#include "SparkFun_ISM330DHCX.h" // Include the library for the IMU

class IMUHandler {
public:
    IMUHandler(); // Constructor
    void setupIMU(); // Method to setup the IMU
    void readIMU(); // Method to read data from the IMU
private:
    SparkFun_ISM330DHCX myISM; // IMU object
    sfe_ism_data_t accelData; // Struct for accelerometer data
    sfe_ism_data_t gyroData; // Struct for gyroscope data
};
