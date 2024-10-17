#ifndef SDR_REMOTE_CONTROL_ROTARY_CONTROL_H
#define SDR_REMOTE_CONTROL_ROTARY_CONTROL_H

#include <stdint.h>

enum RotaryControlChange
{
    RCC_CLOCKWISE,
    RCC_COUNTERCLOCKWISE
};

typedef void (*RotaryControlCallback)(uint8_t, uint64_t);

class RotaryControl
{
private:
    static uint8_t pinA;
    static uint8_t pinB;
    static RotaryControlCallback RCCallbacks[2];
    static volatile bool ccw1_fall;
    static volatile bool cw1_fall;
    static volatile uint64_t lastEncoderMillis;
    static void RCInterruptHandler(RotaryControlChange rcChange, uint8_t acceleratedDelta, uint64_t lastEncoderMillis);
    static void counterClockWiseCallback(uint8_t pinA, uint8_t pinB);
    static void clockWiseCallback(uint8_t pinA, uint8_t pinB);

public:
    static void init(uint8_t pinA, uint8_t pinB, RotaryControlCallback clockWiseCallback, RotaryControlCallback counterClockWiseCallback);
};

#endif