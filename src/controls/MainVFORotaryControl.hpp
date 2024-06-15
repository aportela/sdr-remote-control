#ifndef SDR_REMOTE_CONTROL_MAIN_VFO_ROTARY_CONTROL_H
#define SDR_REMOTE_CONTROL_MAIN_VFO_ROTARY_CONTROL_H

#include <stdint.h>

#include "RotaryControl.hpp"
#include "../../Transceiver.hpp"

class MainVFORotaryControl : public RotaryControl
{
public:
    MainVFORotaryControl(int pinA, int pinB, uint8_t steps, uint8_t acceleration, Transceiver *trxPtr);
    ~MainVFORotaryControl();
    void onChange(int delta) override;

private:
    Transceiver *trx;
};

#endif