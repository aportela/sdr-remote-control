#include "LGFXFilterWidget.hpp"
#include "../../DisplayConfiguration.hpp"

#ifdef DISPLAY_LOVYANN_ILI9488_480x320

#include "../ILI9488/ScreenConnectedDefines.hpp"

#elif defined(DISPLAY_LOVYANN_ST7789_240x320)

#include "../ST7789/ScreenConnectedDefines.hpp"

#else

#error NO DISPLAY DEFINED

#endif // DISPLAY_LOVYANN_ILI9488_480x320

#define TEXT_COLOR_ACTIVE TFT_WHITE
#define TEXT_COLOR_SECONDARY 0x528A
#define TEXT_COLOR_NOT_ACTIVE 0x18C3
#define TEXT_BACKGROUND_COLOR TFT_BLACK

LGFXFilterWidget::LGFXFilterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, Transceiver *transceiverPtr) : LGFXWidget(displayPtr, width, height, xOffset, yOffset, padding), transceiverPtr(transceiverPtr)
{
  if (displayPtr != nullptr)
  {
    this->refresh(true);
  }
}

LGFXFilterWidget::~LGFXFilterWidget()
{
}

void LGFXFilterWidget::refreshPlot(void)
{
  this->parentDisplayPtr->drawFastHLine(this->xOffset + this->padding, this->yOffset + this->padding + 50, 240, TEXT_COLOR_ACTIVE);
  this->parentDisplayPtr->drawFastHLine(this->xOffset + this->padding + 60, this->yOffset + this->padding + 8, 120, TEXT_COLOR_ACTIVE);
  this->parentDisplayPtr->drawFastVLine(this->xOffset + this->padding + 120, this->yOffset + this->padding + 8, 42, TEXT_COLOR_ACTIVE);
}

bool LGFXFilterWidget::refresh(bool force)
{
  bool changed = force;
  if (changed || this->transceiverPtr->changed > 0)
  {
    if (force || (this->transceiverPtr->changed & TRX_CFLAG_ACTIVE_VFO_FILTER_LOW) || (this->transceiverPtr->changed & TRX_CFLAG_ACTIVE_VFO_FILTER_HIGH))
    {
      this->refreshPlot();
      this->transceiverPtr->changed &= ~TRX_CFLAG_ACTIVE_VFO_FILTER_LOW;
      this->transceiverPtr->changed &= ~TRX_CFLAG_ACTIVE_VFO_FILTER_HIGH;
      changed = true;
    }
  }
  return (changed);
}