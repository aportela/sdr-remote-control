#ifndef SDR_REMOTE_CONTROL_LGFX_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_WIDGET_H

#include <LovyanGFX.hpp>
#include "Widget.hpp"

#include "../../DisplayConfiguration.hpp"

class LGFXWidget : public Widget
{
private:
    const lgfx::IFont *previousFont;

protected:
    LovyanGFX *parentDisplayPtr = nullptr;

public:
    LGFXWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding = 0);
    ~LGFXWidget();
    virtual bool refresh(bool force = false) = 0;
    void restorePreviousFont(void);
};

#endif // SDR_REMOTE_CONTROL_LGFX_WIDGET_H
