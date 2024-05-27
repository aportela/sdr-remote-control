#include "Display.h"

const char* Display::connectionScreenSpriteFrames[4] = { "|", "/", "-", "\\" };

Display::Display(uint16_t width, uint16_t height, uint8_t rotation, int8_t pinCS, int8_t pinDC, int8_t pinMOSI, int8_t pinSCLK, int8_t pinRST)
  : screen(pinCS, pinDC, pinRST) {
  this->width = width;
  this->height = height;
  // Llama al método setRotation(1)
  this->screen.init(width, height);
  if (rotation > 0 && rotation < 4) {
    this->screen.setRotation(rotation);
  }
  this->screen.setTextWrap(false);
  this->screen.fillScreen(ST77XX_BLACK);
}

void Display::showConnectScreen(uint32_t serialBaudRate, float currentVersion) {
  if (connectionScreenSpriteCurrentFrame < 0) {
    this->screen.drawRect(4, 4, 312, 232, ST77XX_WHITE);
    this->screen.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    this->screen.setTextSize(2);
    this->screen.setCursor(18, 10);
    this->screen.printf("ESP32 SDR Remote Control");
    this->screen.setCursor(140, 30);
    this->screen.printf("v%.2f", currentVersion);
    this->screen.setCursor(10, 210);
    this->screen.printf("TS-2000 CAT / %d baud", serialBaudRate);
    connectionScreenSpriteCurrentFrame = 0;
  } else if (connectionScreenSpriteCurrentFrame < 4) {
    this->screen.setCursor(160, 110);
    this->screen.printf(Display::connectionScreenSpriteFrames[connectionScreenSpriteCurrentFrame]);
    connectionScreenSpriteCurrentFrame = (connectionScreenSpriteCurrentFrame + 1) % 4;
  }
}

void Display::refreshMainScreen(sdrRemoteTransceiver* trx) {
  if (trx != NULL && trx->changed) {
    if (trx->changed & TRX_CFLAG_TRANSMIT_RECEIVE_STATUS) {
      trx->changed &= ~TRX_CFLAG_TRANSMIT_RECEIVE_STATUS;
    }
    if (trx->changed & TRX_CFLAG_ACTIVE_VFO_INDEX) {
      trx->changed &= ~TRX_CFLAG_ACTIVE_VFO_INDEX;
    }
    if (trx->changed & TRX_CFLAG_ACTIVE_VFO_MODE) {
      trx->changed &= ~TRX_CFLAG_ACTIVE_VFO_MODE;
    }
    if (trx->changed & TRX_CFLAG_ACTIVE_VFO_FREQUENCY) {
      trx->changed &= ~TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
    }
    if (trx->changed & TRX_CFLAG_ACTIVE_VFO_STEP) {
      trx->changed &= ~TRX_CFLAG_ACTIVE_VFO_STEP;
    }
    if (trx->changed & TRX_CFLAG_SIGNAL_METER_LEVEL) {
      trx->changed &= ~TRX_CFLAG_SIGNAL_METER_LEVEL;
    }
    if (trx->changed & TRX_CFLAG_AUDIO_MUTE) {
      trx->changed &= ~TRX_CFLAG_AUDIO_MUTE;
    }
    if (trx->changed & TRX_CFLAG_AF_GAIN) {
      trx->changed &= ~TRX_CFLAG_AF_GAIN;
    }
    if (trx->changed & TRX_CFLAG_FILTER_LOW) {
      trx->changed &= ~TRX_CFLAG_FILTER_LOW;
    }
    if (trx->changed & TRX_CFLAG_FILTER_HIGH) {
      trx->changed &= ~TRX_CFLAG_FILTER_HIGH;
    }
  }
}

void Display::refreshTransmitStatus(bool isTransmitting) {
  if (isTransmitting) {
    this->screen.drawRect(0, 0, 29, 20, ST77XX_RED);
    this->screen.setTextColor(ST77XX_RED, ST77XX_BLACK);
  } else {
    this->screen.drawRect(0, 0, 29, 20, ST77XX_GREEN);
    this->screen.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  }
  this->screen.setCursor(3, 3);
  this->screen.setTextSize(2);
  this->screen.print(isTransmitting ? "TX" : "RX");
}

void Display::refreshActiveVFO(uint8_t number) {
  this->screen.drawRect(31, 0, 53, 20, ST77XX_WHITE);
  this->screen.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  this->screen.setCursor(34, 3);
  this->screen.setTextSize(2);
  this->screen.printf("VFO%d", number);
}

void Display::refreshVFOMode(uint8_t mode) {
  this->screen.drawRect(86, 0, 43, 20, ST77XX_WHITE);
  this->screen.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  this->screen.setCursor(89, 3);
  this->screen.setTextSize(2);
  switch (mode) {
    case 0:
      this->screen.printf("DSB");
      break;
    case 1:
      this->screen.printf("LSB");
      break;
    case 2:
      this->screen.printf("USB");
      break;
    case 3:
      this->screen.printf("CW_U");  // CW (upper sideband)
      break;
    case 4:
      this->screen.printf("FM");
      break;
    case 5:
      this->screen.printf("SAM");  // (synchronous AM, includes ECSS)
      break;
    case 6:
      this->screen.printf("   ");  // not used
      break;
    case 7:
      this->screen.printf("CWL");  // CW (lower sideband)
      break;
    case 8:
      this->screen.printf("WFM");
      break;
    case 9:
      this->screen.printf("BFM");
      break;
    default:
      this->screen.printf("???");  // invalid value
      break;
  }
}

void Display::refreshVFOFreq(uint64_t frequency) {
  this->screen.setTextSize(3);
  char formattedFrequency[16];
  char nformattedFrequency[12];
  /*
  sprintf(
    formattedFrequency,
    "%03lld.%03lld.%03lld.%03lld",
    frequency / 1000000000,
    (frequency % 1000000000) / 1000000,
    (frequency % 1000000) / 1000,
    frequency % 1000
  );
  */
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
  this->screen.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  this->screen.print(formattedFrequency);
}

#define SMETER_PARTS 42
#define SMETER_PART_WIDTH 4
#define SMETER_PART_SPACE_WIDTH 2
#define SMETER_PARTH_HEIGHT 18
#define SMETER_PARTH_HEIGHT_SEPARATOR 20
#define SMETER_PARTH_BG_COLOR 0x8410

void Display::createDigitalSMeter() {
  randomSeed(analogRead(0));
  this->screen.setTextSize(2);
  this->screen.setTextColor(ST77XX_WHITE);
  this->screen.setCursor(36, 70);
  this->screen.print("1 3 5 7 9");
  this->screen.setCursor(165, 70);
  this->screen.print("+20 +40 +60");
  this->screen.setCursor(6, 98);
  this->screen.print("S");
  this->screen.setCursor(292, 98);
  this->screen.print("dB");
  this->screen.drawFastVLine(23, 94, 22, ST77XX_WHITE);
  this->screen.drawFastVLine(284, 94, 22, ST77XX_WHITE);
  this->screen.drawFastHLine(23, 116, 262, ST77XX_WHITE);

  for (int i = 0; i <= SMETER_PARTS; i++) {
    int x = 26 + i * (SMETER_PART_WIDTH + SMETER_PART_SPACE_WIDTH);
    if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 26 || i == 34 || i == 42) {
      this->screen.fillRect(x, 94, SMETER_PART_WIDTH, 22, SMETER_PARTH_BG_COLOR);
    } else {
      this->screen.fillRect(x, 98, SMETER_PART_WIDTH, SMETER_PARTH_HEIGHT, SMETER_PARTH_BG_COLOR);
    }
  }
}

void Display::refreshRNDDigitalSMeter(int newSignal) {
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

    /*
    this->screen.setCursor(20, 160);
    this->screen.setTextSize(2);
    this->screen.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    this->screen.printf("Current: %u", this->currentSignal);
    this->screen.setCursor(20, 180);
    this->screen.setTextSize(2);
    this->screen.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    this->screen.printf("Old: %u", this->oldSignal);

    this->screen.setCursor(20, 200);
    this->screen.setTextSize(2);
    this->screen.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    this->screen.printf("Peak: %u", this->peakSignal);
    */

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
          this->screen.fillRect(x, 94, SMETER_PART_WIDTH, 22, ST77XX_WHITE);
        } else {
          this->screen.fillRect(x, 98, SMETER_PART_WIDTH, SMETER_PARTH_HEIGHT, ST77XX_WHITE);
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
          this->screen.fillRect(x, 94, SMETER_PART_WIDTH, 22, i <= 18 ? ST77XX_GREEN : ST77XX_RED);
        } else {
          this->screen.fillRect(x, 98, SMETER_PART_WIDTH, SMETER_PARTH_HEIGHT, i <= 18 ? ST77XX_GREEN : ST77XX_RED);
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
                this->screen.fillRect(x, 94, SMETER_PART_WIDTH, 22, ST77XX_WHITE);
              } else {
                this->screen.fillRect(x, 98, SMETER_PART_WIDTH, SMETER_PARTH_HEIGHT, ST77XX_WHITE);
              }
            }
          }
        }
      }
    }
  }
}

void Display::debugBottomStr(char* str, uint64_t value) {
  this->screen.setCursor(0, 220);
  this->screen.setTextSize(2);
  this->screen.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  this->screen.printf("%s: %8llu", str, value);
}
