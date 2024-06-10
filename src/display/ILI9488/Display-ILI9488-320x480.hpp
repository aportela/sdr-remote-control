#ifndef SDR_REMOTE_CONTROL_DISPLAY_ILI9488_H
#define SDR_REMOTE_CONTROL_DISPLAY_ILI9488_H

#include "LGFX.hpp"

#include <stdio.h>
#include "../Display.hpp"
#include "SSWAnimationILI9488.hpp"
#include "../../../Transceiver.hpp"

class DisplayILI9488 : public Display
{
public:
  DisplayILI9488(uint16_t width, uint16_t height, uint8_t rotation, bool invertDisplayColors);
  ~DisplayILI9488();
  void clearScreen(uint8_t color) override;
  void showConnectScreen(uint16_t serialBaudRate, float currentVersion) override;
  void hideConnectScreen(void) override;
  void refreshConnectScreen() override;
  void showMainScreen() override;
  void refreshMainScreen(Transceiver *trx) override;

private:
  LGFX screen;
  SSWAnimationILI9488 *animatedScreenPtr = nullptr;
  void refreshTransmitStatus(bool isTransmitting);
  void refreshActiveVFO(uint8_t number);
  void refreshVFOMode(TRXVFOMode mode);
  void refreshFPS(float fps);
  void refreshVFOFreq(uint64_t frequency);
  void createDigitalSMeter();
  void refreshRNDDigitalSMeter(int newSignal);
};

#endif