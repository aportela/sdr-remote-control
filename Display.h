#ifndef SDR_REMOTE_CONTROL_DISPLAY_H
#define SDR_REMOTE_CONTROL_DISPLAY_H

#define DEBUG_FPS // uncomment this ONLY for debug/benchmark display

#include "IDisplay.h"
#include "Transceiver.h"
#ifdef DEBUG_FPS
#include "FPSDebug.h"
#endif

class Display : public IDisplay {
public:
  virtual void clearScreen(uint8_t color) override = 0;
  virtual void showConnectScreen(uint16_t serialBaudRate, float currentVersion) override = 0;
  virtual void hideConnectScreen(void) override = 0;
  virtual void refreshConnectScreen() override = 0;
  virtual void showMainScreen() override = 0;
  virtual void refreshMainScreen(Transceiver* trx) override = 0;
protected:
  uint16_t width;
  uint16_t height;
  uint8_t oldSignal;
  uint8_t currentSignal = 0;
  uint8_t peakSignal = 0;
  long lastPeakChange;
#ifdef DEBUG_FPS
  FPSDebug* fpsDebug;
#endif
};

#endif