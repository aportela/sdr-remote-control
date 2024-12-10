#include "LGFXWidget.hpp"

LGFXWidget::LGFXWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding) : Widget(width, height, xOffset, yOffset, padding), parentDisplayPtr(displayPtr)
{
    if (displayPtr != nullptr)
    {
        this->previousFont = this->parentDisplayPtr->getFont();
#ifdef DEBUG_SCREEN_WIDGETS_BOUNDS
        this->parentDisplayPtr->drawRect(xOffset, yOffset, width, height, TFT_WHITE);
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