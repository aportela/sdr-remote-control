#ifndef SDR_REMOTE_CONTROL_LGFX_H
#define SDR_REMOTE_CONTROL_LGFX_H

#include <LovyanGFX.hpp>
#include <stdint.h>
#include "LGFXScreen.hpp"
#include "../ScreenType.hpp"

class LGFX : public lgfx::LGFX_Device
{
private:
    lgfx::Panel_ST7789 _panel_instance;
    lgfx::Bus_SPI _bus_instance;

    SCREEN_TYPE currentScreenType = SCREEN_TYPE_NONE;

    LGFXScreen *currentScreen = nullptr;

    void DeleteCurrentScreen(void);

public:
    LGFX(uint8_t PIN_SDA, uint8_t PIN_SCL, uint8_t PIN_CS, uint8_t PIN_DC, uint8_t PIN_RST, uint16_t width, uint16_t height, uint8_t rotation, bool invertColors);
    ~LGFX();
    void InitScreen(SCREEN_TYPE scr);
    SCREEN_TYPE GetCurrentScreenType(void);
    bool FlipToScreen(SCREEN_TYPE scr);
    bool ToggleScreen(void);
    bool Refresh(void);
};

#endif // SDR_REMOTE_CONTROL_LGFX_H
