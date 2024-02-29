// IMUHandler.cpp
#include "IMUHandler.h"

IMUHandler::IMUHandler() {
    // Constructor can remain empty if there's no initialization needed here
}

void IMUHandler::setupIMU() {
    Wire.begin();

    if (!myISM.begin()) {
        Serial.println("IMU did not begin.");
        while (1);
    }

    myISM.deviceReset();

    // Wait for it to finish resetting
    while (!myISM.getDeviceReset()) {
        delay(1);
    }

    Serial.println("Reset. Applying settings.");
    delay(10);

    // Configuration settings as per your setup function
    myISM.setDeviceConfig();
    myISM.setBlockDataUpdate();
    myISM.setAccelDataRate(ISM_XL_ODR_104Hz);
    myISM.setAccelFullScale(ISM_4g);
    myISM.setGyroDataRate(ISM_GY_ODR_104Hz);
    myISM.setGyroFullScale(ISM_500dps);
    myISM.setAccelFilterLP2();
    myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100);
    myISM.setGyroFilterLP1();
    myISM.setGyroLP1Bandwidth(ISM_MEDIUM);
}

void IMUHandler::readIMU() {
    if (myISM.checkStatus()) {
        myISM.getAccel(&accelData);
        myISM.getGyro(&gyroData);
        
        // Print accelerometer data
        Serial.print("Accelerometer: X: ");
        Serial.print(accelData.xData);
        Serial.print(" Y: ");
        Serial.print(accelData.yData);
        Serial.print(" Z: ");
        Serial.println(accelData.zData);
        
        // Print gyroscope data
        Serial.print("Gyroscope: X: ");
        Serial.print(gyroData.xData);
        Serial.print(" Y: ");
        Serial.print(gyroData.yData);
        Serial.print(" Z: ");
        Serial.println(gyroData.zData);
    }

    delay(100);
}
