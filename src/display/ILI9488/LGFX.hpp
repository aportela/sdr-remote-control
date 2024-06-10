#ifndef SDR_REMOTE_CONTROL_LGFX_H
#define SDR_REMOTE_CONTROL_LGFX_H

#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9488 _panel_instance;
  lgfx::Bus_SPI _bus_instance;
public:
  LGFX(void);
};

#endif