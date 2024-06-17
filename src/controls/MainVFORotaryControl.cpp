#include "MainVFORotaryControl.hpp"

MainVFORotaryControl::MainVFORotaryControl(int pinA, int pinB, uint8_t steps, uint8_t acceleration, Transceiver *trxPtr) : RotaryControl(pinA, pinB, steps, acceleration)
{
    this->trx = trxPtr;
}

MainVFORotaryControl::~MainVFORotaryControl()
{
    this->trx = nullptr;
}

void MainVFORotaryControl::onChange(int delta)
{
    this->trx->VFO[this->trx->activeVFOIndex].frequency += delta;
    this->trx->changed |= TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
}