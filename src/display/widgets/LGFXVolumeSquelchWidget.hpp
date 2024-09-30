#ifndef SDR_REMOTE_CONTROL_LGFX_VOLUME_SQUELCH_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_VOLUME_SQUELCH_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXWidget.hpp"
#include "../../Transceiver.hpp"

class LGFXVolumeSquelchWidget : public LGFXWidget
{
private:
  Transceiver *transceiverPtr;

  void refreshVolume(bool force, uint8_t value);
  void refreshSquelch(bool force, uint8_t value);
  void setMuted(bool force, uint8_t value);

public:
  LGFXVolumeSquelchWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, Transceiver *transceiverPtr);
  ~LGFXVolumeSquelchWidget();
  bool refresh(bool force, const TransceiverStatus *currentTrxStatus = nullptr) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_VOLUME_SQUELCH_WIDGET_H
