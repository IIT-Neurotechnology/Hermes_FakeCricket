#pragma once
#define ENCODERS_H

#include <Arduino.h>

class EncoderHandler {
public:
    static const int NUM_ENCODERS = 3;
    static EncoderHandler* instance[NUM_ENCODERS]; // Static instances for ISR access

    // Constructor that initializes the encoder pins
    EncoderHandler(const int pins[]);

    // Method to set up the encoder pins and attach interrupts
    void begin();

    // Static ISR handlers for each encoder pin
    static void isrEncoder1A();
    static void isrEncoder1B();
    static void isrEncoder2A();
    static void isrEncoder2B();
    static void isrEncoder3A();
    static void isrEncoder3B();
    // Add more ISR handlers if you have more than 3 encoders

    // Method to get the current count of the specified encoder
    long int getEncoderCount(int index) const;

private:
    int encoderPins[NUM_ENCODERS * 2]; // Array to hold the pin numbers for each encoder
    volatile long int encoderCounts[NUM_ENCODERS]; // Array to hold the count of each encoder
    volatile int encoderPrev[NUM_ENCODERS]; // Array to hold the previous state of each encoder
    static const int transitionTable[16]; // State transition table for quadrature decoding

    // Method to update the encoder count, called by the ISR handlers
    void updateEncoder(int index);
};

// Previous encoder counts for each motor
long prevEncoderCount1 = 0, prevEncoderCount2 = 0, prevEncoderCount3 = 0;
// Last time the speed was calculated for each motor, in milliseconds
unsigned long lastSpeedCalculationTime1 = 0, lastSpeedCalculationTime2 = 0, lastSpeedCalculationTime3 = 0;

double calculateSpeed(long currentEncoderCount, long &prevEncoderCount, unsigned long &lastSpeedCalculationTime) {
  unsigned long currentTime = millis();
  unsigned long timeDelta = currentTime - lastSpeedCalculationTime; // Time since last speed calculation in milliseconds
  
  if(timeDelta > 0) { // Prevent division by zero
    long countDelta = currentEncoderCount - prevEncoderCount; // Change in encoder count
    double speed = (countDelta / (double)timeDelta) * 1000.0; // Speed in counts per second
    
    // Update for next calculation
    prevEncoderCount = currentEncoderCount;
    lastSpeedCalculationTime = currentTime;
    
    return speed;
  }
  
  return 0; // Return 0 if called again before time has passed
}

