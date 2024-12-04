#ifndef SDR_REMOTE_CONTROL_LGFX_SMETER_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_SMETER_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXTransceiverStatusWidget.hpp"
#include "../../SMeter.hpp"

class LGFXSMeterWidget : public LGFXTransceiverStatusWidget
{
private:
  SMeter *smeter = nullptr;
  int8_t previousDBValue = SMETER_MIN_DB;
  int8_t previousDBSmoothedValue = SMETER_MIN_DB;
  lgfx::LGFX_Sprite *dbLabelExpSprite;
  virtual void init(void) = 0;
  virtual void update(int8_t dB) = 0;
  void refreshLabel(bool force, int8_t dB);

public:
  LGFXSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr = nullptr);
  ~LGFXSMeterWidget();
  bool refresh(bool force = false) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_SMETER_WIDGET_H
