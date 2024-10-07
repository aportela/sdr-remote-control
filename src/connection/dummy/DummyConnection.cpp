#include "DummyConnection.hpp"
#include <Arduino.h>

DummyConnection::DummyConnection(HardwareSerial *serialPort, long speed, long timeout)
    : SerialConnection(serialPort, speed, timeout)
{
    // init random seed
    randomSeed(analogRead(0) ^ (micros() * esp_random()));
}

bool DummyConnection::tryConnection(Transceiver *trx)
{
    return (millis() > 0);
}

void DummyConnection::loop(Transceiver *trx, const TransceiverStatus *currentTrxStatus)
{
    if (trx != nullptr && currentTrxStatus != nullptr && !currentTrxStatus->lockedByControls)
    {
        if (millis() % 100 == 0)
        {
            if (this->oldFrequency == 0)
            {
                trx->setVFOMode(0, TRX_VFO_MD_FM);
            }
            this->oldFrequency++;
            if (this->oldFrequency < 1000000000)
            {
                trx->setActiveVFOFrequency(this->oldFrequency);
            }
            if (this->oldSMeter < 20)
            {
                this->oldSMeter += random(0, 10);
            }
            else
            {
                this->oldSMeter -= random(0, 10);
            }
            trx->setSignalMeter(SIGNAL_METER_TS2K_SDR_RADIO_LEVEL, this->oldSMeter);
        }
    }
}

void DummyConnection::syncLocalToRemote(Transceiver *trx, const TransceiverStatus *currentTrxStatus)
{
    if (currentTrxStatus != nullptr)
    {
        if (currentTrxStatus->poweredOn)
        {
        }
    }
}
