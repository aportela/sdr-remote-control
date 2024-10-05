#ifndef SDR_REMOTE_CONTROL_LGFX_SPECTRUM_SCOPE_WATERFALL_ANIMATION_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_SPECTRUM_SCOPE_WATERFALL_ANIMATION_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXWidget.hpp"

#define SSWA_SPECTRUM_SCOPE_HEIGHT 24
#define SSWA_WATERFALL_HEIGHT 64

#define SSWA_MIN_DIST_BETWEEN_CW_SIGNALS 8
#define SSWA_MAX_DIST_BETWEEN_CW_SIGNALS 12
#define SSWA_TOTAL_CW_SIGNALS_BANDWITH 60 // (first 60 = morse, remaining = analog voice)

#define SSWA_MIN_DIST_BETWEEN_ANALOG_VOICE_SIGNALS 16
#define SSWA_MAX_DIST_BETWEEN_ANALOG_VOICE_SIGNALS 32

#define SSWA_MIN_NOISE_VALUE 0
#define SSWA_MAX_NOISE_VALUE 48

#define SSWA_MIN_SIGNAL_VALUE 80
#define SSWA_MAX_SIGNAL_VALUE 255

class LGFXSSWAnimationWidget : public LGFXWidget
{
public:
  LGFXSSWAnimationWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding = 0);
  ~LGFXSSWAnimationWidget();
  bool refresh(bool force = false) override;

private:
  lgfx::LGFX_Sprite *canvasSpectrumScope;
  lgfx::LGFX_Sprite *canvasWaterFall;

  uint16_t *signalsData;
  uint16_t *noiseData;
  uint16_t *signalIndexes;
  // generate random noise data
  void refreshNoise(void);
  // generate start/default signals levels
  void initSignals(void);
  // regenerate signals levels
  void refreshSignals(void);
  // move canvas one line down
  void scrollDownWaterFallCanvas(lgfx::LGFX_Sprite *canvas);
  // generate "blue gradient color" from signal (range value 0..255)
  uint16_t generateBlueGradientColorFromSignal(uint8_t value);
  // paint spectrum scope & waterfall animation
  void draw(uint16_t xOffset, uint16_t yOffset);
};

#endif // SDR_REMOTE_CONTROL_LGFX_SPECTRUM_SCOPE_WATERFALL_ANIMATION_WIDGET_H
