#ifndef SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_H
#define SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_H

#include <Arduino.h>
#include "ISSWAnimation.hpp"

#define SSWA_WIDTH 160
#define SSWA_SPECTRUM_SCOPE_WIDTH SSWA_WIDTH
#define SSWA_SPECTRUM_SCOPE_HEIGHT 24
#define SSWA_WATERFALL_WIDTH SSWA_WIDTH
#define SSWA_WATERFALL_HEIGHT 64

#define SSWA_MIN_DIST_BETWEEN_CW_SIGNALS 8
#define SSWA_MAX_DIST_BETWEEN_CW_SIGNALS 12
#define SSWA_TOTAL_CW_SIGNALS_BANDWITH 60  // (first 60 = morse, remaining = analog voice)

#define SSWA_MIN_DIST_BETWEEN_ANALOG_VOICE_SIGNALS 16
#define SSWA_MAX_DIST_BETWEEN_ANALOG_VOICE_SIGNALS 32

#define SSWA_MIN_NOISE_VALUE 0
#define SSWA_MAX_NOISE_VALUE 48

#define SSWA_MIN_SIGNAL_VALUE 80
#define SSWA_MAX_SIGNAL_VALUE 255

class SSWAnimation : public ISSWAnimation {
public:
  virtual void refresh(uint16_t xOffset, uint16_t yOffset) override = 0;
protected:
  uint16_t signalsData[SSWA_WIDTH];
  uint16_t noiseData[SSWA_WIDTH];
  uint16_t signalIndexes[SSWA_WIDTH];
  // generate random noise data
  void refreshNoise(void);
  // generate start/default signals levels
  void initSignals(void);
  // regenerate signals levels
  void refreshSignals(void);
};
#endif
