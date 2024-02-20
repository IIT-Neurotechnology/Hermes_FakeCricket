#include <Arduino.h>

void setup() {
  Serial.begin(115200); // Start serial communication at 115200 baud rate
}

void loop() {
  // Read joystick values from analog pins
  int joystickX = analogRead(A17); // Joystick X-axis
  int joystickY = analogRead(A19); // Joystick Y-axis

  // Map the analog read values from the joystick (0 to 1023) to velocity (-1.0 to 1.0)
  // Apply dead zone correction: if the absolute value is under 15, set velocity to 0
  float vx = abs(joystickX - 512) < 15 ? 0 : map(joystickX, 0, 1023, -100, 100) / 100.0;
  float vy = abs(joystickY - 512) < 15 ? 0 : map(joystickY, 0, 1023, -100, 100) / 100.0;

  // Package the vx and vy values as binary data
  Serial.write((byte*)&vx, sizeof(vx));
  Serial.write((byte*)&vy, sizeof(vy));
  delay(100);
}
