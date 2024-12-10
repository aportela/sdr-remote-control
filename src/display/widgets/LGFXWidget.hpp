#ifndef SDR_REMOTE_CONTROL_LGFX_WIDGET_H
#define SDR_REMOTE_CONTROL_LGFX_WIDGET_H

#include <LovyanGFX.hpp>
#include "Widget.hpp"

#include "../../DisplayConfiguration.hpp"

#define LGFXWIDGET_ALLOW_OVERFLOWS true

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
    bool setCursor(uint16_t x, uint16_t y);
    bool printAtCursor(uint16_t x, uint16_t y, const char *str);
    bool drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
    bool fillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
    bool drawRoundRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t radius, uint32_t color);
    bool fillRoundRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t radius, uint32_t color);
    bool drawFastHLine(uint16_t x, uint16_t y, uint16_t width, uint32_t color);
    bool drawFastVLine(uint16_t x, uint16_t y, uint16_t height, uint32_t color);
    bool drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);
};

#endif // SDR_REMOTE_CONTROL_LGFX_WIDGET_H
