#ifndef SDR_REMOTE_CONTROL_LGFX_H
#define SDR_REMOTE_CONTROL_LGFX_H

#include <LovyanGFX.hpp>
#include <cstdint>
#include "../DisplayConfiguration.hpp"
#include "LGFXScreen.hpp"
#include "ScreenType.hpp"
#include "../Transceiver.hpp"
#include "../Menu.hpp"

class LGFX : public lgfx::LGFX_Device
{
private:
#ifdef DISPLAY_LOVYANGFX_ILI9488_480x320
    lgfx::Panel_ILI9488 _panel_instance;
#elif defined(DISPLAY_LOVYANGFX_ST7789_240x320)
    lgfx::Panel_ST7789 _panel_instance;
#else
#error NO DISPLAY DEFINED
#endif // DISPLAY_LOVYANGFX_ILI9488_480x320
    lgfx::Bus_SPI _bus_instance;

    SCREEN_TYPE currentScreenType = SCREEN_TYPE_NONE;

    LGFXScreen *currentScreen = nullptr;

    const TransceiverStatus *trxStatus = nullptr;
    Menu *menuPtr = nullptr;

    void DeleteCurrentScreen(void);

public:
    LGFX(uint8_t pinSDA, uint8_t pinSCL, uint8_t pinCS, uint8_t pinDC, uint8_t pinRST, uint16_t width, uint16_t height, uint8_t rotation, bool invertColors, const TransceiverStatus *trxStatus = nullptr, Menu *menuPtr = nullptr);
    ~LGFX();
    void InitScreen(SCREEN_TYPE scr);
    SCREEN_TYPE GetCurrentScreenType(void);
    bool FlipToScreen(SCREEN_TYPE scr);
    bool ToggleScreen(void);
    bool Refresh(bool force = false);
};

#endif // SDR_REMOTE_CONTROL_LGFX_H
