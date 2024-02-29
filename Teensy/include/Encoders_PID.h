#pragma once
#define ENCODERS_PID_H

#include <Arduino.h>

class EncoderHandler {
public:
    static const int NUM_ENCODERS = 3; // Adjust based on the number of encoders used
    static EncoderHandler* instance[NUM_ENCODERS]; // Static instances for ISR access

    // Constructor that initializes the encoder pins
    EncoderHandler(const int pins[]);

    // Method to set up the encoder pins and attach interrupts
    void begin();

    // Static ISR handlers for each encoder pin
    static void isrEncoder0A();
    static void isrEncoder0B();
    static void isrEncoder1A();
    static void isrEncoder1B();
    static void isrEncoder2A();
    static void isrEncoder2B();
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
