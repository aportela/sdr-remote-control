#ifndef SDR_REMOTE_CONTROL_LGFX_ANALOG_SMETER_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_ANALOG_SMETER_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXTransceiverStatusWidget.hpp"

class LGFXAnalogSMeterWidget : public LGFXTransceiverStatusWidget
{
private:
    int8_t previousDBValue = -57;
    lgfx::LGFX_Sprite *templateSprite;
    void refreshAnalogBar(int8_t value);

public:
    LGFXAnalogSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr = nullptr);
    ~LGFXAnalogSMeterWidget();
    bool refresh(bool force = false) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_ANALOG_SMETER_WIDGET_H
