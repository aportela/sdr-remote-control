#ifndef SDR_REMOTE_CONTROL_LGFX_SCREEN_H
#define SDR_REMOTE_CONTROL_LGFX_SCREEN_H

#include <cstdint>
#include <LovyanGFX.hpp>
#include "IScreen.hpp"
#include "../DisplayConfiguration.hpp"

class LGFXScreen : public IScreen
{
protected:
    LovyanGFX *parentDisplay = nullptr;
#ifdef DEBUG_FPS

    uint16_t previousFPS = 0;

#endif // DEBUG_FPS

public:
    LGFXScreen(LovyanGFX *display);
    ~LGFXScreen();
    virtual bool Refresh(bool force) = 0;
};

#endif // SDR_REMOTE_CONTROL_LGFX_SCREEN_H
