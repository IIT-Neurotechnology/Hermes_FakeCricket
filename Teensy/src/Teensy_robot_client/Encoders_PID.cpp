
#include "Encoders_PID.h"

// Initialize static instance pointers to nullptr
EncoderHandler* EncoderHandler::instance[EncoderHandler::NUM_ENCODERS] = {nullptr};

// Transition table for quadrature encoders
const int EncoderHandler::transitionTable[] = {
    0, 1, -1, 0,
    -1, 0, 0, 1,
    1, 0, 0, -1,
    0, -1, 1, 0
};

// Constructor: Initializes encoder pins and zeros counts and previous states
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
            attachInterrupt(digitalPinToInterrupt(encoderPins[0]), isrEncoder0A, CHANGE);
            attachInterrupt(digitalPinToInterrupt(encoderPins[1]), isrEncoder0B, CHANGE);
        } else if (i == 1) {
            attachInterrupt(digitalPinToInterrupt(encoderPins[2]), isrEncoder1A, CHANGE);
            attachInterrupt(digitalPinToInterrupt(encoderPins[3]), isrEncoder1B, CHANGE);
        } else if (i == 2) {
            attachInterrupt(digitalPinToInterrupt(encoderPins[4]), isrEncoder2A, CHANGE);
            attachInterrupt(digitalPinToInterrupt(encoderPins[5]), isrEncoder2B, CHANGE);
        }
        // Add additional else-if blocks for more encoders if necessary
    }
}

// Static ISR methods for each encoder pin
void EncoderHandler::isrEncoder0A() { if (instance[0]) instance[0]->updateEncoder(0); }
void EncoderHandler::isrEncoder0B() { if (instance[0]) instance[0]->updateEncoder(0); }
void EncoderHandler::isrEncoder1A() { if (instance[1]) instance[1]->updateEncoder(1); }
void EncoderHandler::isrEncoder1B() { if (instance[1]) instance[1]->updateEncoder(1); }
void EncoderHandler::isrEncoder2A() { if (instance[2]) instance[2]->updateEncoder(2); }
void EncoderHandler::isrEncoder2B() { if (instance[2]) instance[2]->updateEncoder(2); }
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
