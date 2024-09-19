#ifndef SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_H
#define SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_H

#include <LovyanGFX.hpp>
#include "LGFXScreen.hpp"
#include "../Transceiver.hpp"
#include "widgets/LGFXDualVFOWidget.hpp"

class LGFXScreenConnected : public LGFXScreen
{
private:
    Transceiver *trx;
    uint8_t oldSignal;
    uint8_t currentSignal = 0;
    uint8_t peakSignal = 0;
    long lastPeakChange;
    LGFXDualVFOWidget *vfoWidget = nullptr;

    void refreshVFOIndex(uint8_t number, bool isActive);
    void refreshVFOFreq(uint8_t number, bool isActive, uint64_t frequency);
    void refreshVFOMode(uint8_t number, bool isActive, TrxVFOMode mode);
    void refreshVFOStep(uint8_t number, bool isActive, uint64_t step);
    void createDigitalSMeter();
    void refreshDigitalSMeter(uint8_t newSignal);

public:
    LGFXScreenConnected(LovyanGFX *display, Transceiver *trx);
    ~LGFXScreenConnected();
    bool Refresh(bool force = false) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_H
