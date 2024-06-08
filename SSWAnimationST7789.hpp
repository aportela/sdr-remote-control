#ifndef SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_ST7789_H
#define SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_ST7789_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include "SSWAnimation.hpp"

class SSWAnimationST7789 : public SSWAnimation {
public:
  SSWAnimationST7789(Adafruit_ST7789* existingDisplay);
  ~SSWAnimationST7789();
  void refresh(uint16_t xOffset, uint16_t yOffset) override;
private:
  Adafruit_ST7789* display;
  GFXcanvas16* canvasSpectrumScope;
  GFXcanvas16* canvasWaterFall;
  // move canvas one line down
  void scrollDownWaterFallCanvas(GFXcanvas16* canvas);
  // generate "blue gradient color" from signal (range value 0..255)
  uint16_t generateBlueGradientColorFromSignal(uint8_t value);
  // paint spectrum scope & waterfall animation
  void draw(uint16_t xOffset, uint16_t yOffset);
};

#endif
