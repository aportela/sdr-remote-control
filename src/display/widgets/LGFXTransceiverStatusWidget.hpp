#ifndef SDR_REMOTE_CONTROL_LGFX_TRANSCEIVER_STATUS_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_TRANSCEIVER_STATUS_WIDGET_H

#include <LovyanGFX.hpp>
#include "LGFXWidget.hpp"
#include "../../Transceiver.hpp"

class LGFXTransceiverStatusWidget : public LGFXWidget
{
protected:
    const TransceiverStatus *currentTransceiverStatusPtr = nullptr;

public:
    LGFXTransceiverStatusWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding = 0, const TransceiverStatus *currentTransceiverStatusPtr = nullptr);
    ~LGFXTransceiverStatusWidget();
    virtual bool refresh(bool force = false) = 0;
};

#endif // SDR_REMOTE_CONTROL_LGFX_TRANSCEIVER_STATUS_WIDGET_H
