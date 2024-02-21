#include <Arduino.h>
#include <SparkFun_TB6612.h>
#include "cinematica.h"
// these constants are used to allow you to make your motor configuration
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;
const int offsetC = -1;
const int STBY = 9;
const int  STBY2 = 25;

// Pins for all inputs, keep in mind the PWM defines must be on PWM pins
#define AIN1 2
#define BIN1 7
#define AIN2 4
#define BIN2 8
#define PWMA 5
#define PWMB 6
#define STBY 9
#define CIN1 11
#define CIN2 12
#define PWMC 29


Motor motor1(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2(BIN1, BIN2, PWMB, offsetB, STBY);
Motor motor3(CIN1, CIN2, PWMC, offsetC, STBY2);

// Constants and motor setup code...
void setMotorsSpeed(int vx, int vy, int w);

    void setup()
{
  Serial.begin(9600);
  Serial4.begin(115200); // Initialize Serial4 (TX4/RX4) with the baud rate matching the sender
}

void loop()
{
  static bool startFound = false;
  static byte incomingBuffer[8];
  static int bufferIndex = 0;

  while (Serial4.available() > 0)
  {
    byte incomingByte = Serial4.read();

    // Look for start byte
    if (!startFound && incomingByte == 0xAA)
    {
      startFound = true; // Start byte found, start collecting the data
      bufferIndex = 0;   // Reset buffer index
    }
    else if (startFound)
    {
      // Check if we've reached the end byte
      if (incomingByte == 0x55 && bufferIndex == 8)
      {
        // We've found a complete packet, process it
        float vx, vy;
        memcpy(&vx, &incomingBuffer[0], sizeof(vx));
        memcpy(&vy, &incomingBuffer[4], sizeof(vy));

        // Use the velocities as needed
        setMotorsSpeed(vx * 250, vy * 250, 0); // Example usage

        // For debugging, print the velocities to the Serial Monitor
        Serial.print("Velocity X: ");
        Serial.print(vx);
        Serial.print(", Velocity Y: ");
        Serial.println(vy);

        startFound = false; // Reset for the next packet
        continue;           // Skip the rest of the loop
      }
      else if (bufferIndex < 8)
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

// Helper function to set motor speeds based on desired vx, vy, and w
void setMotorsSpeed(int vx, int vy, int w)
{
  std::array<int, 3> motorSpeeds = calculateMotorSpeeds(vx, vy, w);

  Serial.print("Motor 1 Speed: ");
  Serial.println(motorSpeeds[0]);
  Serial.print("Motor 2 Speed: ");
  Serial.println(motorSpeeds[1]);
  Serial.print("Motor 3 Speed: ");
  Serial.println(motorSpeeds[2]);
  motor1.drive(motorSpeeds[0]);
  motor2.drive(motorSpeeds[1]);
  motor3.drive(motorSpeeds[2]);

}

// Drive in direction given by sign, at speed given by magnitude of the
// parameter.
void drive(int speed);
// drive(), but with a delay(duration)
void drive(int speed, int duration);

extern "C"
{
  int _write(int file, char *ptr, int len)
  {
    // Implementation for write
    // For example, you could write data to the serial port
    if (file == 1)
    { // STDOUT
      Serial.write((uint8_t *)ptr, len);
      return len;
    }
    return -1; // Error or unsupported file
  }
}
