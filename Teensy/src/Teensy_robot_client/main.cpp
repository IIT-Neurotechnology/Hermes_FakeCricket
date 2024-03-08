#include <Arduino.h>
#include <SparkFun_TB6612.h>
#include "cinematica.h"
#include "IMUHandler.h"
#include "Encoders.h"
#include "PIDController.h"

const int encoderPins[] = {20, 21, 22, 23, 16, 17}; 
EncoderHandler encoder(encoderPins);


// these constants are used to allow you to make your motor configuration
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;
const int offsetC = -1;
const int STBY = 9;

IMUHandler imu;

#define AIN1 2
#define BIN1 7
#define AIN2 4
#define BIN2 8
#define PWMA 5
#define PWMB 6
#define STBY 9
#define CIN1 11
#define CIN2 12
#define PWMC 36

Motor motor1(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2(BIN1, BIN2, PWMB, offsetB, STBY);
Motor motor3(CIN1, CIN2, PWMC, offsetC, STBY);

PIDController motor1PID(2.0, 5.0, 1.0);
PIDController motor2PID(2.5, 4.5, 1.2);
PIDController motor3PID(2.2, 4.8, 1.1);

void setMotorsSpeed(int vx, int vy, int w);
void setMotorsSpeed_PID(int vx, int vy, int w,double currentSpeed1, double currentSpeed2, double currentSpeed3);

    void setup()
{
  Serial1.begin(115200);
  encoder.begin();
  //imu.setupIMU();
}

void loop()
{
  static bool startFound = false;
  static byte incomingBuffer[12];
  static int bufferIndex = 0;
  double currentSpeed1 = calculateSpeed(encoder.getEncoderCount(0),prevEncoderCount1,lastSpeedCalculationTime1);
  double currentSpeed2 = calculateSpeed(encoder.getEncoderCount(1),prevEncoderCount2,lastSpeedCalculationTime2);
  double currentSpeed3 = calculateSpeed(encoder.getEncoderCount(2),prevEncoderCount3,lastSpeedCalculationTime3);
  while (Serial1.available() > 0)
  {
    byte incomingByte = Serial1.read();

    // Look for start byte
    if (!startFound && incomingByte == 0xAA)
    {
      startFound = true; // Start byte found, start collecting the data
      bufferIndex = 0;   // Reset buffer index
    }
    else if (startFound)
    {
      // Check if we've reached the end byte
      if (incomingByte == 0x55 && bufferIndex == 12)
      {
        // We've found a complete packet, process it
        float vx, vy, w;
        memcpy(&vx, &incomingBuffer[0], sizeof(vx));
        memcpy(&vy, &incomingBuffer[4], sizeof(vy));
        memcpy(&w, &incomingBuffer[8], sizeof(w));

        // Use the velocities as needed
        setMotorsSpeed_PID(vx * 250, vy * 250, w, currentSpeed1, currentSpeed2, currentSpeed3);
        startFound = false; // Reset for the next packet
        continue;           // Skip the rest of the loop
      }
      else if (bufferIndex < 12)
      {
        // Collect data into buffer
        incomingBuffer[bufferIndex++] = incomingByte;
      }
      else
      {
        // If we get here, something went wrong (we never found the end byte)
        startFound = false; // Reset and look for a new start byte
      }
    }
  }
}

void setMotorsSpeed(int vx, int vy, int w)
{
  std::array<int, 3> motorSpeeds = calculateMotorSpeeds(vx, vy, w);
  motor1.drive(motorSpeeds[0]);
  motor2.drive(motorSpeeds[1]);
  motor3.drive(motorSpeeds[2]);

}

void setMotorsSpeed_PID(int vx, int vy, int w,double currentSpeed1, double currentSpeed2, double currentSpeed3)
{
  std::array<int, 3> motorSpeeds = calculateMotorSpeeds(vx, vy, w);
  motor1PID.setSetpoint(motorSpeeds[0]);
  motor2PID.setSetpoint(motorSpeeds[1]);
  motor3PID.setSetpoint(motorSpeeds[2]);
  double controlSignal1 = motor1PID.update(currentSpeed1);
  double controlSignal2 = motor2PID.update(currentSpeed2);
  double controlSignal3 = motor3PID.update(currentSpeed3);
  motor1.drive(controlSignal1);
  motor2.drive(controlSignal2);
  motor3.drive(controlSignal3);

}

void drive(int speed);
void drive(int speed, int duration);

extern "C"
{
  int _write(int file, char *ptr, int len)
  {
    // Implementation for write
    // For example, you could write data to the serial port
    if (file == 1)
    {
      Serial.write((uint8_t *)ptr, len);
      return len;
    }
    return -1; // Error or unsupported file
  }
}
