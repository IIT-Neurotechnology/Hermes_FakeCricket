#include <Arduino.h>
#include <SparkFun_TB6612.h>
// these constants are used to allow you to make your motor configuration
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;
const int STBY = 9;

// Pins for all inputs, keep in mind the PWM defines must be on PWM pins
#define AIN1 2
#define BIN1 7
#define AIN2 4
#define BIN2 8
#define PWMA 5
#define PWMB 6
#define STBY 9

Motor motor1(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2(BIN1, BIN2, PWMB, offsetB, STBY);

void setup()
{
  // Initialize serial communication
  Serial.begin(9600);
  Serial2.begin(9600); // connected to the ESP8266
}

void loop()
{
  // Use of the drive function which takes as arguements the speed
  // and optional duration.  A negative speed will cause it to go
  // backwards.  Speed can be from -255 to 255.  Also use of the
  // brake function which takes no arguements.
  motor1.drive(255, 1000);
  motor1.drive(-255, 1000);
  motor1.brake();
  delay(1000);

  // Use of the drive function which takes as arguements the speed
  // and optional duration.  A negative speed will cause it to go
  // backwards.  Speed can be from -255 to 255.  Also use of the
  // brake function which takes no arguements.
  motor2.drive(255, 1000);
  motor2.drive(-255, 1000);
  motor2.brake();
  delay(1000);

  // Use of the forward function, which takes as arguements two motors
  // and optionally a speed.  If a negative number is used for speed
  // it will go backwards
  forward(motor1, motor2, 150);
  delay(1000);

  // Use of the back function, which takes as arguments two motors
  // and optionally a speed.  Either a positive number or a negative
  // number for speed will cause it to go backwards
  back(motor1, motor2, -150);
  delay(1000);

  // Use of the brake function which takes as arguments two motors.
  // Note that functions do not stop motors on their own.
  brake(motor1, motor2);
  delay(1000);

  // Use of the left and right functions which take as arguements two
  // motors and a speed.  This function turns both motors to move in
  // the appropriate direction.  For turning a single motor use drive.
  left(motor1, motor2, 100);
  delay(1000);
  right(motor1, motor2, 100);
  delay(1000);

  // Use of brake again.
  brake(motor1, motor2);
  delay(1000);
}

// Drive in direction given by sign, at speed given by magnitude of the
// parameter.
void drive(int speed);
// drive(), but with a delay(duration)
void drive(int speed, int duration);

// currently not implemented
// void stop();           // Stop motors, but allow them to coast to a halt.
// void coast();          // Stop motors, but allow them to coast to a halt.

// Stops motor by setting both input pins high
void brake();

// set the chip to standby mode.  The drive function takes it out of standby
//(forward, back, left, and right all call drive)
void standby();

// Takes 2 motors and goes forward, if it does not go forward adjust offset
// values until it does.  These will also take a negative number and go backwards
// There is also an optional speed input, if speed is not used, the function will
// use the DEFAULTSPEED constant.
void forward(Motor motor1, Motor motor2, int speed);
void forward(Motor motor1, Motor motor2);

// Similar to forward, will take 2 motors and go backwards.  This will take either
// a positive or negative number and will go backwards either way.  Once again the
// speed input is optional and will use DEFAULTSPEED if it is not defined.
void back(Motor motor1, Motor motor2, int speed);
void back(Motor motor1, Motor motor2);

// Left and right take 2 motors, and it is important the order they are sent.
// The left motor should be on the left side of the bot.  These functions
// also take a speed value
void left(Motor left, Motor right, int speed);
void right(Motor left, Motor right, int speed);

// This function takes 2 motors and and brakes them
void brake(Motor motor1, Motor motor2);
