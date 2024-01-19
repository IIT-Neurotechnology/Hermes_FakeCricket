#include <Arduino.h>

// Function declaration
int myFunction(int, int);

// Global variable
int result;

void setup()
{
  // Initialize serial communication
  Serial.begin(9600);

  // Compute result
  result = myFunction(2, 3);
}

void loop()
{
  // Print to serial monitor
  Serial.print("Ciao, result is: ");
  Serial.println(result);

  // Add a delay to make the output readable
  delay(1000); // Delay for 1 second
}

// Function definition
int myFunction(int x, int y)
{
  return x + y;
}
