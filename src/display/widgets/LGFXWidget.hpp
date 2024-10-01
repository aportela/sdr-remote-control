#ifndef SDR_REMOTE_CONTROL_LGFX_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_WIDGET_H

#include <LovyanGFX.hpp>
#include "Widget.hpp"

class LGFXWidget : public Widget
{
protected:
    LovyanGFX *parentDisplayPtr = nullptr;

public:
    LGFXWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding = 0);
    ~LGFXWidget();
    // bool refresh(bool force = false) override = 0;
};

#endif // SDR_REMOTE_CONTROL_LGFX_WIDGET_H
