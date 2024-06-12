#include "Display-ILI9488-320x480.hpp"

#define WIDGETS_VERTICAL_MARGIN 8

#define VFO_WIDGET_WIDTH 464                                                                                                                                    // widget width (without padding)
#define VFO_WIDGET_HEIGHT 72                                                                                                                                    // widget height (without padding)
#define VFO_WIDGET_PADDING 8                                                                                                                                    // real draw starts at this padding
#define VFO_WIDGET_FONT_SIZE 3                                                                                                                                  // font size (setTextSize())
#define VFO_WIDGET_FONT_WIDTH 20                                                                                                                                // font size (total pixel width)
#define VFO_WIDGET_FONT_HEIGHT 20                                                                                                                               // font size (total pixel height)
#define VFO_WIDGET_START_X_COORDINATE VFO_WIDGET_PADDING                                                                                                        // draw (x) starts here
#define VFO_WIDGET_START_Y_COORDINATE VFO_WIDGET_PADDING                                                                                                        // draw (y) starts here
#define VFO_WIDGET_STEP_HORIZONTAL_MARGIN 2                                                                                                                     // frequency step indicator top/bottom margin
#define VFO_WIDGET_STEP_VERTICAL_MARGIN 4                                                                                                                       // frequency step indicator top/bottom margin
#define VFO_WIDGET_STEP_WIDTH 16                                                                                                                                // frequency step indicator width
#define VFO_WIDGET_STEP_HEIGHT 3                                                                                                                                // frequency step indicator height
#define VFO_WIDGET_SINGLE_VFO_LINE_HEIGHT (VFO_WIDGET_FONT_HEIGHT + VFO_WIDGET_STEP_VERTICAL_MARGIN + VFO_WIDGET_STEP_HEIGHT + VFO_WIDGET_STEP_VERTICAL_MARGIN) // single VFO line (frequency step indicator included) height

// first VFO x/y coordinates
#define VFO_WIDGET_PRIMARY_START_X_COORDINATE VFO_WIDGET_START_X_COORDINATE
#define VFO_WIDGET_PRIMARY_START_Y_COORDINATE VFO_WIDGET_START_Y_COORDINATE
// first VFO freq step indicator x/y coordinates
#define VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE 90
#define VFO_WIDGET_PRIMARY_STEP_START_Y_COORDINATE VFO_WIDGET_PRIMARY_START_Y_COORDINATE + VFO_WIDGET_FONT_HEIGHT + VFO_WIDGET_STEP_VERTICAL_MARGIN
// secondary VFO x/y coordinates
#define VFO_WIDGET_SECONDARY_START_X_COORDINATE VFO_WIDGET_START_X_COORDINATE
#define VFO_WIDGET_SECONDARY_START_Y_COORDINATE VFO_WIDGET_START_Y_COORDINATE + VFO_WIDGET_SINGLE_VFO_LINE_HEIGHT
// secondary VFO freq step indicator x/y coordinates
#define VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE 90
#define VFO_WIDGET_SECONDARY_STEP_START_Y_COORDINATE VFO_WIDGET_SECONDARY_START_Y_COORDINATE + VFO_WIDGET_FONT_HEIGHT + VFO_WIDGET_STEP_VERTICAL_MARGIN

#define SMETER_PARTS 42
#define SMETER_PART_WIDTH 3
#define SMETER_PART_SPACE_WIDTH 2
#define SMETER_PARTH_HEIGHT 18
#define SMETER_PARTH_HEIGHT_SEPARATOR 20
#define SMETER_PARTH_BG_COLOR 0x8410

#define VFO_BLOCK_START_X_COORDINATE 8
#define VFO_PRIMARY_BLOCK_START_Y_COORDINATE 8
#define VFO_SECONDARY_BLOCK_START_Y_COORDINATE 44

#define COLOR_ACTIVE 0xFFFF
#define COLOR_SECONDARY 0x3186 // 0x3186, 0x4208, 0x7BEF

DisplayILI9488::DisplayILI9488(uint16_t width, uint16_t height, uint8_t rotation, bool invertDisplayColors)
    : screen()
{
  this->width = width;
  this->height = height;
  this->screen.init();
  if (rotation > 0 && rotation < 4)
  {
    // this->screen.setRotation(rotation);
    if (rotation == 1 || rotation == 3)
    {
      this->width = height;
      this->height = width;
    }
  }
  this->screen.setTextWrap(false);
  this->screen.fillScreen(TFT_BLACK);
#ifdef DISPLAY_DEBUG
  this->fpsDebug = new FPSDebug();
#endif
}

DisplayILI9488::~DisplayILI9488()
{
}

void DisplayILI9488::clearScreen(uint8_t color)
{
  this->screen.fillScreen(color);
}

void DisplayILI9488::showConnectScreen(uint16_t serialBaudRate, float currentVersion)
{
  this->screen.fillScreen(TFT_BLACK);
  this->screen.drawRect(0, 0, this->width, this->height, TFT_WHITE);
  this->screen.setTextColor(TFT_WHITE, TFT_BLACK);
  this->screen.setTextSize(2);
  this->screen.setTextDatum(CC_DATUM);
  char str[64];
  sprintf(str, "ESP32 SDR Remote Control");
  this->screen.drawString(str, this->width / 2, 16);
  sprintf(str, "v%.2f", currentVersion);
  this->screen.drawString(str, this->width / 2, 40);
  this->screen.setTextSize(1);
  sprintf(str, "Searching TS-2000 CAT connection (%d baud)", serialBaudRate);
  this->screen.drawString(str, this->width / 2, 300);
  this->animatedScreenPtr = new SSWAnimationILI9488(&this->screen, 240);
}

void DisplayILI9488::hideConnectScreen(void)
{
  delete this->animatedScreenPtr;
  this->animatedScreenPtr = nullptr;
}

void DisplayILI9488::refreshConnectScreen()
{
  this->animatedScreenPtr->refresh(120, 80);
#ifdef DISPLAY_DEBUG
  this->fpsDebug->loop();
  this->screen.setCursor(230, 190);
  this->screen.setTextSize(1);
  this->screen.printf("%03u FPS", this->fpsDebug->getFPS());
#endif
}

void DisplayILI9488::showMainScreen()
{
  this->screen.fillScreen(TFT_BLACK);
  // screen recangle border (DEBUG)
  this->screen.drawRect(0, 0, this->width, this->height, TFT_WHITE);

  /*
  this->screen.drawRect(0, 72, this->width / 2, 120, TFT_WHITE);
  this->screen.drawRect(this->width / 2, 72, this->width, 120, TFT_WHITE);
  this->screen.setTextDatum(TL_DATUM);
  this->createDigitalSMeter();

  // Volume
  this->screen.setTextColor(COLOR_ACTIVE);
  this->screen.setCursor(250, 80);
  this->screen.setTextSize(2);
  this->screen.print("VOLUME");
  this->screen.drawRect(330, 80, 102, 14, TFT_PINK);
  this->screen.setCursor(445, 80);
  this->screen.print("00");

  // Passband filter
  this->screen.drawFastHLine(260, 175, 200, TFT_WHITE);
  */
}

void DisplayILI9488::refreshMainScreen(Transceiver *trx)
{
  // TODO: only refresh active VFO
  if (trx->changed & TRX_CFLAG_TRANSMIT_RECEIVE_POWER_STATUS)
  {
    // this->refreshTransmitStatus(false);
    trx->changed &= ~TRX_CFLAG_TRANSMIT_RECEIVE_POWER_STATUS;
  }
  if (trx->changed & TRX_CFLAG_ACTIVE_VFO_INDEX)
  {
    this->refreshVFOIndex(0, trx->activeVFOIndex == 0);
    this->refreshVFOIndex(1, trx->activeVFOIndex == 1);
    trx->changed &= ~TRX_CFLAG_ACTIVE_VFO_INDEX;
  }
  if (trx->changed & TRX_CFLAG_ACTIVE_VFO_FREQUENCY)
  {
    this->refreshVFOFreq(0, trx->activeVFOIndex == 0, trx->VFO[0].frequency);
    this->refreshVFOFreq(1, trx->activeVFOIndex == 1, trx->VFO[1].frequency);
    trx->changed &= ~TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
  }
  if (trx->changed & TRX_CFLAG_ACTIVE_VFO_MODE)
  {
    this->refreshVFOMode(0, trx->activeVFOIndex == 0, trx->VFO[0].mode);
    this->refreshVFOMode(1, trx->activeVFOIndex == 1, trx->VFO[1].mode);
    trx->changed &= ~TRX_CFLAG_ACTIVE_VFO_MODE;
  }
  if (trx->changed & TRX_CFLAG_ACTIVE_VFO_STEP)
  {
    this->refreshVFOStep(0, trx->activeVFOIndex == 0, trx->VFO[0].customStep);
    this->refreshVFOStep(1, trx->activeVFOIndex == 1, trx->VFO[1].customStep);
    trx->changed &= ~TRX_CFLAG_ACTIVE_VFO_STEP;
  }
  if (trx->changed & TRX_CFLAG_SIGNAL_METER_LEVEL)
  {
    // this->refreshRNDDigitalSMeter(random(0, 42));
    // this->refreshRNDDigitalSMeter(trx->signalMeterLevel); //
    // trx->changed &= ~TRX_CFLAG_SIGNAL_METER_LEVEL;
  }
  if (trx->changed & TRX_CFLAG_FILTER_LOW || trx->changed & TRX_CFLAG_FILTER_HIGH)
  {
    // this->refreshPassBandFilter(trx->VFO[trx->activeVFOIndex].LF, trx->VFO[trx->activeVFOIndex].HF, trx->VFO[trx->activeVFOIndex].BW);
    trx->changed &= ~TRX_CFLAG_FILTER_LOW;
    trx->changed &= ~TRX_CFLAG_FILTER_HIGH;
  }
  if ((trx->changed & TRX_CFLAG_AF_GAIN) || (trx->changed & TRX_CFLAG_AUDIO_MUTE))
  {
    // this->refreshVolume(trx->AFGain, trx->audioMuted);
    trx->changed &= ~TRX_CFLAG_AF_GAIN;
    trx->changed &= ~TRX_CFLAG_AUDIO_MUTE;
  }
#ifdef DISPLAY_DEBUG
  this->fpsDebug->loop();
  this->screen.setTextColor(0xF85E, TFT_BLACK);
  this->screen.setCursor(VFO_BLOCK_START_X_COORDINATE, this->height - 16);
  this->screen.setTextSize(1);
  this->screen.printf("%03u FPS", this->fpsDebug->getFPS());
#endif
}

void DisplayILI9488::refreshTransmitStatus(bool isTransmitting)
{
  if (isTransmitting)
  {
    this->screen.drawRect(0, 0, 29, 20, TFT_RED);
    this->screen.setTextColor(TFT_RED, TFT_BLACK);
  }
  else
  {
    this->screen.drawRect(0, 0, 29, 20, TFT_GREEN);
    this->screen.setTextColor(TFT_GREEN, TFT_BLACK);
  }
  this->screen.setCursor(3, 3);
  this->screen.setTextSize(2);
  this->screen.print(isTransmitting ? "TX" : "RX");
}

void DisplayILI9488::refreshVFOIndex(uint8_t number, bool isActive)
{
  this->screen.setTextColor(isActive ? COLOR_ACTIVE : COLOR_SECONDARY, TFT_BLACK);
  if (number == 0)
  {
    this->screen.setCursor(VFO_WIDGET_PRIMARY_START_X_COORDINATE, VFO_WIDGET_PRIMARY_START_Y_COORDINATE);
  }
  else
  {
    this->screen.setCursor(VFO_WIDGET_SECONDARY_START_X_COORDINATE, VFO_WIDGET_SECONDARY_START_Y_COORDINATE);
  }

  this->screen.setTextSize(VFO_WIDGET_FONT_SIZE);
  this->screen.print(number == 0 ? "VFOA" : "VFOB");
}

void DisplayILI9488::refreshVFOFreq(uint8_t number, bool isActive, uint64_t frequency)
{
  char formattedFrequency[16];
  char nformattedFrequency[12];
  // test if this method is more optimized than div operators
  int resultIndex = 0;
  sprintf(nformattedFrequency, "%012llu", frequency);
  for (int i = 0; i < 12; ++i)
  {
    formattedFrequency[resultIndex++] = nformattedFrequency[i];
    if ((i + 1) % 3 == 0 && i < 11)
    {
      formattedFrequency[resultIndex++] = '.';
    }
  }
  formattedFrequency[resultIndex] = '\0';
  this->screen.setTextColor(isActive ? COLOR_ACTIVE : COLOR_SECONDARY, TFT_BLACK);
  this->screen.setCursor(90, number == 0 ? VFO_WIDGET_PRIMARY_START_Y_COORDINATE : VFO_WIDGET_SECONDARY_START_Y_COORDINATE);
  this->screen.setTextSize(VFO_WIDGET_FONT_SIZE);
  this->screen.print(formattedFrequency);
  if (frequency >= 1000000000)
  {
    this->screen.print("GHz");
  }
  else if (frequency >= 1000000)
  {
    this->screen.print("Mhz");
  }
  else if (frequency >= 1000)
  {
    this->screen.print("Khz");
  }
  else
  {
    this->screen.print("Hz ");
  }
}

void DisplayILI9488::refreshVFOMode(uint8_t number, bool isActive, TRXVFOMode mode)
{
  this->screen.setTextColor(isActive ? TFT_CYAN : COLOR_SECONDARY, TFT_BLACK);
  this->screen.setCursor(420, number == 0 ? VFO_WIDGET_PRIMARY_START_Y_COORDINATE : VFO_WIDGET_SECONDARY_START_Y_COORDINATE);
  this->screen.setTextSize(VFO_WIDGET_FONT_SIZE);
  switch (mode)
  {
  case TRX_VFO_MD_DSB:
    this->screen.print("DSB");
    break;
  case TRX_VFO_MD_LSB:
    this->screen.print("LSB");
    break;
  case TRX_VFO_MD_USB:
    this->screen.print("USB");
    break;
  case TRX_VFO_MD_CW_U:
    this->screen.print("CWU"); // CW (upper sideband)
    break;
  case TRX_VFO_MD_FM:
    this->screen.print("FM ");
    break;
  case TRX_VFO_MD_SAM:
    this->screen.print("SAM"); // (synchronous AM, includes ECSS)
    break;
  case TRX_VFO_MD_RESERVED:
    this->screen.print("  "); // reserved / not used
    break;
  case TRX_VFO_MD_CW_L:
    this->screen.print("CWL"); // CW (lower sideband)
    break;
  case TRX_VFO_MD_WFM:
    this->screen.print("WFM");
    break;
  case TRX_VFO_MD_BFM:
    this->screen.print("BFM");
    break;
  default:
    this->screen.print("???"); // error / invalid value
    break;
  }
}

void DisplayILI9488::refreshVFOStep(uint8_t number, bool isActive, uint64_t step)
{
  uint16_t x = 0;
  uint16_t y = 0;
  // clear old step
  if (number == 0)
  {
    x = VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE;
    y = VFO_WIDGET_PRIMARY_STEP_START_Y_COORDINATE;
  }
  else
  {
    x = VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE;
    y = VFO_WIDGET_SECONDARY_STEP_START_Y_COORDINATE;
  }
  this->screen.fillRect(x, y, 15 * (VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN), VFO_WIDGET_STEP_HEIGHT, TFT_BLACK);
  if (step > 0)
  {
    switch (step)
    {
    case 1: // 1 hz
      x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 14);
      break;
    case 10: // 10 hz
      x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 13);
      break;
    case 100: // 100 hz
      x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 12);
      break;
    case 1000: // 1 Khz
      x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 10);
      break;
    case 10000: // 10 Khz
      x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 9);
      break;
    case 100000: // 100 Khz
      x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 8);
      break;
    case 1000000: // 1 Mhz
      x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 6);
      break;
    case 10000000: // 10 Mhz
      x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 5);
      break;
    case 100000000: // 100 Mhz
      x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 4);
      break;
    case 1000000000: // 1 Ghz
      x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 2);
      break;
    case 10000000000: // 10 Ghz
      x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + (VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN);
      break;
    case 100000000000: // 100 Ghz
      x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE);
      break;
    default:
      x = 0;
      break;
    }
    this->screen.fillRect(x, y, VFO_WIDGET_STEP_WIDTH, VFO_WIDGET_STEP_HEIGHT, isActive ? COLOR_ACTIVE : COLOR_SECONDARY);
  }
}

void DisplayILI9488::createDigitalSMeter()
{
  this->screen.setTextSize(3);
  this->screen.setTextColor(COLOR_ACTIVE);
  this->screen.setCursor(24, 90);
  this->screen.print("S9");
  this->screen.setTextSize(2);
  this->screen.print("+30dB");
  this->screen.setTextSize(1);
  this->screen.setTextColor(COLOR_ACTIVE);
  this->screen.setCursor(13, 170);
  this->screen.printf("%u", 0);
  this->screen.setCursor(23, 170);
  this->screen.printf("%u", 1);
  this->screen.setCursor(33, 170);
  this->screen.printf("%u", 2);
  this->screen.setCursor(43, 170);
  this->screen.printf("%u", 3);
  this->screen.setCursor(53, 170);
  this->screen.printf("%u", 4);
  this->screen.setCursor(63, 170);
  this->screen.printf("%u", 5);
  this->screen.setCursor(73, 170);
  this->screen.printf("%u", 6);
  this->screen.setCursor(83, 170);
  this->screen.printf("%u", 7);
  this->screen.setCursor(93, 170);
  this->screen.printf("%u", 8);
  this->screen.setCursor(103, 170);
  this->screen.printf("%u", 9);
  this->screen.setCursor(138, 170);
  this->screen.print("+20");
  this->screen.setCursor(174, 170);
  this->screen.print("+40");
  this->screen.setCursor(210, 170);
  this->screen.print("+60");

  /*
  for (int i = 0; i <= SMETER_PARTS; i++)
  {
    int x = 14 + i * (SMETER_PART_WIDTH + SMETER_PART_SPACE_WIDTH);
    if (i == 0 || i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 26 || i == 34 || i == 42)
    {
      this->screen.fillRect(x, 40 + 98, SMETER_PART_WIDTH, 22 + 4, COLOR_SECONDARY);
    }
    else
    {
      this->screen.fillRect(x, 40 + 98, SMETER_PART_WIDTH, 18, COLOR_SECONDARY);
    }
  }
  */
  for (int i = 0; i <= SMETER_PARTS; i++)
  {
    int x = 14 + i * (SMETER_PART_WIDTH + SMETER_PART_SPACE_WIDTH);
    int height = 18;
    this->screen.fillRect(x, 145 - i, SMETER_PART_WIDTH, height + i, COLOR_SECONDARY);
  }
}

void DisplayILI9488::refreshRNDDigitalSMeter(int newSignal)
{
  this->oldSignal = this->currentSignal;
  // signal changed
  if (this->oldSignal != newSignal)
  {
    if (newSignal >= this->oldSignal)
    {
      // signal increased
      this->currentSignal = newSignal;
      if (newSignal > this->oldSignal)
      {
        this->peakSignal = newSignal;
        this->lastPeakChange = millis();
      }
    }
    else if (newSignal < this->oldSignal)
    {
      // this->peakSignal = this->oldSignal;
      long current = millis();
      if (current - this->lastPeakChange > 200)
      {
        this->peakSignal--;
        this->lastPeakChange = current;
      }
      this->currentSignal = this->oldSignal - 1;
    }
    if (this->currentSignal < 0)
    {
      this->currentSignal = 0;
    }
    else if (this->currentSignal > 42)
    {
      this->currentSignal = 42;
    }
    int start = 0;
    if (this->oldSignal < this->currentSignal)
    {
      start = this->oldSignal;
    }
    else if (this->oldSignal > this->currentSignal)
    {
      start = this->currentSignal;
    }
    else
    {
      start = 0;
    }
    if (this->peakSignal < 0)
    {
      this->peakSignal = 0;
    }

    // BG
    for (int i = start; i <= SMETER_PARTS; i++)
    {
      int x = 26 + i * (SMETER_PART_WIDTH + SMETER_PART_SPACE_WIDTH);
      if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42)
      {
        this->screen.fillRect(x, 94, SMETER_PART_WIDTH, 22, SMETER_PARTH_BG_COLOR);
      }
      else
      {
        this->screen.fillRect(x, 98, SMETER_PART_WIDTH, SMETER_PARTH_HEIGHT, SMETER_PARTH_BG_COLOR);
      }
    }

    // PEAK
    for (int i = 0; i <= SMETER_PARTS; i++)
    {
      if (i == this->peakSignal)
      {
        int x = 26 + i * (SMETER_PART_WIDTH + SMETER_PART_SPACE_WIDTH);
        // set more height on marked values
        if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42)
        {
          this->screen.fillRect(x, 94, SMETER_PART_WIDTH, 22, TFT_WHITE);
        }
        else
        {
          this->screen.fillRect(x, 98, SMETER_PART_WIDTH, SMETER_PARTH_HEIGHT, TFT_WHITE);
        }
      }
    }

    // SIGNAL
    for (int i = 0; i <= SMETER_PARTS; i++)
    {
      // real value draw
      if (i < this->currentSignal)
      {
        int x = 26 + i * (SMETER_PART_WIDTH + SMETER_PART_SPACE_WIDTH);
        // set more height on marked values
        if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42)
        {
          this->screen.fillRect(x, 94, SMETER_PART_WIDTH, 22, i <= 18 ? TFT_GREEN : TFT_RED);
        }
        else
        {
          this->screen.fillRect(x, 98, SMETER_PART_WIDTH, SMETER_PARTH_HEIGHT, i <= 18 ? TFT_GREEN : TFT_RED);
        }
      }
    }
  }
  else
  {
    if (this->peakSignal > this->currentSignal)
    {
      long current = millis();
      if (current - this->lastPeakChange > 200)
      {
        this->peakSignal--;
        if (this->peakSignal > 0)
        {
          this->peakSignal--;
          this->lastPeakChange = current;

          for (int i = this->peakSignal + 1; i <= SMETER_PARTS; i++)
          {
            int x = 26 + i * (SMETER_PART_WIDTH + SMETER_PART_SPACE_WIDTH);
            if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42)
            {
              this->screen.fillRect(x, 94, SMETER_PART_WIDTH, 22, SMETER_PARTH_BG_COLOR);
            }
            else
            {
              this->screen.fillRect(x, 98, SMETER_PART_WIDTH, SMETER_PARTH_HEIGHT, SMETER_PARTH_BG_COLOR);
            }
          }

          for (int i = 0; i <= SMETER_PARTS; i++)
          {
            if (i == this->peakSignal)
            {
              int x = 26 + i * (SMETER_PART_WIDTH + SMETER_PART_SPACE_WIDTH);
              // set more height on marked values
              if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42)
              {
                this->screen.fillRect(x, 94, SMETER_PART_WIDTH, 22, TFT_WHITE);
              }
              else
              {
                this->screen.fillRect(x, 98, SMETER_PART_WIDTH, SMETER_PARTH_HEIGHT, TFT_WHITE);
              }
            }
          }
        }
      }
    }
  }
}

void DisplayILI9488::refreshVolume(uint8_t AFGain, bool isMuted)
{
  this->screen.fillRect(331, 81, AFGain, 12, !isMuted ? COLOR_ACTIVE : TFT_RED);
  if (AFGain < 100)
  {
    this->screen.fillRect(331 + AFGain, 81, 100 - AFGain, 12, COLOR_SECONDARY);
  }
  this->screen.setTextColor(!isMuted ? COLOR_ACTIVE : TFT_RED, TFT_BLACK);
  this->screen.setTextSize(2);
  this->screen.setCursor(445, 80);
  this->screen.printf("%02u", AFGain);
}

void DisplayILI9488::refreshPassBandFilter(uint32_t LF, uint32_t HF, uint32_t BW)
{
  // TODO: real data
  this->screen.setTextSize(1);
  this->screen.setTextColor(COLOR_ACTIVE);
  this->screen.setCursor(260, 110);
  this->screen.print("LC 200Hz");
  this->screen.setCursor(320, 110);
  this->screen.print("BW 2.6Khz");
  this->screen.setCursor(380, 110);
  this->screen.print("HC 2.8Khz");
  this->screen.setCursor(340, 180);
  this->screen.print("1500Hz");
  // TODO: REAL graph
  // left
  this->screen.drawLine(290, 175, 320, 130, TFT_WHITE);
  // top
  this->screen.drawFastHLine(320, 130, 100, TFT_WHITE);
  // center
  this->screen.drawFastVLine(340, 130, 45, TFT_WHITE);
  // right
  this->screen.drawLine(420, 130, 450, 175, TFT_WHITE);
}
