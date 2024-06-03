#ifndef SDR_REMOTE_CONTROL_DISPLAY_H
#define SDR_REMOTE_CONTROL_DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include "SSWAnimation.h"
#include "Transceiver.h"

class Display {
public:
  Display(uint16_t width, uint16_t height, uint8_t rotation, int8_t pinCS, int8_t pinDC, int8_t pinMOSI, int8_t pinSCLK, int8_t pinRST);
  void clearScreen(uint8_t color);

  void showConnectScreen(uint32_t serialBaudRate, float currentVersion);
  void hideConnectScreen(void);
  void refreshConnectScreen(void);

  void showMainScreen();
  void refreshMainScreen(Transceiver* trx);

  void debugBottomStr(char* str, uint64_t value);
  void debugBottomStr2(String, uint64_t value);
  void refreshFPS(float fps);
private:
  uint16_t width;
  uint16_t height;
  Adafruit_ST7789 screen;
  SSWAnimation* animatedScreenPtr = nullptr;
  uint8_t oldSignal;
  uint8_t currentSignal = 0;
  uint8_t peakSignal = 0;
  long lastPeakChange;

  void refreshTransmitStatus(bool isTransmitting);
  void refreshActiveVFO(uint8_t number);
  void refreshVFOMode(TRXVFOMode mode);
  void refreshVFOFreq(uint64_t frequency);
  void createDigitalSMeter();
  void refreshRNDDigitalSMeter(int newSignal);
};

#endif
