#include "LGFXSSWAnimation.hpp"

LGFXSSWAnimation::LGFXSSWAnimation(LovyanGFX *existingDisplay, uint16_t width)
{
  if (existingDisplay != nullptr)
  {
    // init random seed
    randomSeed(analogRead(0) ^ (micros() * esp_random()));
    this->display = existingDisplay;
    this->canvasSpectrumScope = new lgfx::LGFX_Sprite(existingDisplay);
    this->canvasSpectrumScope->setColorDepth(16);
    if (this->width > 0)
    {
      this->width = width;
    }
    this->canvasSpectrumScope->createSprite(this->width, SSWA_SPECTRUM_SCOPE_HEIGHT);
    this->canvasWaterFall = new lgfx::LGFX_Sprite(existingDisplay);
    this->canvasWaterFall->setColorDepth(16);
    this->canvasWaterFall->createSprite(this->width, SSWA_WATERFALL_HEIGHT);
    this->signalsData = new uint16_t[this->width];
    this->noiseData = new uint16_t[this->width];
    this->signalIndexes = new uint16_t[this->width];
    this->refreshNoise();
    this->initSignals();
  }
}

LGFXSSWAnimation::~LGFXSSWAnimation()
{
  delete this->canvasSpectrumScope;
  this->canvasSpectrumScope = nullptr;
  delete this->canvasWaterFall;
  this->canvasWaterFall = nullptr;
  delete[] this->signalsData;
  this->signalsData = nullptr;
  delete[] this->noiseData;
  this->noiseData = nullptr;
  delete[] this->signalIndexes;
  this->signalIndexes = nullptr;
}

// move canvas one line down
void LGFXSSWAnimation::scrollDownWaterFallCanvas(lgfx::LGFX_Sprite *canvas)
{
  uint16_t *buffer = static_cast<uint16_t *>(canvas->getBuffer());
  int16_t w = canvas->width();
  int16_t h = canvas->height();
  for (int16_t y = h - 1; y > 0; y--)
  {
    for (int16_t x = 0; x < w; x++)
    {
      buffer[y * w + x] = buffer[(y - 1) * w + x];
    }
  }
}

// generate "blue gradient color" from signal (range value 0..255)
uint16_t LGFXSSWAnimation::generateBlueGradientColorFromSignal(uint8_t value)
{
  uint8_t r, g, b;
  if (value <= 71)
  {
    r = 0;
    g = 0;
    b = 50 + value;
  }
  else if (value <= 191)
  {
    r = g = b = 30 + (value - 72) * 2;
  }
  else
  {
    r = g = b = 140 + (value - 192);
  }
  return this->display->color565(r, g, b);
}

// paint spectrum scope & waterfall animation
void LGFXSSWAnimation::draw(uint16_t xOffset, uint16_t yOffset)
{
  // clear old spectrum scope data
  this->canvasSpectrumScope->fillScreen(TFT_BLACK);
  for (int16_t i = 0; i < this->width; i++)
  {
    // left block are "cw/morse" signals
    if (i < SSWA_TOTAL_CW_SIGNALS_BANDWITH)
    {
      // if there is a signal at this index
      if (this->signalIndexes[i] == 1)
      {
        // "intermitent" morse signals
        if (millis() % (i * 3) == 0 || (millis() % (this->signalsData[i] / 8) == 0))
        {
          uint16_t y_val = map(this->signalsData[i], 0, 255, 0, SSWA_SPECTRUM_SCOPE_HEIGHT);
          this->canvasSpectrumScope->drawPixel(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val, TFT_WHITE);
          this->canvasSpectrumScope->drawFastVLine(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val, y_val, TFT_WHITE);
          this->canvasWaterFall->drawPixel(i, 0, this->generateBlueGradientColorFromSignal(this->signalsData[i]));
        }
        else
        {
          //
          uint16_t y_val = map(this->noiseData[i], 0, 255, 0, SSWA_SPECTRUM_SCOPE_HEIGHT);
          this->canvasSpectrumScope->drawPixel(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val, TFT_WHITE);
          this->canvasSpectrumScope->drawFastVLine(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val + 1, y_val - 1, this->generateBlueGradientColorFromSignal(32));
          this->canvasWaterFall->drawPixel(i, 0, this->generateBlueGradientColorFromSignal(this->noiseData[i]));
        }
      }
      else
      {
        // draw "noise" (not real signal data)
        uint16_t y_val = map(this->noiseData[i], 0, 255, 0, SSWA_SPECTRUM_SCOPE_HEIGHT);
        canvasSpectrumScope->drawPixel(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val, TFT_WHITE);
        canvasSpectrumScope->drawFastVLine(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val + 1, y_val - 1, generateBlueGradientColorFromSignal(32));
        canvasWaterFall->drawPixel(i, 0, generateBlueGradientColorFromSignal(this->noiseData[i]));
      }
    }
    else
    {
      // right block are "analog voice" signals
      uint16_t y_val = map(this->signalsData[i], 0, 255, 0, SSWA_SPECTRUM_SCOPE_HEIGHT);
      uint16_t len = 0;
      switch (signalIndexes[i])
      {
      case 4:
        // start  signal vertical line
        len = y_val - map(this->signalsData[i - 1] - 16, 0, 255, 0, SSWA_SPECTRUM_SCOPE_HEIGHT);
        this->canvasSpectrumScope->drawFastVLine(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val, y_val - map(this->signalsData[i - 1] - 16, 0, 255, 0, SSWA_SPECTRUM_SCOPE_HEIGHT), TFT_WHITE);
        break;
      case 5:
        //  end signal vertical line
        len = y_val - map(this->signalsData[i + 1] - 16, 0, 255, 0, SSWA_SPECTRUM_SCOPE_HEIGHT);
        this->canvasSpectrumScope->drawFastVLine(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val, y_val - map(this->signalsData[i + 1] - 16, 0, 255, 0, SSWA_SPECTRUM_SCOPE_HEIGHT), TFT_WHITE);
        break;
      default:
        this->canvasSpectrumScope->drawPixel(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val, TFT_WHITE);
        this->canvasSpectrumScope->drawFastVLine(i, SSWA_SPECTRUM_SCOPE_HEIGHT - y_val + 1, y_val - 1, this->generateBlueGradientColorFromSignal(32));
        break;
      }
      this->canvasWaterFall->drawPixel(i, 0, this->generateBlueGradientColorFromSignal(this->signalsData[i]));
    }
  }
  this->canvasSpectrumScope->pushSprite(xOffset, yOffset);
  this->canvasWaterFall->pushSprite(xOffset, yOffset + SSWA_SPECTRUM_SCOPE_HEIGHT + 4);
  this->scrollDownWaterFallCanvas(this->canvasWaterFall);
}

// refresh animation
void LGFXSSWAnimation::refresh(uint16_t xOffset, uint16_t yOffset)
{
  this->refreshSignals();
  this->draw(xOffset, yOffset);
}
