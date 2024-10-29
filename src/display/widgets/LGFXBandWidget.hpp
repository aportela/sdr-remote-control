#ifndef SDR_REMOTE_CONTROL_LGFX_BAND_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_BAND_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXTransceiverStatusWidget.hpp"
#include "../../Transceiver.hpp"

class LGFXBandWidget : public LGFXTransceiverStatusWidget
{
private:
  uint64_t oldFrequency = 0;

  void refreshLabels(bool force, uint64_t frequency);

public:
  LGFXBandWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr = nullptr);
  ~LGFXBandWidget();
  bool refresh(bool force = false) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_BAND_WIDGET_H
