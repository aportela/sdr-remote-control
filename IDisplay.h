#ifndef SDR_REMOTE_CONTROL_IDISPLAY
#define SDR_REMOTE_CONTROL_IDISPLAY

#include "Transceiver.h"

class IDisplay {
public:
  virtual ~IDisplay() {}
  virtual void clearScreen(uint8_t color) = 0;

  virtual void showConnectScreen(uint16_t serialBaudRate, float currentVersion) = 0;
  virtual void hideConnectScreen(void) = 0;
  virtual void refreshConnectScreen(void) = 0;

  virtual void showMainScreen() = 0;
  virtual void refreshMainScreen(Transceiver* trx, float fps) = 0;
};

#endif