#ifndef SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_H
#define SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define TOTAL_BANDWITH_RESOLUTION 320 / 2

#define MIN_HZ_STEP_BETWEEN_MORSE_SIGNALS 8
#define MAX_HZ_STEP_BETWEEN_MORSE_SIGNALS 12
#define HZ_LIMIT_MORSE_SIGNALS 60  // (first 60 = morse, remaining = analog voice)

#define MIN_HZ_STEP_BETWEEN_ANALOG_VOICE_SIGNALS 16
#define MAX_HZ_STEP_BETWEEN_ANALOG_VOICE_SIGNALS 32

#define STEP_BETWEEN_LSB_SIGNALS 10
#define DEFAULT_SIGNAL_INIT_VALUE 128

#define MIN_SIGNAL_VALUE 80
#define MAX_SIGNAL_VALUE 255

#define MIN_NOISE_VALUE 0
#define MAX_NOISE_VALUE 48

#define OSCILLOSCOPE_WIDTH TOTAL_BANDWITH_RESOLUTION
#define OSCILLOSCOPE_HEIGHT 24

#define WATERFALL_WIDTH TOTAL_BANDWITH_RESOLUTION
#define WATERFALL_HEIGHT 64

class SSWAnimation {
public:
  //SSWAnimation(Adafruit_ST7789 *existingDisplay);
  ~SSWAnimation();
  void draw(uint16_t xOffset, uint16_t yOffset);
  void refresh(void);
private:
  Adafruit_ST7789* display;
  GFXcanvas16 canvasSpectrumScope;
  GFXcanvas16 canvasWaterFall;

  uint16_t signalsData[TOTAL_BANDWITH_RESOLUTION];
  uint16_t noiseData[TOTAL_BANDWITH_RESOLUTION];
  uint16_t signalIndexes[TOTAL_BANDWITH_RESOLUTION];

  void refreshNoise(void);
  void initSignals(void);
  void refreshSignals(void);
  void moveCanvasDown(GFXcanvas16* canvas);
  uint16_t generateBlueGradientColorFromSignal(uint8_t value);
};

#endif  // SDR_REMOTE_CONTROL_SPECTRUM_SCOPE_WATERFALL_ANIMATION_H
