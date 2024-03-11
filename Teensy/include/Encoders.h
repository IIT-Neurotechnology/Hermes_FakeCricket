#pragma once
#define ENCODERS_H
#include <Arduino.h>

class EncoderHandler {
public:
    static const int NUM_ENCODERS = 3;
    static EncoderHandler* instance[NUM_ENCODERS]; // Static instances for ISR access

    EncoderHandler(const int pins[]);
    void begin();

    static void isrEncoder1A();
    static void isrEncoder1B();
    static void isrEncoder2A();
    static void isrEncoder2B();
    static void isrEncoder3A();
    static void isrEncoder3B();

    long int getEncoderCount(int index) const;

    static long getPrevEncoderCount1() { return prevEncoderCount1; }
    static long getPrevEncoderCount2() { return prevEncoderCount2; }
    static long getPrevEncoderCount3() { return prevEncoderCount3; }
    static unsigned long getLastSpeedCalculationTime1() { return lastSpeedCalculationTime1; }
    static unsigned long getLastSpeedCalculationTime2() { return lastSpeedCalculationTime2; }
    static unsigned long getLastSpeedCalculationTime3() { return lastSpeedCalculationTime3; }
    static long prevEncoderCount1, prevEncoderCount2, prevEncoderCount3;
    static unsigned long lastSpeedCalculationTime1, lastSpeedCalculationTime2, lastSpeedCalculationTime3;

    // Declare calculateSpeed as a public static member function
    static double calculateSpeed(long currentEncoderCount, long &prevEncoderCount, unsigned long &lastSpeedCalculationTime);

private:
    int encoderPins[NUM_ENCODERS * 2];
    volatile long int encoderCounts[NUM_ENCODERS];
    volatile int encoderPrev[NUM_ENCODERS];
    static const int transitionTable[16];

    void updateEncoder(int index);
};

