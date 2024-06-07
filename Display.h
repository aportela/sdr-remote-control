#ifndef SDR_REMOTE_CONTROL_DISPLAY_H
#define SDR_REMOTE_CONTROL_DISPLAY_H

#include "IDisplay.h"
#include "Transceiver.h"

class Display : public IDisplay {
public:

  virtual void clearScreen(uint8_t color) override = 0;
  virtual void showConnectScreen(uint32_t serialBaudRate, float currentVersion) override = 0;
  virtual void hideConnectScreen(void) override = 0;
  virtual void refreshConnectScreen(void) override = 0;

  virtual void showMainScreen() override = 0;
  virtual void refreshMainScreen(Transceiver* trx, float fps) override = 0;

protected:
  uint16_t width;
  uint16_t height;
  uint8_t oldSignal;
  uint8_t currentSignal = 0;
  uint8_t peakSignal = 0;
  long lastPeakChange;
};

#endif