#ifndef SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_H
#define SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define SSWA_WIDTH 160

class SSWAnimation {
public:
#ifdef DISPLAY_ST7789_240x320
  SSWAnimation(Adafruit_ST7789* existingDisplay);
#endif

#ifdef DISPLAY_ILI9488_480x320
  SSWAnimation(TFT_eSPI* existingDisplay);
#endif
  ~SSWAnimation();
  // refresh animation
  void refresh(uint16_t xOffset, uint16_t yOffset);
private:
#ifdef DISPLAY_ST7789_240x320
  Adafruit_ST7789* display;
  GFXcanvas16* canvasSpectrumScope;
  GFXcanvas16* canvasWaterFall;
#endif
#ifdef DISPLAY_ILI9488_480x320
  TFT_eSPI* ^ display;
  TFT_eSprite* canvasSpectrumScope;
  TFT_eSprite* canvasWaterFall;
#endif

  uint16_t signalsData[SSWA_WIDTH];
  uint16_t noiseData[SSWA_WIDTH];
  uint16_t signalIndexes[SSWA_WIDTH];

  // generate random noise data
  void refreshNoise(void);
  // generate start/default signals levels
  void initSignals(void);
  // regenerate signals levels
  void refreshSignals(void);
  // move canvas one line down
  #ifdef DISPLAY_ST7789_240x320
  void scrollDownWaterFallCanvas(GFXcanvas16* canvas);
  #endif
  #ifdef DISPLAY_ILI9488_480x320
  void scrollDownWaterFallCanvas(TFT_eSPI* canvas);
  #endif
  // generate "blue gradient color" from signal (range value 0..255)
  uint16_t generateBlueGradientColorFromSignal(uint8_t value);
  // paint spectrum scope & waterfall animation
  void draw(uint16_t xOffset, uint16_t yOffset);
};

#endif  // SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_H
