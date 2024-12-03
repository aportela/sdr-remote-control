#include "SMeter.hpp"
#include <Arduino.h>

SMeter::SMeter(int8_t currentdB)
{
    this->set(currentdB);
    this->dBSMoothed = this->dB;
    this->lastSmoothTimestamp = millis();
}

SMeter::~SMeter()
{
}

void SMeter::set(int8_t currentdB)
{
    if (currentdB >= SMETER_MIN_DB && currentdB <= SMETER_MAX_DB)
    {
        this->dB = currentdB;
    }
    else
    {
        if (currentdB < SMETER_MIN_DB)
        {
            this->dB = SMETER_MIN_DB;
        }
        else
        {
            this->dB = SMETER_MAX_DB;
        }
    }
}

int8_t SMeter::get(bool smooth)
{
    if (smooth)
    {
        uint64_t diff = millis() - this->lastSmoothTimestamp;
        // "smooth" increase required
        if (this->dBSMoothed < this->dB && diff > SMETER_SMOOTH_INCREASE_MS_INTERVAL)
        {
            this->dBSMoothed++;
            this->lastSmoothTimestamp = millis();
        }
        // "smooth" decrease required
        else if (this->dBSMoothed > this->dB && diff > SMETER_SMOOTH_DECREASE_MS_INTERVAL)
        {
            this->dBSMoothed--;
            this->lastSmoothTimestamp = millis();
        }
        return (this->dBSMoothed);
    }
    else
    {
        return (this->dB);
    }
}
