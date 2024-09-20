#ifndef SDR_REMOTE_CONTROL_LGFX_DIGITAL_SMETER_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_DIGITAL_SMETER_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXWidget.hpp"
#include "../../Transceiver.hpp"

class LGFXDigitalSMeterWidget : public LGFXWidget
{
private:
  Transceiver *transceiverPtr;
  int8_t previousDB = -57;
  void drawBars(int8_t dB);
  void createSMeter(void);
  void refreshSMeter(int8_t dB);

public:
  LGFXDigitalSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, Transceiver *transceiverPtr);
  ~LGFXDigitalSMeterWidget();
  bool refresh(bool force = false) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_DIGITAL_SMETER_WIDGET_H
