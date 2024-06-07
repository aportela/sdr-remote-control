#include "Display-ILI9488-320x480.h"

#define SMETER_PARTS 42
#define SMETER_PART_WIDTH 4
#define SMETER_PART_SPACE_WIDTH 2
#define SMETER_PARTH_HEIGHT 18
#define SMETER_PARTH_HEIGHT_SEPARATOR 20
#define SMETER_PARTH_BG_COLOR 0x8410

DisplayILI9488::DisplayILI9488(uint16_t width, uint16_t height, uint8_t rotation, bool invertDisplayColors)
  : screen() {
  this->width = width;
  this->height = height;
  this->screen.init();
  if (invertDisplayColors) {
    this->screen.invertDisplay(true);
  }
  if (rotation > 0 && rotation < 4) {
    this->screen.setRotation(rotation);
    if (rotation == 1 || rotation == 3) {
      this->width = height;
      this->height = width;
    }
  }
  this->screen.setTextWrap(false);
  this->screen.fillScreen(TFT_BLACK);
}

void DisplayILI9488::clearScreen(uint8_t color) {
  this->screen.fillScreen(color);
}

void DisplayILI9488::showConnectScreen(uint32_t serialBaudRate, float currentVersion) {
  this->screen.fillScreen(TFT_BLACK);
  this->screen.drawRect(4, 4, this->width - 8, this->height - 8, TFT_WHITE);
  this->screen.setTextColor(TFT_WHITE, TFT_BLACK);
  this->screen.setTextSize(2);
  this->screen.setCursor(18, 10);
  this->screen.printf("ESP32 SDR Remote Control");
  this->screen.setCursor(140, 30);
  this->screen.printf("v%.2f", currentVersion);
  this->screen.setCursor(10, 210);
  this->screen.printf("TS-2000 CAT / %d baud", serialBaudRate);
  this->animatedScreenPtr = new SSWAnimation(&this->screen);
}

void DisplayILI9488::hideConnectScreen(void) {
  delete this->animatedScreenPtr;
  this->animatedScreenPtr = nullptr;
}

void DisplayILI9488::refreshConnectScreen(void) {
  this->animatedScreenPtr->refresh(80, 80);
}

void DisplayILI9488::showMainScreen() {
  this->screen.fillScreen(TFT_BLACK);
  this->createDigitalSMeter();
}

void DisplayILI9488::refreshMainScreen(Transceiver* trx, float fps) {
  if (trx->changed & TRX_CFLAG_TRANSMIT_RECEIVE_POWER_STATUS) {
    this->refreshTransmitStatus(false);
    trx->changed &= ~TRX_CFLAG_TRANSMIT_RECEIVE_POWER_STATUS;
  }
  if (trx->changed & TRX_CFLAG_ACTIVE_VFO_INDEX) {
    this->refreshActiveVFO(trx->activeVFOIndex);
    trx->changed &= ~TRX_CFLAG_ACTIVE_VFO_INDEX;
  }
  if (trx->changed & TRX_CFLAG_ACTIVE_VFO_MODE) {
    this->refreshVFOMode(trx->VFO[trx->activeVFOIndex].mode);
    trx->changed &= ~TRX_CFLAG_ACTIVE_VFO_MODE;
  }
  if (trx->changed & TRX_CFLAG_ACTIVE_VFO_FREQUENCY) {
    this->refreshVFOFreq(trx->VFO[trx->activeVFOIndex].frequency);
    trx->changed &= ~TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
  }
  if (trx->changed & TRX_CFLAG_SIGNAL_METER_LEVEL) {
    //this->refreshRNDDigitalSMeter(random(0, 42));
    this->refreshRNDDigitalSMeter(trx->signalMeterLevel);
    //trx->changed &= ~TRX_CFLAG_SIGNAL_METER_LEVEL;
  }
  this->refreshFPS(fps);
}

void DisplayILI9488::refreshTransmitStatus(bool isTransmitting) {
  if (isTransmitting) {
    this->screen.drawRect(0, 0, 29, 20, TFT_RED);
    this->screen.setTextColor(TFT_RED, TFT_BLACK);
  } else {
    this->screen.drawRect(0, 0, 29, 20, TFT_GREEN);
    this->screen.setTextColor(TFT_GREEN, TFT_BLACK);
  }
  this->screen.setCursor(3, 3);
  this->screen.setTextSize(2);
  this->screen.print(isTransmitting ? "TX" : "RX");
}

void DisplayILI9488::refreshActiveVFO(uint8_t number) {
  this->screen.drawRect(31, 0, 53, 20, TFT_WHITE);
  this->screen.setTextColor(TFT_WHITE, TFT_BLACK);
  this->screen.setCursor(34, 3);
  this->screen.setTextSize(2);
  this->screen.printf("VFO%d", number);
}

void DisplayILI9488::refreshVFOMode(TRXVFOMode mode) {
  this->screen.drawRect(86, 0, 43, 20, TFT_WHITE);
  this->screen.setTextColor(TFT_WHITE, TFT_BLACK);
  this->screen.setCursor(89, 3);
  this->screen.setTextSize(2);
  switch (mode) {
    case TRX_VFO_MD_DSB:
      this->screen.printf("DSB");
      break;
    case TRX_VFO_MD_LSB:
      this->screen.printf("LSB");
      break;
    case TRX_VFO_MD_USB:
      this->screen.printf("USB");
      break;
    case TRX_VFO_MD_CW_U:
      this->screen.printf("CWU");  // CW (upper sideband)
      break;
    case TRX_VFO_MD_FM:
      this->screen.printf(" FM");
      break;
    case TRX_VFO_MD_SAM:
      this->screen.printf("SAM");  // (synchronous AM, includes ECSS)
      break;
    case TRX_VFO_MD_RESERVED:
      this->screen.printf("  ");  // reserved / not used
      break;
    case TRX_VFO_MD_CW_L:
      this->screen.printf("CWL");  // CW (lower sideband)
      break;
    case TRX_VFO_MD_WFM:
      this->screen.printf("WFM");
      break;
    case TRX_VFO_MD_BFM:
      this->screen.printf("BFM");
      break;
    default:
      this->screen.printf("???");  // error / invalid value
      break;
  }
}

void DisplayILI9488::refreshFPS(float fps) {
  this->screen.drawRect(230, 0, 89, 20, 0xF85E);
  this->screen.setTextColor(0xF85E, TFT_BLACK);
  this->screen.setCursor(233, 3);
  this->screen.setTextSize(2);
  this->screen.printf("%03u FPS", (int16_t)fps);
}

void DisplayILI9488::refreshVFOFreq(uint64_t frequency) {
  this->screen.setTextSize(3);
  char formattedFrequency[16];
  char nformattedFrequency[12];
  // test if this method is more optimized than div operators
  int resultIndex = 0;
  sprintf(nformattedFrequency, "%012llu", frequency);
  for (int i = 0; i < 12; ++i) {
    formattedFrequency[resultIndex++] = nformattedFrequency[i];
    if ((i + 1) % 3 == 0 && i < 11) {
      formattedFrequency[resultIndex++] = '.';
    }
  }

  formattedFrequency[resultIndex] = '\0';

  this->screen.setCursor(35, 30);
  this->screen.setTextColor(TFT_WHITE, TFT_BLACK);
  this->screen.print(formattedFrequency);
}

void DisplayILI9488::createDigitalSMeter() {
  randomSeed(analogRead(0));
  this->screen.setTextSize(2);
  this->screen.setTextColor(TFT_WHITE);
  this->screen.setCursor(36, 70);
  this->screen.print("1 3 5 7 9");
  this->screen.setCursor(165, 70);
  this->screen.print("+20 +40 +60");
  this->screen.setCursor(6, 98);
  this->screen.print("S");
  this->screen.setCursor(292, 98);
  this->screen.print("dB");
  this->screen.drawFastVLine(23, 94, 22, TFT_WHITE);
  this->screen.drawFastVLine(284, 94, 22, TFT_WHITE);
  this->screen.drawFastHLine(23, 116, 262, TFT_WHITE);

  for (int i = 0; i <= SMETER_PARTS; i++) {
    int x = 26 + i * (SMETER_PART_WIDTH + SMETER_PART_SPACE_WIDTH);
    if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 26 || i == 34 || i == 42) {
      this->screen.fillRect(x, 94, SMETER_PART_WIDTH, 22, SMETER_PARTH_BG_COLOR);
    } else {
      this->screen.fillRect(x, 98, SMETER_PART_WIDTH, SMETER_PARTH_HEIGHT, SMETER_PARTH_BG_COLOR);
    }
  }
}

void DisplayILI9488::refreshRNDDigitalSMeter(int newSignal) {
  this->oldSignal = this->currentSignal;
  // signal changed
  if (this->oldSignal != newSignal) {
    if (newSignal >= this->oldSignal) {
      // signal increased
      this->currentSignal = newSignal;
      if (newSignal > this->oldSignal) {
        this->peakSignal = newSignal;
        this->lastPeakChange = millis();
      }
    } else if (newSignal < this->oldSignal) {
      //this->peakSignal = this->oldSignal;
      long current = millis();
      if (current - this->lastPeakChange > 200) {
        this->peakSignal--;
        this->lastPeakChange = current;
      }
      this->currentSignal = this->oldSignal - 1;
    }
    if (this->currentSignal < 0) {
      this->currentSignal = 0;
    } else if (this->currentSignal > 42) {
      this->currentSignal = 42;
    }
    int start = 0;
    if (this->oldSignal < this->currentSignal) {
      start = this->oldSignal;
    } else if (this->oldSignal > this->currentSignal) {
      start = this->currentSignal;
    } else {
      start = 0;
    }
    if (this->peakSignal < 0) {
      this->peakSignal = 0;
    }

    // BG
    for (int i = start; i <= SMETER_PARTS; i++) {
      int x = 26 + i * (SMETER_PART_WIDTH + SMETER_PART_SPACE_WIDTH);
      if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42) {
        this->screen.fillRect(x, 94, SMETER_PART_WIDTH, 22, SMETER_PARTH_BG_COLOR);
      } else {
        this->screen.fillRect(x, 98, SMETER_PART_WIDTH, SMETER_PARTH_HEIGHT, SMETER_PARTH_BG_COLOR);
      }
    }

    // PEAK
    for (int i = 0; i <= SMETER_PARTS; i++) {
      if (i == this->peakSignal) {
        int x = 26 + i * (SMETER_PART_WIDTH + SMETER_PART_SPACE_WIDTH);
        // set more height on marked values
        if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42) {
          this->screen.fillRect(x, 94, SMETER_PART_WIDTH, 22, TFT_WHITE);
        } else {
          this->screen.fillRect(x, 98, SMETER_PART_WIDTH, SMETER_PARTH_HEIGHT, TFT_WHITE);
        }
      }
    }

    // SIGNAL
    for (int i = 0; i <= SMETER_PARTS; i++) {
      // real value draw
      if (i < this->currentSignal) {
        int x = 26 + i * (SMETER_PART_WIDTH + SMETER_PART_SPACE_WIDTH);
        // set more height on marked values
        if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42) {
          this->screen.fillRect(x, 94, SMETER_PART_WIDTH, 22, i <= 18 ? TFT_GREEN : TFT_RED);
        } else {
          this->screen.fillRect(x, 98, SMETER_PART_WIDTH, SMETER_PARTH_HEIGHT, i <= 18 ? TFT_GREEN : TFT_RED);
        }
      }
    }
  } else {
    if (this->peakSignal > this->currentSignal) {
      long current = millis();
      if (current - this->lastPeakChange > 200) {
        this->peakSignal--;
        if (this->peakSignal > 0) {
          this->peakSignal--;
          this->lastPeakChange = current;

          for (int i = this->peakSignal + 1; i <= SMETER_PARTS; i++) {
            int x = 26 + i * (SMETER_PART_WIDTH + SMETER_PART_SPACE_WIDTH);
            if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42) {
              this->screen.fillRect(x, 94, SMETER_PART_WIDTH, 22, SMETER_PARTH_BG_COLOR);
            } else {
              this->screen.fillRect(x, 98, SMETER_PART_WIDTH, SMETER_PARTH_HEIGHT, SMETER_PARTH_BG_COLOR);
            }
          }

          for (int i = 0; i <= SMETER_PARTS; i++) {
            if (i == this->peakSignal) {
              int x = 26 + i * (SMETER_PART_WIDTH + SMETER_PART_SPACE_WIDTH);
              // set more height on marked values
              if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42) {
                this->screen.fillRect(x, 94, SMETER_PART_WIDTH, 22, TFT_WHITE);
              } else {
                this->screen.fillRect(x, 98, SMETER_PART_WIDTH, SMETER_PARTH_HEIGHT, TFT_WHITE);
              }
            }
          }
        }
      }
    }
  }
}

void DisplayILI9488::debugBottomStr(char* str, uint64_t value) {
  this->screen.setCursor(0, 220);
  this->screen.setTextSize(2);
  this->screen.setTextColor(TFT_WHITE, TFT_BLACK);
  this->screen.printf("%s: %8llu", str, value);
}

void DisplayILI9488::debugBottomStr2(String str, uint64_t value) {
  this->screen.setCursor(0, 220);
  this->screen.setTextSize(2);
  this->screen.setTextColor(TFT_WHITE, TFT_BLACK);
  this->screen.printf("%s: %8llu", str, value);
}
