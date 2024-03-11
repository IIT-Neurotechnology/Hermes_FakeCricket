#include "Encoders.h"

// Initialize static instance pointers to nullptr
EncoderHandler* EncoderHandler::instance[EncoderHandler::NUM_ENCODERS] = {nullptr};

long EncoderHandler::prevEncoderCount1 = 0, EncoderHandler::prevEncoderCount2 = 0, EncoderHandler::prevEncoderCount3 = 0;
unsigned long EncoderHandler::lastSpeedCalculationTime1 = 0, EncoderHandler::lastSpeedCalculationTime2 = 0, EncoderHandler::lastSpeedCalculationTime3 = 0;

// Transition table for quadrature encoders
const int EncoderHandler::transitionTable[] = {
    0, 1, -1, 0,
    -1, 0, 0, 1,
    1, 0, 0, -1,
    0, -1, 1, 0
};
EncoderHandler::EncoderHandler(const int pins[]) {
    for (int i = 0; i < NUM_ENCODERS * 2; ++i) {
        this->encoderPins[i] = pins[i];
    }
    memset((void*)this->encoderCounts, 0, sizeof(this->encoderCounts));
    memset((void*)this->encoderPrev, 0, sizeof(this->encoderPrev));
}

// Begin method: Sets up pin modes and attaches interrupts
void EncoderHandler::begin() {
    for (int i = 0; i < NUM_ENCODERS; ++i) {
        pinMode(encoderPins[i * 2], INPUT);
        pinMode(encoderPins[i * 2 + 1], INPUT);
        instance[i] = this; // Assign this instance for static access from ISR

        // Attach interrupts for both pins of each encoder
        if (i == 0) {
            attachInterrupt(digitalPinToInterrupt(encoderPins[0]), isrEncoder1A, CHANGE);
            attachInterrupt(digitalPinToInterrupt(encoderPins[1]), isrEncoder1B, CHANGE);
        } else if (i == 1) {
            attachInterrupt(digitalPinToInterrupt(encoderPins[2]), isrEncoder2A, CHANGE);
            attachInterrupt(digitalPinToInterrupt(encoderPins[3]), isrEncoder2B, CHANGE);
        } else if (i == 2) {
            attachInterrupt(digitalPinToInterrupt(encoderPins[4]), isrEncoder3A, CHANGE);
            attachInterrupt(digitalPinToInterrupt(encoderPins[5]), isrEncoder3B, CHANGE);
        }
        // Add additional else-if blocks for more encoders if necessary
    }
}

double EncoderHandler::calculateSpeed(long currentEncoderCount, long &prevEncoderCount, unsigned long &lastSpeedCalculationTime) {
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

// Static ISR methods for each encoder pin
void EncoderHandler::isrEncoder1A() { if (instance[0]) instance[0]->updateEncoder(0); }
void EncoderHandler::isrEncoder1B() { if (instance[0]) instance[0]->updateEncoder(0); }
void EncoderHandler::isrEncoder2A() { if (instance[1]) instance[1]->updateEncoder(1); }
void EncoderHandler::isrEncoder2B() { if (instance[1]) instance[1]->updateEncoder(1); }
void EncoderHandler::isrEncoder3A() { if (instance[2]) instance[2]->updateEncoder(2); }
void EncoderHandler::isrEncoder3B() { if (instance[2]) instance[2]->updateEncoder(2); }
// Add more static ISR methods for additional encoders as needed

// updateEncoder: Updates the count for the specified encoder
void EncoderHandler::updateEncoder(int index) {
    noInterrupts();
    int stateA = digitalRead(encoderPins[2 * index]);
    int stateB = digitalRead(encoderPins[2 * index + 1]);
    int portValue = stateA | (stateB << 1);
    int currentValue = portValue;
    int prevValue = encoderPrev[index];
    int tableIdx = (currentValue & 0x03) | (prevValue & 0x03) << 2;
    encoderCounts[index] += transitionTable[tableIdx];
    encoderPrev[index] = portValue;
    interrupts();
}

// getEncoderCount: Returns the current count for the specified encoder
long int EncoderHandler::getEncoderCount(int index) const {
    long int count;
    noInterrupts();
    count = encoderCounts[index];
    interrupts();
    return count;
}
