#ifndef SDR_REMOTE_CONTROL_LGFX_ANALOG_SMETER_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_ANALOG_SMETER_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXTransceiverStatusWidget.hpp"

class LGFXAnalogSMeterWidget : public LGFXTransceiverStatusWidget
{
private:
public:
    LGFXAnalogSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr = nullptr);
    ~LGFXAnalogSMeterWidget();
    bool refresh(bool force = false) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_ANALOG_SMETER_WIDGET_H
