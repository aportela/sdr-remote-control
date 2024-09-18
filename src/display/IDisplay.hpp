#ifndef SDR_REMOTE_CONTROL_DISPLAY_INTERFACE
#define SDR_REMOTE_CONTROL_DISPLAY_INTERFACE

#include <stdint.h>
#include "../Transceiver.hpp"

class IDisplay
{
public:
  virtual ~IDisplay() {}
  virtual void clearScreen(uint8_t color) = 0;
  virtual void showConnectScreen(uint16_t serialBaudRate, float currentVersion) = 0;
  virtual void hideConnectScreen(void) = 0;
  virtual void refreshConnectScreen() = 0;
  virtual void showMainScreen() = 0;
  virtual void hideMainScreen() = 0;
  virtual void refreshMainScreen(Transceiver *trx) = 0;
};

#endif