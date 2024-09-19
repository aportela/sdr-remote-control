#ifndef SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTING_H
#define SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTING_H

#include "LGFXScreen.hpp"
#include "LGFXSSWAnimation.hpp"

class LGFXScreenConnecting : public LGFXScreen
{
private:
    LGFXSSWAnimation *animatedScreenPtr = nullptr;

public:
    LGFXScreenConnecting(LovyanGFX *display);
    ~LGFXScreenConnecting();
    bool Refresh(bool force = false) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTING_H
