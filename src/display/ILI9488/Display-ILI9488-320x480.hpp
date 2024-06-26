#ifndef SDR_REMOTE_CONTROL_DISPLAY_ILI9488_H
#define SDR_REMOTE_CONTROL_DISPLAY_ILI9488_H

#include "LGFX.hpp"

#include <stdio.h>
#include "../Display.hpp"
#include "SSWAnimationILI9488.hpp"
#include "SMeter-ILI9488-Digital.hpp"
#include "Menu-ILI9498.hpp"
#include "../../../Transceiver.hpp"

#include <Arduino.h>
#include <String.h>

#define DISPLAY_ILI9488_DEBUG

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
  void hideMainScreen() override;
  void refreshMainScreen(Transceiver *trx) override;
  void debug(int32_t message);

private:
  LGFX screen;
  SSWAnimationILI9488 *animatedScreenPtr = nullptr;
  SMeterILI9488Digital *smeterDigitalPtr = nullptr;
  MenuILI9488 *menuPtr = nullptr;
  void refreshTransmitStatus(bool isTransmitting);
  void refreshVFOIndex(uint8_t number, bool isActive);
  void refreshVFOFreq(uint8_t number, bool isActive, uint64_t frequency);
  void refreshVFOMode(uint8_t number, bool isActive, TRXVFOMode mode);
  void refreshVFOStep(uint8_t number, bool isActive, uint64_t step);
  void refreshRNDDigitalSMeter(uint8_t newSignal);
  void refreshVolume(uint8_t AFGain, bool isMuted);
  void refreshPassBandFilter(uint32_t LF, uint32_t HF, uint32_t BW);
};

#endif