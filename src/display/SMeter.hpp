#ifndef SDR_REMOTE_CONTROL_SMETER_H
#define SDR_REMOTE_CONTROL_SMETER_H

#include <Arduino.h>
#include "ISMeter.hpp"

class SMeter : public ISMeter
{
public:
  virtual void refresh(uint8_t level) = 0;

private:
protected:
  uint16_t width = 0;
  uint16_t height = 0;
  uint16_t xOffset = 0;
  uint16_t yOffset = 0;
};
#endif
