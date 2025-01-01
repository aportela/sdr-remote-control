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
    uint8_t pinA;
    uint8_t pinB;
    RotaryControlCallback RCCallbacks[2];
    volatile bool ccw1_fall;
    volatile bool cw1_fall;
    volatile uint64_t lastEncoderMillis;

    static void globalInterruptHandler(void *arg);

    void handleInterrupt(RotaryControlChange rcChange);

public:
    RotaryControl(uint8_t pinA, uint8_t pinB, RotaryControlCallback clockWiseCallback, RotaryControlCallback counterClockWiseCallback);
    ~RotaryControl();
};

#endif // SDR_REMOTE_CONTROL_ROTARY_CONTROL_H
