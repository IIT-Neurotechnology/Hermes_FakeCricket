#include <Arduino.h>
#include <SparkFun_TB6612.h>
#include "cinematica.h"
// these constants are used to allow you to make your motor configuration
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;
const int offsetC = 1;
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
#define PWMC 24


Motor motor1(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2(BIN1, BIN2, PWMB, offsetB, STBY);
Motor motor3(CIN1, CIN2, PWMC, offsetC, STBY2);

    void setup()
{
  // Initialize serial communication
  Serial.begin(9600);
  Serial2.begin(9600); // connected to the ESP8266
}

void loop()
{
  motor1.drive(255, 1000);
  motor1.drive(-255, 1000);
  motor1.brake();
  delay(1000);

  motor2.drive(255, 1000);
  motor2.drive(-255, 1000);
  motor2.brake();
  delay(1000);

  motor3.drive(255, 1000);
  motor3.drive(-255, 1000);
  motor3.brake();
  delay(1000);

}

