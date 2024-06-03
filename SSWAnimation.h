#ifndef SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_H
#define SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define SSWA_WIDTH 160

class SSWAnimation {
public:
  SSWAnimation(Adafruit_ST7789* existingDisplay);
  ~SSWAnimation();
  // refresh animation
  void refresh(uint16_t xOffset, uint16_t yOffset);
private:
  Adafruit_ST7789* display;
  GFXcanvas16* canvasSpectrumScope;
  GFXcanvas16* canvasWaterFall;

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
  void scrollDownWaterFallCanvas(GFXcanvas16* canvas);
  // generate "blue gradient color" from signal (range value 0..255)
  uint16_t generateBlueGradientColorFromSignal(uint8_t value);
  // paint spectrum scope & waterfall animation
  void draw(uint16_t xOffset, uint16_t yOffset);
};

#endif  // SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_H
