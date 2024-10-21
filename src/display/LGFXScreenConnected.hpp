#ifndef SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_H
#define SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_H

#include <LovyanGFX.hpp>
#include "LGFXScreen.hpp"
#include "../Transceiver.hpp"
#include "../Menu.hpp"
#include "widgets/LGFXVFOWidget.hpp"
#include "widgets/LGFXDigitalSMeterWidget.hpp"
#include "widgets/LGFXVolumeSquelchWidget.hpp"
#include "widgets/LGFXFilterWidget.hpp"
#include "widgets/LGFXMenu.hpp"

class LGFXScreenConnected : public LGFXScreen
{
private:
    // Transceiver *trx;
    uint8_t oldSignal;
    uint8_t currentSignal = 0;
    uint8_t peakSignal = 0;
    long lastPeakChange;
    LGFXVFOWidget *primaryVFOWidget = nullptr;
    LGFXVFOWidget *secondaryVFOWidget = nullptr;
    LGFXDigitalSMeterWidget *digitalSMeterWidget = nullptr;
    LGFXVolumeSquelchWidget *volumeSquelchWidget = nullptr;
    LGFXFilterWidget *filterWidget = nullptr;
    LGFXMenu *menuWidget = nullptr;

    void createDigitalSMeter();
    void refreshDigitalSMeter(uint8_t newSignal);

public:
    LGFXScreenConnected(LovyanGFX *display, const TransceiverStatus *currentTransceiverStatusPtr, Menu *menuPtr = nullptr);
    ~LGFXScreenConnected();
    bool Refresh(bool force) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_H
