#ifndef SDR_REMOTE_CONTROL_SMETER_INTERFACE
#define SDR_REMOTE_CONTROL_SMETER_INTERFACE

#include <stdint.h>

class ISMeter
{
public:
  virtual ~ISMeter() {}
  virtual void refresh(uint8_t level) = 0;
};

#endif