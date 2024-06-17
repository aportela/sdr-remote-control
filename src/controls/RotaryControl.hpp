#ifndef SDR_REMOTE_CONTROL_ROTARY_CONTROL_H
#define SDR_REMOTE_CONTROL_ROTARY_CONTROL_H

#include <AiEsp32RotaryEncoder.h>

#include <stdint.h>

class RotaryControl
{
public:
    RotaryControl(int pinA, int pinB, uint8_t steps, uint8_t acceleration);
    ~RotaryControl();
    virtual void onChange(int delta) = 0;
    AiEsp32RotaryEncoder *encoder;
    void onUpdate(void);
};

#endif