#ifndef SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_H
#define SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>


class SSWAnimation {
public:
  SSWAnimation(Adafruit_ST7789 *existingDisplay);
private:
  Adafruit_ST7789 *display;
};

#endif
