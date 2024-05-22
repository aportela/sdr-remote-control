#include "Display.h"

const char* Display::connectionScreenSpriteFrames[4] = { "|", "/", "-", "\\" };

void Display::init(uint8_t width, uint8_t height) {
  this->width = width;
  this->height = height;
}

void Display::showConnectScreen(Adafruit_ST7789* display, uint32_t serialBaudRate, float currentVersion) {
  if (display != NULL) {
    if (connectionScreenSpriteCurrentFrame < 0) {
      display->drawRect(4, 4, 312, 232, ST77XX_WHITE);
      display->setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      display->setTextSize(2);
      display->setCursor(18, 10);
      display->printf("ESP32 SDR Remote Control");
      display->setCursor(140, 30);
      display->printf("v%.2f", currentVersion);
      display->setCursor(10, 210);
      display->printf("TS-2000 CAT / %d baud", serialBaudRate);
      connectionScreenSpriteCurrentFrame = 0;
    } else if (connectionScreenSpriteCurrentFrame < 4) {
      display->setCursor(160, 110);
      display->printf(Display::connectionScreenSpriteFrames[connectionScreenSpriteCurrentFrame]);
      connectionScreenSpriteCurrentFrame = (connectionScreenSpriteCurrentFrame + 1) % 4;
    }
  }
}

void Display::refreshTransmitStatus(Adafruit_ST7789* display, bool isTransmitting) {
  if (display != NULL) {
    if (isTransmitting) {
      display->drawRect(0, 0, 29, 20, ST77XX_RED);
      display->setTextColor(ST77XX_RED, ST77XX_BLACK);
    } else {
      display->drawRect(0, 0, 29, 20, ST77XX_GREEN);
      display->setTextColor(ST77XX_GREEN, ST77XX_BLACK);
    }
    display->setCursor(3, 3);
    display->setTextSize(2);
    display->print(isTransmitting ? "TX" : "RX");
  }
}

void Display::refreshActiveVFO(Adafruit_ST7789* display, uint8_t number) {
  if (display != NULL) {
    display->drawRect(31, 0, 53, 20, ST77XX_WHITE);
    display->setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    display->setCursor(34, 3);
    display->setTextSize(2);
    display->printf("VFO%d", number);
  }
}

void Display::refreshVFOMode(Adafruit_ST7789* display, uint8_t mode) {
  if (display != NULL) {
    display->drawRect(86, 0, 43, 20, ST77XX_WHITE);
    display->setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    display->setCursor(89, 3);
    display->setTextSize(2);
    switch (mode) {
      case 0:
        display->printf("DSB");
        break;
      case 1:
        display->printf("LSB");
        break;
      case 2:
        display->printf("USB");
        break;
      case 3:
        display->printf("CW_U");  // CW (upper sideband)
        break;
      case 4:
        display->printf("FM");
        break;
      case 5:
        display->printf("SAM");  // (synchronous AM, includes ECSS)
        break;
      case 6:
        display->printf("   ");  // not used
        break;
      case 7:
        display->printf("CWL");  // CW (lower sideband)
        break;
      case 8:
        display->printf("WFM");
        break;
      case 9:
        display->printf("BFM");
        break;
      default:
        display->printf("???");  // invalid value
        break;
    }
  }
}

void Display::refreshVFOFreq(Adafruit_ST7789* display, uint64_t frequency) {
  display->setTextSize(3);
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

  display->setCursor(35, 30);
  display->setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  display->print(formattedFrequency);
}
