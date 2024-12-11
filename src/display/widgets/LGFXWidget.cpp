#include "LGFXWidget.hpp"

LGFXWidget::LGFXWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding) : Widget(width, height, xOffset, yOffset, padding), parentDisplayPtr(displayPtr)
{
    if (displayPtr != nullptr)
    {
        this->previousFont = this->parentDisplayPtr->getFont();
#ifdef DEBUG_SCREEN_WIDGETS_BOUNDS
        this->parentDisplayPtr->drawRect(xOffset, yOffset, width, height, DEBUG_SCREEN_WIDGETS_BOUNDS_BORDER_COLOR);
#endif // DEBUG_SCREEN_WIDGETS_BOUNDS
    }
}

LGFXWidget::~LGFXWidget()
{
    this->parentDisplayPtr = nullptr;
}

void LGFXWidget::restorePreviousFont(void)
{
    this->parentDisplayPtr->setFont(this->previousFont);
}

bool LGFXWidget::setCursor(uint16_t x, uint16_t y)
{
    if (x <= (this->width - this->padding) && y <= (this->height - this->padding))
    {
        this->parentDisplayPtr->setCursor(this->xOffsetWithPadding + x, this->yOffsetWithPadding + y);
        return (true);
    }
    else
    {
        return (false);
    }
}

bool LGFXWidget::printAtCursor(uint16_t x, uint16_t y, const char *str)
{
    if (this->setCursor(x, y))
    {
        this->parentDisplayPtr->print(str);
        return (true);
    }
    else
    {
        return (false);
    }
}

bool LGFXWidget::printAtCursorWithColorAndSize(uint16_t x, uint16_t y, const char *str, int textColor, int backgroundColor, uint8_t fontSize)
{
    this->parentDisplayPtr->setTextColor(textColor, backgroundColor);
    this->parentDisplayPtr->setTextSize(fontSize);
    if (this->setCursor(x, y))
    {
        this->parentDisplayPtr->print(str);
        return (true);
    }
    else
    {
        return (false);
    }
}

bool LGFXWidget::drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, int color)
{
    if (x <= (this->width - this->padding) && y <= (this->height - this->padding))
    {
#ifdef LGFXWIDGET_ALLOW_OVERFLOWS
        this->parentDisplayPtr->drawRect(this->xOffsetWithPadding + x, this->yOffsetWithPadding + y, width, height, color);
        return (true);
#else
#error TODO
#endif
    }
    else
    {
        return (false);
    }
}

bool LGFXWidget::fillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, int color)
{
    if (x <= (this->width - this->padding) && y <= (this->height - this->padding))
    {
#ifdef LGFXWIDGET_ALLOW_OVERFLOWS
        this->parentDisplayPtr->fillRect(this->xOffsetWithPadding + x, this->yOffsetWithPadding + y, width, height, color);
        return (true);
#else
#error TODO
#endif
    }
    else
    {
        return (false);
    }
}

bool LGFXWidget::drawRoundRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t radius, int color)
{
    if (x <= (this->width - this->padding) && y <= (this->height - this->padding))
    {
#ifdef LGFXWIDGET_ALLOW_OVERFLOWS
        this->parentDisplayPtr->drawRoundRect(this->xOffsetWithPadding + x, this->yOffsetWithPadding + y, width, height, radius, color);
        return (true);
#else
#error TODO
#endif
    }
    else
    {
        return (false);
    }
}

bool LGFXWidget::fillRoundRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t radius, int color)
{
    if (x <= (this->width - this->padding) && y <= (this->height - this->padding))
    {
#ifdef LGFXWIDGET_ALLOW_OVERFLOWS
        this->parentDisplayPtr->fillRoundRect(this->xOffsetWithPadding + x, this->yOffsetWithPadding + y, width, height, radius, color);
        return (true);
#else
#error TODO
#endif
    }
    else
    {
        return (false);
    }
}

bool LGFXWidget::drawFastHLine(uint16_t x, uint16_t y, uint16_t width, int color)
{
    if (x <= (this->width - this->padding) && y <= (this->height - this->padding))
    {
#ifdef LGFXWIDGET_ALLOW_OVERFLOWS
        this->parentDisplayPtr->drawFastHLine(this->xOffsetWithPadding + x, this->yOffsetWithPadding + y, width, color);
        return (true);
#else
#error TODO
#endif
    }
    else
    {
        return (false);
    }
}

bool LGFXWidget::drawFastVLine(uint16_t x, uint16_t y, uint16_t height, int color)
{
    if (x <= (this->width - this->padding) && y <= (this->height - this->padding))
    {
#ifdef LGFXWIDGET_ALLOW_OVERFLOWS
        this->parentDisplayPtr->drawFastVLine(this->xOffsetWithPadding + x, this->yOffsetWithPadding + y, height, color);
        return (true);
#else
#error TODO
#endif
    }
    else
    {
        return (false);
    }
}

bool LGFXWidget::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, int color)
{
    if (x0 <= (this->width - this->padding) && y0 <= (this->height - this->padding))
    {
#ifdef LGFXWIDGET_ALLOW_OVERFLOWS
        this->parentDisplayPtr->drawLine(this->xOffsetWithPadding + x0, this->yOffsetWithPadding + y0, this->xOffsetWithPadding + x1, this->yOffsetWithPadding + y1, color);
        return (true);
#else
#error TODO
#endif
    }
    else
    {
        return (false);
    }
}
