#ifndef SDR_REMOTE_CONTROL_LGFX_DIGITAL_SMETER_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_DIGITAL_SMETER_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXWidget.hpp"
#include "../../Transceiver.hpp"

class LGFXDigitalSMeterWidget : public LGFXWidget
{
private:
  Transceiver *transceiverPtr;
  void createSMeter(void);
  void refreshSMeter(uint8_t level);

public:
  LGFXDigitalSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, Transceiver *transceiverPtr);
  ~LGFXDigitalSMeterWidget();
  bool refresh(bool force = false) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_DIGITAL_SMETER_WIDGET_H
