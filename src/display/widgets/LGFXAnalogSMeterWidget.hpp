#ifndef SDR_REMOTE_CONTROL_LGFX_ANALOG_SMETER_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_ANALOG_SMETER_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXTransceiverStatusWidget.hpp"
#include "../../SMeter.hpp"

class LGFXAnalogSMeterWidget : public LGFXTransceiverStatusWidget
{
private:
    SMeter *smeter = nullptr;
    int8_t previousDBValue = -57;
    lgfx::LGFX_Sprite *expSprite;
    lgfx::LGFX_Sprite *templateSprite;
    void refreshSMeterDBLabel(bool force, int8_t dB);
#ifdef USE_HORIZONTAL_GRADIENT_ON_ANALOG_SMETER_WIDGET
    uint32_t getGradientColor(int index);
#endif
    void createSMeter(void);
    void refreshSMeter(int8_t dB);

public:
    LGFXAnalogSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr = nullptr);
    ~LGFXAnalogSMeterWidget();
    bool refresh(bool force = false) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_ANALOG_SMETER_WIDGET_H
