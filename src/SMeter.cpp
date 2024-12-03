#include "SMeter.hpp"

SMeter::SMeter(int8_t currentdB)
{
    this->dB = currentdB;
}

SMeter::~SMeter()
{
}

void SMeter::set(int8_t currentdB)
{
    this->dB = currentdB;
}

int8_t SMeter::get(bool smooth)
{
    return (this->dB);
}
