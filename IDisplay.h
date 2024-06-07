#ifndef SDR_REMOTE_CONTROL_IDISPLAY
#define SDR_REMOTE_CONTROL_IDISPLAY

#include "Transceiver.h"

class IDisplay {
public:
  virtual void clearScreen(uint8_t color);

  virtual void showConnectScreen(uint32_t serialBaudRate, float currentVersion);
  virtual void hideConnectScreen(void);
  virtual void refreshConnectScreen(void);

  virtual void showMainScreen();
  virtual void refreshMainScreen(Transceiver* trx, float fps);
  
};

#endif