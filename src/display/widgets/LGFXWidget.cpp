#include "LGFXWidget.hpp"
#include "../../DisplayConfiguration.hpp"

LGFXWidget::LGFXWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset) : Widget(width, height, xOffset, yOffset), parentDisplayPtr(displayPtr)
{
#ifdef DEBUG_SCREEN_WIDGETS_BOUNDS
    this->parentDisplayPtr->drawRect(xOffset, yOffset, width, height, TFT_WHITE);
#endif
}

LGFXWidget::~LGFXWidget()
{
}
