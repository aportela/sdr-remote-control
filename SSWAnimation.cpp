#include <stdint.h>
#include "SSWAnimation.h"

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

SSWAnimation::SSWAnimation(Adafruit_ST7789* existingDisplay) {
  if (existingDisplay != nullptr) {
    // init random seed
    randomSeed(analogRead(0) ^ (micros() * esp_random()));
    this->display = existingDisplay;
    this->canvasSpectrumScope = new GFXcanvas16(SSWA_SPECTRUM_SCOPE_WIDTH, SSWA_SPECTRUM_SCOPE_HEIGHT);
    this->canvasWaterFall = new GFXcanvas16(SSWA_WATERFALL_WIDTH, SSWA_WATERFALL_HEIGHT);
    this->refreshNoise();
    this->initSignals();
  }
}

SSWAnimation::~SSWAnimation() {
  delete this->canvasSpectrumScope;
  this->canvasSpectrumScope = nullptr;
  delete this->canvasWaterFall;
  this->canvasWaterFall = nullptr;
}

// generate random noise data
void SSWAnimation::refreshNoise(void) {
  const uint8_t MAX_VALUE = SSWA_MIN_SIGNAL_VALUE / 2;
  const uint8_t MIN_PEAK_INTERVAL = 10;
  const uint8_t MAX_PEAK_INTERVAL = 30;
  for (uint16_t i = 0; i < SSWA_WIDTH; i++) {
    uint8_t currentValue = random(0, SSWA_MAX_NOISE_VALUE);
    uint8_t nextPeak = random(MIN_PEAK_INTERVAL, MAX_PEAK_INTERVAL);
    if (i % nextPeak == 0) {
      uint8_t peakValue = random(SSWA_MIN_NOISE_VALUE, SSWA_MAX_NOISE_VALUE);
      int8_t direction = random(0, 2) * 2 - 1;
      for (int j = i; j < i + nextPeak && j < SSWA_WIDTH; j++) {
        this->noiseData[j] = max(0, min(255, currentValue + direction * abs(currentValue - peakValue) / nextPeak * (j - i)));
      }
      currentValue = peakValue;
      nextPeak = random(MIN_PEAK_INTERVAL, MAX_PEAK_INTERVAL);
    }
  }
}

// generate start/default signals levels
void SSWAnimation::initSignals(void) {
  // left block are "cw/morse" signals
  // random spacing between signals
  uint16_t currentSignalIndex = random(SSWA_MIN_DIST_BETWEEN_CW_SIGNALS, SSWA_MAX_DIST_BETWEEN_CW_SIGNALS);
  for (uint16_t i = 0; i < SSWA_TOTAL_CW_SIGNALS_BANDWITH; i++) {
    // set by default that this is a "noise" (not real signal)
    this->signalIndexes[i] = 0;
    if (i != 0) {
      // when reach next signal index
      if (i == currentSignalIndex) {
        // generate signal level
        this->signalsData[i] = random(SSWA_MIN_SIGNAL_VALUE, SSWA_MAX_SIGNAL_VALUE);
        // change "noise" to -> "signal"
        this->signalIndexes[i] = 1;
        // generate next signal index
        currentSignalIndex += random(SSWA_MIN_DIST_BETWEEN_CW_SIGNALS, SSWA_MAX_DIST_BETWEEN_CW_SIGNALS);
      } else {
        // no signal at this index => get "noise" data
        this->signalsData[i] = this->noiseData[i];
      }
    } else {
      // first index (0) => get "noise" data
      this->signalsData[i] = this->noiseData[i];
    }
  }

  // right block are "analog voice" signals
  // random spacing between signals
  currentSignalIndex = SSWA_TOTAL_CW_SIGNALS_BANDWITH + random(SSWA_MIN_DIST_BETWEEN_ANALOG_VOICE_SIGNALS, SSWA_MAX_DIST_BETWEEN_ANALOG_VOICE_SIGNALS);
  // generate signal level
  uint8_t currentSignalValue = random(SSWA_MIN_SIGNAL_VALUE, SSWA_MAX_SIGNAL_VALUE);
  // set "noise" (not real signal) by default
  for (uint16_t i = SSWA_TOTAL_CW_SIGNALS_BANDWITH; i < SSWA_WIDTH; i++) {
    this->signalIndexes[i] = 0;
  }
  for (uint16_t i = SSWA_TOTAL_CW_SIGNALS_BANDWITH; i < SSWA_WIDTH; i++) {
    if (i != 0) {
      // when reach next signal index
      if (i == currentSignalIndex) {
        // allow near signal indexes
        if (i > 4 && i < SSWA_WIDTH - 4) {
          // set signal level
          this->signalsData[i] = currentSignalValue;
          // analog voice signal requires more bandwith than "cw/morse" so re-generate also near signal indexes with random values
          this->signalsData[i - 1] = constrain(this->signalsData[i] - random(1, 4), 0, 255);
          this->signalsData[i - 2] = constrain(this->signalsData[i - 1] - random(1, 8), 0, 255);
          this->signalsData[i - 3] = constrain(this->signalsData[i - 2] - random(1, 16), 0, 255);
          this->signalsData[i - 4] = constrain(this->signalsData[i - 3] - random(1, 32), 0, 255);
          this->signalsData[i - 5] = constrain(this->signalsData[i - 4] - random(1, 64), 0, 255);
          this->signalsData[i + 1] = constrain(this->signalsData[i] - random(1, 4), 0, 255);
          this->signalsData[i + 2] = constrain(this->signalsData[i + 1] - random(1, 8), 0, 255);
          this->signalsData[i + 3] = constrain(this->signalsData[i + 2] - random(1, 16), 0, 255);
          this->signalsData[i + 4] = constrain(this->signalsData[i + 3] - random(1, 32), 0, 255);
          this->signalsData[i + 5] = constrain(this->signalsData[i + 4] - random(1, 64), 0, 255);
          // change "noise" to -> "signal start"
          this->signalIndexes[i - 5] = 4;
          // change "noise" to -> "intermediate signal"
          this->signalIndexes[i - 4] = 2;
          this->signalIndexes[i - 3] = 2;
          this->signalIndexes[i - 2] = 2;
          this->signalIndexes[i - 1] = 2;
          // change "noise" to -> "signal" (peak)
          this->signalIndexes[i] = 1;
          // change "noise" to -> "intermediate signal"
          this->signalIndexes[i + 1] = 3;
          this->signalIndexes[i + 2] = 3;
          this->signalIndexes[i + 3] = 3;
          this->signalIndexes[i + 4] = 3;
          // change "noise" to -> "signal end"
          this->signalIndexes[i + 5] = 5;

          // generate next signal index
          currentSignalIndex += random(SSWA_MIN_DIST_BETWEEN_ANALOG_VOICE_SIGNALS, SSWA_MAX_DIST_BETWEEN_ANALOG_VOICE_SIGNALS);
          // regenerate signal level
          currentSignalValue = random(SSWA_MIN_SIGNAL_VALUE, SSWA_MAX_SIGNAL_VALUE);
        } else {
          this->signalsData[i] = this->noiseData[i];
        }
      } else {
        // check (ONLY) for existing noise at this index
        if (this->signalIndexes[i] == 0) {
          // no signal at this index => get "noise" data
          this->signalsData[i] = this->noiseData[i];
        }
      }
    } else {
      // first index (0) => get "noise" data
      this->signalsData[i] = this->noiseData[i];
    }
  }
}

// regenerate signals levels
void SSWAnimation::refreshSignals(void) {
  // re/generate "noise"
  this->refreshNoise();
  // left block are "cw/morse" signals
  for (uint16_t i = 0; i < SSWA_TOTAL_CW_SIGNALS_BANDWITH; i++) {
    // ignore existing signal and change only to new "noise" level
    if (this->signalIndexes[i] == 0) {
      // no signal at this index => get "noise" data
      this->signalsData[i] = this->noiseData[i];
    }
  }
  // right block are "analog voice" signals
  for (uint16_t i = SSWA_TOTAL_CW_SIGNALS_BANDWITH; i < SSWA_WIDTH; i++) {
    // ignore noise, only eval real signals (peak & near data)
    if (this->signalIndexes[i] > 0) {
      // check signal existence on current index
      if (this->signalIndexes[i] == 1) {
        // re-generate near signal indexes with random values
        this->signalsData[i - 1] = constrain(this->signalsData[i] - random(4, 8), 0, 255);
        this->signalsData[i - 2] = constrain(this->signalsData[i - 1] - random(4, 32), 0, 255);
        this->signalsData[i - 3] = constrain(this->signalsData[i - 2] + random(4, 40), 0, 255);
        this->signalsData[i - 4] = constrain(this->signalsData[i - 3] - random(4, 8), 0, 255);
        this->signalsData[i - 5] = constrain(this->signalsData[i - 4] - random(4, 16), 0, 255);
        this->signalsData[i + 1] = constrain(this->signalsData[i] - random(4, 8), 0, 255);
        this->signalsData[i + 2] = constrain(this->signalsData[i + 1] - random(4, 32), 0, 255);
        this->signalsData[i + 3] = constrain(this->signalsData[i + 2] + random(4, 40), 0, 255);
        this->signalsData[i + 4] = constrain(this->signalsData[i + 3] - random(4, 8), 0, 255);
        this->signalsData[i + 5] = constrain(this->signalsData[i + 4] - random(4, 16), 0, 255);
      }
    } else {
      // first index (0) => get "noise" data
      this->signalsData[i] = this->noiseData[i];
    }
  }
}

// move canvas one line down
void SSWAnimation::scrollDownWaterFallCanvas(GFXcanvas16* canvas) {
  uint16_t* buffer = canvas->getBuffer();
  for (int16_t y = canvas->height() - 1; y > 0; y--) {
    for (int16_t x = 0; x < canvas->width(); x++) {
      buffer[y * canvas->width() + x] = buffer[(y - 1) * canvas->width() + x];
    }
  }
}

// generate "blue gradient color" from signal (range value 0..255)
uint16_t SSWAnimation::generateBlueGradientColorFromSignal(uint8_t value) {
  uint8_t r, g, b;
  if (value <= 71) {
    r = 0;
    g = 0;
    b = 50 + value;
  } else if (value <= 191) {
    r = g = b = 30 + (value - 72) * 2;
  } else {
    r = g = b = 140 + (value - 192);
  }
  return this->display->color565(r, g, b);
}

// paint spectrum scope & waterfall animation
void SSWAnimation::draw(uint16_t xOffset, uint16_t yOffset) {
  // clear old spectrum scope data
  this->canvasSpectrumScope->fillScreen(ST77XX_BLACK);
  for (int16_t i = 0; i < SSWA_WIDTH; i++) {
    // left block are "cw/morse" signals
    if (i < SSWA_TOTAL_CW_SIGNALS_BANDWITH) {
      // if there is a signal at this index
      if (this->signalIndexes[i] == 1) {
        // "intermitent" morse signals
        if (millis() % (i * 3) == 0 || (millis() % (this->signalsData[i] / 8) == 0)) {
          uint16_t y_val = map(this->signalsData[i], 0, 255, 0, SSWA_SPECTRUM_SCOPE_HEIGHT);
          this->canvasSpectrumScope->drawPixel(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val, ST77XX_WHITE);
          this->canvasSpectrumScope->drawFastVLine(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val, y_val, ST77XX_WHITE);
          this->canvasWaterFall->drawPixel(i, 0, this->generateBlueGradientColorFromSignal(this->signalsData[i]));
        } else {
          //
          uint16_t y_val = map(this->noiseData[i], 0, 255, 0, SSWA_SPECTRUM_SCOPE_HEIGHT);
          this->canvasSpectrumScope->drawPixel(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val, ST77XX_WHITE);
          this->canvasSpectrumScope->drawFastVLine(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val + 1, y_val - 1, this->generateBlueGradientColorFromSignal(32));
          this->canvasWaterFall->drawPixel(i, 0, this->generateBlueGradientColorFromSignal(this->noiseData[i]));
        }
      } else {
        // draw "noise" (not real signal data)
        uint16_t y_val = map(this->noiseData[i], 0, 255, 0, SSWA_SPECTRUM_SCOPE_HEIGHT);
        canvasSpectrumScope->drawPixel(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val, ST77XX_WHITE);
        canvasSpectrumScope->drawFastVLine(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val + 1, y_val - 1, generateBlueGradientColorFromSignal(32));
        canvasWaterFall->drawPixel(i, 0, generateBlueGradientColorFromSignal(this->noiseData[i]));
      }
    } else {
      // right block are "analog voice" signals
      uint16_t y_val = map(this->signalsData[i], 0, 255, 0, SSWA_SPECTRUM_SCOPE_HEIGHT);
      uint16_t len = 0;
      switch (signalIndexes[i]) {
        case 4:
          // start  signal vertical line
          len = y_val - map(this->signalsData[i - 1] - 16, 0, 255, 0, SSWA_SPECTRUM_SCOPE_HEIGHT);
          this->canvasSpectrumScope->drawFastVLine(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val, y_val - map(this->signalsData[i - 1] - 16, 0, 255, 0, SSWA_SPECTRUM_SCOPE_HEIGHT), ST77XX_WHITE);
          break;
        case 5:
          //  end signal vertical line
          len = y_val - map(this->signalsData[i + 1] - 16, 0, 255, 0, SSWA_SPECTRUM_SCOPE_HEIGHT);
          this->canvasSpectrumScope->drawFastVLine(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val, y_val - map(this->signalsData[i + 1] - 16, 0, 255, 0, SSWA_SPECTRUM_SCOPE_HEIGHT), ST77XX_WHITE);
          break;
        default:
          this->canvasSpectrumScope->drawPixel(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val, ST77XX_WHITE);
          this->canvasSpectrumScope->drawFastVLine(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val + 1, y_val - 1, this->generateBlueGradientColorFromSignal(32));
          break;
      }
      this->canvasWaterFall->drawPixel(i, 0, this->generateBlueGradientColorFromSignal(this->signalsData[i]));
    }
  }
  this->display->drawRGBBitmap(xOffset, yOffset, this->canvasSpectrumScope->getBuffer(), SSWA_SPECTRUM_SCOPE_WIDTH, SSWA_SPECTRUM_SCOPE_HEIGHT);
  this->display->drawRGBBitmap(xOffset, yOffset + SSWA_SPECTRUM_SCOPE_HEIGHT + 4, this->canvasWaterFall->getBuffer(), SSWA_WATERFALL_WIDTH, SSWA_WATERFALL_HEIGHT);
  this->scrollDownWaterFallCanvas(this->canvasWaterFall);
}

// refresh animation
void SSWAnimation::refresh(uint16_t xOffset, uint16_t yOffset) {
  this->refreshSignals();
  this->draw(xOffset, yOffset);
}