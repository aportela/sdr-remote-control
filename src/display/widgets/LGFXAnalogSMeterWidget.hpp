#ifndef SDR_REMOTE_CONTROL_LGFX_ANALOG_SMETER_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_ANALOG_SMETER_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXSMeterWidget.hpp"

class LGFXAnalogSMeterWidget : public LGFXSMeterWidget
{
private:
    lgfx::LGFX_Sprite *templateSprite;
#ifdef USE_HORIZONTAL_GRADIENT_ON_ANALOG_SMETER_WIDGET
    uint32_t getGradientColor(int index);
#endif

protected:
    void init(void) override;
    void update(int8_t dB) override;

public:
    LGFXAnalogSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr = nullptr);
    ~LGFXAnalogSMeterWidget();
};

#endif // SDR_REMOTE_CONTROL_LGFX_ANALOG_SMETER_WIDGET_H
