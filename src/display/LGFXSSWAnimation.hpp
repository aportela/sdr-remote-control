#ifndef SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_ST7789_H
#define SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_ST7789_H

#include <LovyanGFX.hpp>

#include "SSWAnimation.hpp"

class LGFXSSWAnimation : public SSWAnimation
{
public:
  LGFXSSWAnimation(LovyanGFX *existingDisplay, uint16_t width);
  ~LGFXSSWAnimation();
  void refresh(uint16_t xOffset, uint16_t yOffset) override;

private:
  LovyanGFX *display;
  lgfx::LGFX_Sprite *canvasSpectrumScope;
  lgfx::LGFX_Sprite *canvasWaterFall;
  // move canvas one line down
  void scrollDownWaterFallCanvas(lgfx::LGFX_Sprite *canvas);
  // generate "blue gradient color" from signal (range value 0..255)
  uint16_t generateBlueGradientColorFromSignal(uint8_t value);
  // paint spectrum scope & waterfall animation
  void draw(uint16_t xOffset, uint16_t yOffset);
};

#endif
