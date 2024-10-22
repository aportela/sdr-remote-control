#ifndef SDR_REMOTE_CONTROL_LGFX_FILTER_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_FILTER_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXTransceiverStatusWidget.hpp"
#include "../../Transceiver.hpp"

class LGFXFilterWidget : public LGFXTransceiverStatusWidget
{
private:
  uint32_t oldLFValue = 0;
  uint32_t oldHFValue = 0;
  uint8_t previousCurrentVFOIndex = 0;
  lgfx::LGFX_Sprite *plotSprite;

  void refreshPlot(bool force, uint64_t minLowCut, uint64_t minHighCut, uint64_t maxLowCut, uint64_t maxHighCut, uint64_t currentLowCut, uint64_t currentHighCut);
  void refreshLabels(bool force, uint64_t lowCut, uint64_t highCut);

public:
  LGFXFilterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr = nullptr);
  ~LGFXFilterWidget();
  bool refresh(bool force = false) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_FILTER_WIDGET_H
