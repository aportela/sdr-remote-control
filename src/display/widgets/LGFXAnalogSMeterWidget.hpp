#ifndef SDR_REMOTE_CONTROL_LGFX_ANALOG_SMETER_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_ANALOG_SMETER_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXSMeterWidget.hpp"

class LGFXAnalogSMeterWidget : public LGFXSMeterWidget
{
private:
    lgfx::LGFX_Sprite *templateSprite;
    int16_t getVLineValueXOffset(int8_t dB);

protected:
    void init(void) override;
    void update(int8_t dB) override;

public:
    LGFXAnalogSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr = nullptr);
    ~LGFXAnalogSMeterWidget();
};

#endif // SDR_REMOTE_CONTROL_LGFX_ANALOG_SMETER_WIDGET_H
