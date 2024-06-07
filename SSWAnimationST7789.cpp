#include "SSWAnimationST7789.h"

SSWAnimationST7789::SSWAnimationST7789(Adafruit_ST7789* existingDisplay) {
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

SSWAnimationST7789::~SSWAnimationST7789() {
  delete this->canvasSpectrumScope;
  this->canvasSpectrumScope = nullptr;
  delete this->canvasWaterFall;
  this->canvasWaterFall = nullptr;
}

// move canvas one line down
void SSWAnimationST7789::scrollDownWaterFallCanvas(GFXcanvas16* canvas) {
  uint16_t* buffer = canvas->getBuffer();
  int16_t w = canvas->width();
  int16_t h = canvas->height();
  for (int16_t y = h - 1; y > 0; y--) {
    for (int16_t x = 0; x < w; x++) {
      buffer[y * w + x] = buffer[(y - 1) * w + x];
    }
  }
}

// generate "blue gradient color" from signal (range value 0..255)
uint16_t SSWAnimationST7789::generateBlueGradientColorFromSignal(uint8_t value) {
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
void SSWAnimationST7789::draw(uint16_t xOffset, uint16_t yOffset) {
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
void SSWAnimationST7789::refresh(uint16_t xOffset, uint16_t yOffset) {
  this->refreshSignals();
  this->draw(xOffset, yOffset);
}
