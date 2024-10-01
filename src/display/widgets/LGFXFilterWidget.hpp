#ifndef SDR_REMOTE_CONTROL_LGFX_FILTER_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_FILTER_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXWidget.hpp"
#include "../../Transceiver.hpp"

class LGFXFilterWidget : public LGFXWidget
{
private:
  uint32_t oldLFValue = 0;
  uint32_t oldHFValue = 0;

  void refreshPlot(uint64_t minLowCut, uint64_t minHighCut, uint64_t maxLowCut, uint64_t maxHighCut, uint64_t currentLowCut, uint64_t currentHighCut);
  void refreshLabels(uint64_t lowCut, uint64_t highCut);

public:
  LGFXFilterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr);
  ~LGFXFilterWidget();
  bool refresh(bool force, const TransceiverStatus *currentTransceiverStatusPtr);
};

#endif // SDR_REMOTE_CONTROL_LGFX_FILTER_WIDGET_H
