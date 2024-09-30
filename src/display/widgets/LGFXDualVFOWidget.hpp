#ifndef SDR_REMOTE_CONTROL_LGFX_DUAL_VFO_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_DUAL_VFO_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXWidget.hpp"
#include "../../Transceiver.hpp"

class LGFXDualVFOWidget : public LGFXWidget
{
private:
  Transceiver *transceiverPtr;

  void refreshVFOIndex(uint8_t number, bool isActive);
  void refreshVFOFreq(uint8_t number, bool isActive, uint64_t frequency);
  void refreshVFOMode(uint8_t number, bool isActive, TrxVFOMode mode);
  void refreshVFOFrequencyStep(uint8_t number, bool isActive, uint64_t step);

public:
  LGFXDualVFOWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, Transceiver *transceiverPtr);
  ~LGFXDualVFOWidget();
  bool refresh(bool force, const TransceiverStatus *currentTrxStatus = nullptr) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_DUAL_VFO_WIDGET_H
