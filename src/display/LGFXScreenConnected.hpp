#ifndef SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_H
#define SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_H

#include <LovyanGFX.hpp>
#include "LGFXScreen.hpp"
#include "../Transceiver.hpp"
#include "../Menu.hpp"
#include "widgets/LGFXVFOWidget.hpp"
#ifdef USE_ANALOG_SMETER_WIDGET
#include "widgets/LGFXAnalogSMeterWidget.hpp"
#else
#include "widgets/LGFXDigitalSMeterWidget.hpp"
#endif
#include "widgets/LGFXVolumeSquelchWidget.hpp"
#include "widgets/LGFXBandWidget.hpp"
#include "widgets/LGFXFilterWidget.hpp"
#include "widgets/LGFXMenu.hpp"

class LGFXScreenConnected : public LGFXScreen
{
private:
    LGFXVFOWidget *primaryVFOWidget = nullptr;
    LGFXVFOWidget *secondaryVFOWidget = nullptr;
#ifdef USE_ANALOG_SMETER_WIDGET
    LGFXAnalogSMeterWidget *analogSMeterWidget = nullptr;
#else
    LGFXDigitalSMeterWidget *digitalSMeterWidget = nullptr;
#endif
    LGFXVolumeSquelchWidget *volumeSquelchWidget = nullptr;
    LGFXBandWidget *bandWidget = nullptr;
    LGFXFilterWidget *filterWidget = nullptr;
    LGFXMenu *menuWidget = nullptr;

public:
    LGFXScreenConnected(LovyanGFX *display, const TransceiverStatus *currentTransceiverStatusPtr, Menu *menuPtr = nullptr);
    ~LGFXScreenConnected();
    bool Refresh(bool force) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_H
