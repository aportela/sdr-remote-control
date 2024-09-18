#ifndef SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_H
#define SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_H

#include <LovyanGFX.hpp>
#include "../LGFXScreen.hpp"

class LGFXScreenConnectedST7789 : public LGFXScreen
{
private:
    uint16_t previousFPS = 0;

public:
    LGFXScreenConnectedST7789(LovyanGFX *display);
    ~LGFXScreenConnectedST7789();
    bool Refresh(bool force = false) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_H
