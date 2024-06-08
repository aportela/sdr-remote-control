#ifndef SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_INTERFACE_H
#define SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_INTERFACE_H

#include <stdint.h>

class ISSWAnimation {
public:
  virtual ~ISSWAnimation() {}
  virtual void refresh(uint16_t xOffset, uint16_t yOffset) = 0;
};

#endif
