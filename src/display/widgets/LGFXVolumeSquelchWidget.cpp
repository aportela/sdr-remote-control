#include "LGFXVolumeSquelchWidget.hpp"
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

LGFXVolumeSquelchWidget::LGFXVolumeSquelchWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, Transceiver *transceiverPtr) : LGFXWidget(displayPtr, width, height, xOffset, yOffset, padding), transceiverPtr(transceiverPtr)
{
  if (displayPtr != nullptr)
  {
    this->refresh(true);
  }
}

LGFXVolumeSquelchWidget::~LGFXVolumeSquelchWidget()
{
}

void LGFXVolumeSquelchWidget::refreshVolume(bool force, uint8_t value)
{
  this->parentDisplayPtr->setTextSize(_VOLUME_SQUELCH_WIDGET_FONT_SIZE);
  this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND_COLOR);
#define _VOLUME_SQUELCH_WIDGET_LABELS_Y_OFFSET 2
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding, this->yOffset + this->padding);
  this->parentDisplayPtr->print("VOLUME");

  this->parentDisplayPtr->drawRect(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_VOLUME_BAR_X_OFFSET, this->yOffset + this->padding, _VOLUME_SQUELCH_WIDGET_BARS_WIDTH, _VOLUME_SQUELCH_WIDGET_BARS_HEIGHT, TEXT_COLOR_ACTIVE);
  this->parentDisplayPtr->fillRect(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_VOLUME_BAR_X_OFFSET + 1, this->yOffset + this->padding + 1, value, _VOLUME_SQUELCH_WIDGET_BARS_HEIGHT - 2, TEXT_COLOR_SECONDARY);

  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_LABEL_VOLUME_VALUE_X_OFFSET, this->yOffset + this->padding);
  this->parentDisplayPtr->printf("%03d", value);

  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_LABEL_SQUELCH_X_OFFSET, this->yOffset + this->padding);
  this->parentDisplayPtr->print("SQUELCH");

  this->parentDisplayPtr->drawRect(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_SQUELCH_BAR_X_OFFSET, this->yOffset + this->padding, _VOLUME_SQUELCH_WIDGET_BARS_WIDTH, _VOLUME_SQUELCH_WIDGET_BARS_HEIGHT, TEXT_COLOR_ACTIVE);
  this->parentDisplayPtr->fillRect(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_SQUELCH_BAR_X_OFFSET + 1, this->yOffset + this->padding + 1, value, _VOLUME_SQUELCH_WIDGET_BARS_HEIGHT - 2, TEXT_COLOR_SECONDARY);

  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_LABEL_SQUELCH_VALUE_X_OFFSET, this->yOffset + this->padding);
  this->parentDisplayPtr->printf("%03d", value);
}

void LGFXVolumeSquelchWidget::refreshSquelch(bool force, uint8_t value)
{
}

void LGFXVolumeSquelchWidget::setMuted(bool force, uint8_t value)
{
}

bool LGFXVolumeSquelchWidget::refresh(bool force)
{
  bool changed = force;
  if (changed || this->transceiverPtr->changed > 0)
  {
    if (force || (this->transceiverPtr->changed & TRX_CFLAG_AF_GAIN))
    {
      this->refreshVolume(force, this->transceiverPtr->AFGain);
      this->transceiverPtr->changed &= ~TRX_CFLAG_AF_GAIN;
      changed = true;
    }
  }
  return (changed);
}