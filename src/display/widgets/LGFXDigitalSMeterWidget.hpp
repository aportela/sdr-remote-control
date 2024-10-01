#ifndef SDR_REMOTE_CONTROL_LGFX_DIGITAL_SMETER_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_DIGITAL_SMETER_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXWidget.hpp"
#include "../../Transceiver.hpp"

class LGFXDigitalSMeterWidget : public LGFXWidget
{
private:
  int8_t previousDBValue = -57;
  void drawBars(int8_t dB);
  void createSMeter(void);
  void refreshSMeterDBLabel(void);
  void refreshSMeter(int8_t dB);

public:
  LGFXDigitalSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr);
  ~LGFXDigitalSMeterWidget();
  bool refresh(bool force, const TransceiverStatus *currentTransceiverStatusPtr);
};

#endif // SDR_REMOTE_CONTROL_LGFX_DIGITAL_SMETER_WIDGET_H
