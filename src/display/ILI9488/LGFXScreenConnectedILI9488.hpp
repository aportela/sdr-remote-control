#ifndef SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_H
#define SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_H

#include <LovyanGFX.hpp>
#include "../LGFXScreen.hpp"
#include "../../Transceiver.hpp"

class LGFXScreenConnectedILI9488 : public LGFXScreen
{
private:
    void refreshVFOIndex(uint8_t number, bool isActive);
    void refreshVFOFreq(uint8_t number, bool isActive, uint64_t frequency);
    void refreshVFOMode(uint8_t number, bool isActive, TRXVFOMode mode);
    void refreshVFOStep(uint8_t number, bool isActive, uint64_t step);

public:
    LGFXScreenConnectedILI9488(LovyanGFX *display);
    ~LGFXScreenConnectedILI9488();
    bool Refresh(bool force = false) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_H
