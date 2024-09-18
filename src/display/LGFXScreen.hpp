#ifndef SDR_REMOTE_CONTROL_LGFX_SCREEN_H
#define SDR_REMOTE_CONTROL_LGFX_SCREEN_H

#include <LovyanGFX.hpp>
#include "IScreen.hpp"

class LGFXScreen : public IScreen
{
protected:
    LovyanGFX *parentDisplay = nullptr;

public:
    LGFXScreen(LovyanGFX *display);
    ~LGFXScreen();
    virtual bool Refresh(bool force = false) = 0;
};

#endif // SDR_REMOTE_CONTROL_LGFX_SCREEN_H
