#include "SSWAnimation.h"

/*
SSWAnimation::SSWAnimation(Adafruit_ST7789 *existingDisplay) {  
  this->display = existingDisplay;
  canvasSpectrumScope(OSCILLOSCOPE_WIDTH, OSCILLOSCOPE_HEIGHT);
  canvasWaterFall(WATERFALL_WIDTH, WATERFALL_HEIGHT);
  this.refreshNoise();
  this.initSignals();
}

*/
SSWAnimation::~SSWAnimation() {
  this->display = nullptr;  // ??? required
}

// generate random (bandwith) noise data
void SSWAnimation::refreshNoise(void) {
  const uint8_t MAX_VALUE = 32;
  const uint8_t MIN_PEAK_INTERVAL = 10;
  const uint8_t MAX_PEAK_INTERVAL = 30;
  for (uint16_t i = 0; i < TOTAL_BANDWITH_RESOLUTION; i++) {
    uint8_t currentValue = random(0, MAX_VALUE);
    uint8_t nextPeak = random(MIN_PEAK_INTERVAL, MAX_PEAK_INTERVAL);
    if (i % nextPeak == 0) {
      uint8_t peakValue = random(0, MAX_VALUE);
      int8_t direction = random(0, 2) * 2 - 1;
      for (int j = i; j < i + nextPeak && j < TOTAL_BANDWITH_RESOLUTION; j++) {
        this->noiseData[j] = max(0, min(255, currentValue + direction * abs(currentValue - peakValue) / nextPeak * (j - i)));
      }
      currentValue = peakValue;
      nextPeak = random(MIN_PEAK_INTERVAL, MAX_PEAK_INTERVAL);
    }
  }
}

// generate start signals levels
void SSWAnimation::initSignals(void) {
  // re/generate "noise"
  this->refreshNoise();
  // left block are "cw/morse" signals
  // random spacing between signals
  uint16_t currentSignalIndex = random(MIN_HZ_STEP_BETWEEN_MORSE_SIGNALS, MAX_HZ_STEP_BETWEEN_MORSE_SIGNALS);
  for (uint16_t i = 0; i < HZ_LIMIT_MORSE_SIGNALS; i++) {
    // set by default that this is a "noise" (not real signal)
    this->signalIndexes[i] = 0;
    if (i != 0) {
      // when reach next signal index
      if (i == currentSignalIndex) {
        // generate signal level
        this->signalsData[i] = random(MIN_SIGNAL_VALUE, MAX_SIGNAL_VALUE);
        // change "noise" to -> "signal"
        this->signalIndexes[i] = 1;
        // generate next signal index
        currentSignalIndex += random(MIN_HZ_STEP_BETWEEN_MORSE_SIGNALS, MAX_HZ_STEP_BETWEEN_MORSE_SIGNALS);
      } else {
        // no signal at this index => get "noise"
        this->signalsData[i] = this->noiseData[i];
      }
    } else {
      // first index (0) => get "noise"
      this->signalsData[i] = this->noiseData[i];
    }
  }

  // right block are "analog voice" signals
  // random spacing between signals
  currentSignalIndex = HZ_LIMIT_MORSE_SIGNALS + random(MIN_HZ_STEP_BETWEEN_ANALOG_VOICE_SIGNALS, MAX_HZ_STEP_BETWEEN_ANALOG_VOICE_SIGNALS);
  // generate signal level
  // TODO: remove random / 2
  uint8_t currentSignalValue = random(MIN_SIGNAL_VALUE / 2, MAX_SIGNAL_VALUE / 2) + random(MIN_SIGNAL_VALUE / 2, MAX_SIGNAL_VALUE / 2);
  // set "noise" by default
  for (uint16_t i = HZ_LIMIT_MORSE_SIGNALS; i < TOTAL_BANDWITH_RESOLUTION; i++) {
    signalIndexes[i] = 0;
  }

  for (uint16_t i = HZ_LIMIT_MORSE_SIGNALS; i < TOTAL_BANDWITH_RESOLUTION; i++) {
    if (i != 0) {
      // when reach next signal index
      if (i == currentSignalIndex) {
        // allow near signal indexes
        if (i > 4 && i < TOTAL_BANDWITH_RESOLUTION - 4) {
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
          currentSignalIndex += random(MIN_HZ_STEP_BETWEEN_ANALOG_VOICE_SIGNALS, MAX_HZ_STEP_BETWEEN_ANALOG_VOICE_SIGNALS);
          // regenerate signal level
          // TODO: remove random / 2
          currentSignalValue = random(MIN_SIGNAL_VALUE / 2, MAX_SIGNAL_VALUE / 2) + random(MIN_SIGNAL_VALUE / 2, MAX_SIGNAL_VALUE / 2);
        } else {
          //signalsData[i] = random(MIN_NOISE_VALUE, MAX_NOISE_VALUE);  // "noise" with low signal level
          this->signalsData[i] = this->noiseData[i];
        }
      } else {
        // check (ONLY) for existing noise at this index
        if (this->signalIndexes[i] == 0) {
          // no signal at this index => get "noise"
          this->signalsData[i] = this->noiseData[i];
        }
      }
    } else {
      // first index (0) => get "noise"
      this->signalsData[i] = this->noiseData[i];
    }
  }
}

// regenerate signals levels
void SSWAnimation::refreshSignals(void) {
  // re/generate "noise"
  refreshNoise();
  // left block are "cw/morse" signals
  for (uint16_t i = 0; i < HZ_LIMIT_MORSE_SIGNALS; i++) {
    // ignore existing signal and change only to new "noise" level
    if (this->signalIndexes[i] == 0) {
      // no signal at this index => get "noise"
      this->signalsData[i] = this->noiseData[i];
    }
  }
  // right block are "analog voice" signals
  for (uint16_t i = HZ_LIMIT_MORSE_SIGNALS; i < TOTAL_BANDWITH_RESOLUTION; i++) {
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
      // first index (0) => get "noise"
      this->signalsData[i] = this->noiseData[i];
    }
  }
}

// move canvas one line down
void SSWAnimation::moveCanvasDown(GFXcanvas16* canvas) {
  uint16_t* buffer = canvas->getBuffer();
  for (int y = canvas->height() - 1; y > 0; y--) {
    for (int x = 0; x < canvas->width(); x++) {
      buffer[y * canvas->width() + x] = buffer[(y - 1) * canvas->width() + x];
    }
  }
}

// generate "blue gradient" from signal (range value 0..255)
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
  canvasSpectrumScope.fillScreen(ST77XX_BLACK);
  for (int16_t i = 0; i < TOTAL_BANDWITH_RESOLUTION; i++) {
    if (i < HZ_LIMIT_MORSE_SIGNALS) {
      if (signalIndexes[i] == 1) {
        if (millis() % (i * 3) == 0 || (millis() % (this->signalsData[i] / 8) == 0)) {  // "intermitent" morse signals
          uint16_t y_val = map(this->signalsData[i], 0, 255, 0, OSCILLOSCOPE_HEIGHT);
          canvasSpectrumScope.drawPixel(i, OSCILLOSCOPE_HEIGHT - y_val, ST77XX_WHITE);
          canvasSpectrumScope.drawFastVLine(i, OSCILLOSCOPE_HEIGHT - y_val, y_val, ST77XX_WHITE);
          canvasWaterFall.drawPixel(i, 0, generateBlueGradientColorFromSignal(this->signalsData[i]));
        } else {
          uint16_t y_val = map(this->noiseData[i], 0, 255, 0, OSCILLOSCOPE_HEIGHT);
          canvasSpectrumScope.drawPixel(i, OSCILLOSCOPE_HEIGHT - y_val, ST77XX_WHITE);
          canvasSpectrumScope.drawFastVLine(i, OSCILLOSCOPE_HEIGHT - y_val + 1, y_val - 1, generateBlueGradientColorFromSignal(32));
          canvasWaterFall.drawPixel(i, 0, generateBlueGradientColorFromSignal(this->noiseData[i]));
        }
      } else {
        uint16_t y_val = map(this->noiseData[i], 0, 255, 0, OSCILLOSCOPE_HEIGHT);
        canvasSpectrumScope.drawPixel(i, OSCILLOSCOPE_HEIGHT - y_val, ST77XX_WHITE);
        canvasSpectrumScope.drawFastVLine(i, OSCILLOSCOPE_HEIGHT - y_val + 1, y_val - 1, generateBlueGradientColorFromSignal(32));
        canvasWaterFall.drawPixel(i, 0, generateBlueGradientColorFromSignal(this->noiseData[i]));
      }
    } else {
      uint16_t y_val = map(this->signalsData[i], 0, 255, 0, OSCILLOSCOPE_HEIGHT);
      uint16_t len = 0;
      switch (signalIndexes[i]) {
        case 4:
          // start  signal vertical line
          len = y_val - map(this->signalsData[i - 1] - 16, 0, 255, 0, OSCILLOSCOPE_HEIGHT);
          canvasSpectrumScope.drawFastVLine(i, OSCILLOSCOPE_HEIGHT - y_val, y_val - map(this->signalsData[i - 1] - 16, 0, 255, 0, OSCILLOSCOPE_HEIGHT), ST77XX_WHITE);
          break;
        case 5:
          //  end signal vertical line
          len = y_val - map(this->signalsData[i + 1] - 16, 0, 255, 0, OSCILLOSCOPE_HEIGHT);
          canvasSpectrumScope.drawFastVLine(i, OSCILLOSCOPE_HEIGHT - y_val, y_val - map(this->signalsData[i + 1] - 16, 0, 255, 0, OSCILLOSCOPE_HEIGHT), ST77XX_WHITE);
          break;
        default:
          canvasSpectrumScope.drawPixel(i, OSCILLOSCOPE_HEIGHT - y_val, ST77XX_WHITE);
          canvasSpectrumScope.drawFastVLine(i, OSCILLOSCOPE_HEIGHT - y_val + 1, y_val - 1, generateBlueGradientColorFromSignal(32));
          break;
      }
      canvasWaterFall.drawPixel(i, 0, generateBlueGradientColorFromSignal(this->signalsData[i]));
    }
  }
  this->display->drawRGBBitmap(xOffset, yOffset, canvasSpectrumScope.getBuffer(), OSCILLOSCOPE_WIDTH, OSCILLOSCOPE_HEIGHT);
  this->display->drawRGBBitmap(xOffset, yOffset + OSCILLOSCOPE_HEIGHT + 4, canvasWaterFall.getBuffer(), WATERFALL_WIDTH, WATERFALL_HEIGHT);
  moveCanvasDown(&canvasWaterFall);
}

void SSWAnimation::refresh(void) {
  // TODO
}