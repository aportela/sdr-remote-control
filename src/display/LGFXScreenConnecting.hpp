#ifndef SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTING_H
#define SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTING_H

#include "LGFXScreen.hpp"
#include "widgets/LGFXSSWAnimationWidget.hpp"

class LGFXScreenConnecting : public LGFXScreen
{
private:
    LGFXSSWAnimationWidget *animatedScreenPtr = nullptr;

public:
    LGFXScreenConnecting(LovyanGFX *display);
    ~LGFXScreenConnecting();
    bool Refresh(bool force) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTING_H
