#ifndef SDR_REMOTE_CONTROL_LGFX_DUAL_VFO_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_DUAL_VFO_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXTransceiverStatusWidget.hpp"
#include "../../Transceiver.hpp"

class LGFXVFOWidget : public LGFXTransceiverStatusWidget
{
private:
  bool isActive = false;
  uint8_t index = 0;
  uint64_t previousFrequency = 0;
  TrxVFOMode previousMode = TRX_VFO_MODE_ERROR;
  uint64_t previousFrequencyStep = 0;

  void printIndex();
  void printFrequency(uint64_t frequency);
  void getModeLabel(TrxVFOMode mode, char *label, size_t labelSize);
  void printMode(TrxVFOMode mode);
  void printFrequencyStep(uint64_t step);

public:
  LGFXVFOWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, uint8_t index, const TransceiverStatus *currentTransceiverStatusPtr = nullptr);
  ~LGFXVFOWidget();
  bool refresh(bool force = false) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_DUAL_VFO_WIDGET_H
