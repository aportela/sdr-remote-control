#ifndef SDR_REMOTE_CONTROL_DISPLAY_ST7789_H
#define SDR_REMOTE_CONTROL_DISPLAY_ST7789_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include <stdio.h>
#include "../Display.hpp"
#include "SSWAnimationST7789.hpp"
#include "../../Transceiver.hpp"

class DisplayST7789 : public Display
{
public:
  DisplayST7789(uint16_t width, uint16_t height, uint8_t rotation, int8_t pinCS, int8_t pinDC, int8_t pinMOSI, int8_t pinSCLK, int8_t pinRST);
  ~DisplayST7789();
  void clearScreen(uint8_t color) override;
  void showConnectScreen(uint16_t serialBaudRate, float currentVersion) override;
  void hideConnectScreen(void) override;
  void refreshConnectScreen() override;
  void showMainScreen() override;
  void hideMainScreen() override;
  void refreshMainScreen(Transceiver *trx) override;

private:
  Adafruit_ST7789 screen;
  SSWAnimationST7789 *animatedScreenPtr = nullptr;
  void refreshTransmitStatus(bool isTransmitting);
  void refreshActiveVFO(uint8_t number);
  void refreshVFOMode(TRXVFOMode mode);
  void refreshFPS(uint16_t fps);
  void refreshVFOFreq(uint64_t frequency);
  void createDigitalSMeter();
  void refreshRNDDigitalSMeter(int newSignal);
};

#endif