#ifndef SDR_REMOTE_CONTROL_LGFX_FILTER_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_FILTER_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXWidget.hpp"
#include "../../Transceiver.hpp"

class LGFXFilterWidget : public LGFXWidget
{
private:
  Transceiver *transceiverPtr;

  void refreshPlot(uint64_t minLowCut, uint64_t minHighCut, uint64_t maxLowCut, uint64_t maxHighCut, uint64_t currentLowCut, uint64_t currentHighCut);
  void refreshLabels(uint64_t lowCut, uint64_t highCut);

public:
  LGFXFilterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, Transceiver *transceiverPtr);
  ~LGFXFilterWidget();
  bool refresh(bool force, const TransceiverStatus *currentTrxStatus = nullptr) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_FILTER_WIDGET_H
