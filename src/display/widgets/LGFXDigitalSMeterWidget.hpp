#ifndef SDR_REMOTE_CONTROL_LGFX_DIGITAL_SMETER_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_DIGITAL_SMETER_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXSMeterWidget.hpp"

class LGFXDigitalSMeterWidget : public LGFXSMeterWidget
{
private:
  void drawBars(int8_t dB);

protected:
  void init(void) override;
  void update(int8_t dB) override;

public:
  LGFXDigitalSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr = nullptr);
  ~LGFXDigitalSMeterWidget();
};

#endif // SDR_REMOTE_CONTROL_LGFX_DIGITAL_SMETER_WIDGET_H
