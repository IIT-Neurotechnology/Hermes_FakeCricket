#define Kp 0.012   // Proportional Gain
#define Ki 0.00    // Integral Gain
#define Kd 0.00003 // Derivative Gain
#define NUM_ENCODERS 3

double setPoints[NUM_ENCODERS] = {0, 0, 0}; // Desired speeds for each motor
double inputs[NUM_ENCODERS], outputs[NUM_ENCODERS], lastInputs[NUM_ENCODERS];
// For each motor
double errorSum[NUM_ENCODERS] = {0, 0, 0}, lastErr[NUM_ENCODERS];
double err = 0;

int encoder_pin[] = {20, 21, 22, 23, 16, 17};

static const int transition_table[] = {
    0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0};