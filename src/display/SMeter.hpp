#ifndef SDR_REMOTE_CONTROL_SMETER_H
#define SDR_REMOTE_CONTROL_SMETER_H

#include <Arduino.h>
#include "ISMeter.hpp"

class SMeter : public ISMeter
{
public:
  virtual void refresh(uint16_t xOffset, uint16_t yOffset, uint8_t level) = 0;

private:
protected:
};
#endif
