#ifndef SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_ST7789_H
#define SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_ST7789_H

#include <SPI.h>
#include <TFT_eSPI.h>

#include "SSWAnimation.h"

class SSWAnimationILI9488 : public SSWAnimation {
public:
  SSWAnimationILI9488(TFT_eSPI* existingDisplay);
  ~SSWAnimationILI9488();
  void refresh(uint16_t xOffset, uint16_t yOffset) override;
private:
  TFT_eSPI* display;
  TFT_eSprite* canvasSpectrumScope;
  TFT_eSprite* canvasWaterFall;
  // move canvas one line down
  void scrollDownWaterFallCanvas(TFT_eSprite* canvas);
  // generate "blue gradient color" from signal (range value 0..255)
  uint16_t generateBlueGradientColorFromSignal(uint8_t value);
  // paint spectrum scope & waterfall animation
  void draw(uint16_t xOffset, uint16_t yOffset);
};

#endif
