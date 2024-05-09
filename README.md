# Robocricket_Hermes
![thumbnail_d5b9b2e3-0b14-4333-bd85-6a36017eb567](https://github.com/IIT-Neurotechnology/Hermes_FakeCricket/assets/135230111/1d1dafbe-7662-4f35-b5c6-8ebbfd60502e)

## Description
This project is a sophisticated robotic system designed for precision motion control, utilizing a combination of motor encoders, an Inertial Measurement Unit (IMU), and Proportional-Integral-Derivative (PID) controllers for accurate real-time adjustments. It features a modular codebase that includes support for motor speed calculations, IMU data handling, encoder feedback processing, and PID control for optimal motion performance.

## Features
- Precise control over motor speeds using PID controllers.
- Real-time feedback from motor encoders for accurate positioning.
- IMU integration for orientation and motion sensing.
- Modular and easily expandable codebase.

## Hardware Requirements
- Teensy 3.6 Microcontroller
- SparkFun TB6612FNG Motor Driver
- SparkFun 6DoF ISM330DHCX IMU
- Rotary Encoders for motor feedback

## Software Requirements
- Arduino IDE or PlatformIO for compiling and uploading the firmware.
- SparkFun_TB6612 and SparkFun 6DoF ISM330DHCX libraries.

## Installation
1. Install the Arduino IDE or PlatformIO.
2. Clone this repository to your local machine.
3. Install the required libraries through your IDE's library manager.
4. Connect your Teensy 3.6 to your computer.
5. Open the project in your IDE, select the correct board and port, and upload the code.

## Usage
After powering on the system, the robot can be controlled via serial commands to move with specified velocities and orientations. The PID controllers adjust motor speeds based on encoder feedback and IMU data to achieve the desired motion.

## Contributing
Contributions to this project are welcome! Please fork the repository and submit a pull request with your proposed changes.

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments
- Special thanks to the open-source community for providing the libraries and tools used in this project.
- This project utilizes the SparkFun_TB6612 and SparkFun 6DoF ISM330DHCX libraries for motor and IMU integration.
