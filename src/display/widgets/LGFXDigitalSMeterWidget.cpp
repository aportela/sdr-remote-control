#include <cstdbool>
#include "LGFXDigitalSMeterWidget.hpp"
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
#define TEXT_BACKGROUND TFT_BLACK

LGFXDigitalSMeterWidget::LGFXDigitalSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, Transceiver *transceiverPtr) : LGFXWidget(displayPtr, width, height, xOffset, yOffset, padding), transceiverPtr(transceiverPtr)
{
  if (displayPtr != nullptr)
  {
    this->createSMeter();
  }
}

LGFXDigitalSMeterWidget::~LGFXDigitalSMeterWidget()
{
}

void LGFXDigitalSMeterWidget::createSMeter(void)
{
  this->parentDisplayPtr->drawFastVLine(this->xOffset + this->padding + _DIGITAL_SEMETER_WIDGET_LEFT_VERTICAL_LINE_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SEMETER_WIDGET_VERTICAL_LINES_Y_OFFSET, _DIGITAL_SEMETER_WIDGET_VERTICAL_LINES_LENGTH, TFT_RED);
  this->parentDisplayPtr->drawFastVLine(this->xOffset + this->padding + _DIGITAL_SEMETER_WIDGET_RIGHT_VERTICAL_LINE_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SEMETER_WIDGET_VERTICAL_LINES_Y_OFFSET, _DIGITAL_SEMETER_WIDGET_VERTICAL_LINES_LENGTH, TFT_RED);
  // this->parentDisplayPtr->drawFastHLine(this->xOffset + this->padding + _DIGITAL_SEMETER_WIDGET_HORIZONTAL_LINE_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SEMETER_WIDGET_HORIZONTAL_LINE_Y_OFFSET, _DIGITAL_SEMETER_WIDGET_HORIZONTAL_LINE_LENGTH, TFT_RED);

  this->parentDisplayPtr->setTextSize(_DIGITAL_SMETER_FONT_SIZE);
  this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND);

  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_LABELS_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_LABELS_Y_OFFSET);
  this->parentDisplayPtr->print("1  3  5  7  9 +15 +30    +60");
  //  this->parentDisplayPtr->setCursor(_DISPLAY_PADDING, 98);
  //  this->parentDisplayPtr->print("S");
  // this->parentDisplayPtr->setCursor(370, 102);
  // this->parentDisplayPtr->print("S9+60dB");

  for (int i = 1; i <= _DIGITAL_SMETER_WIDGET_BAR_COUNT; i++)
  {
    uint16_t x = this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_BARS_X_OFFSET + ((i - 1) * (SMETER_BAR_WIDTH + _DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN));
    if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 23 || i == 28 || i == 38)
    {
      this->parentDisplayPtr->fillRect(x, SMETER_HIGH_BARS_Y_OFFSET, SMETER_BAR_WIDTH, SMETER_HIGH_BAR_HEIGHT, SMETER_BAR_BG_COLOR);
    }
    else
    {
      this->parentDisplayPtr->fillRect(x, SMETER_LOW_BARS_Y_OFFSET, SMETER_BAR_WIDTH, SMETER_LOW_BAR_HEIGHT, SMETER_BAR_BG_COLOR);
    }
  }
}

void LGFXDigitalSMeterWidget::refreshSMeter(uint8_t level)
{
}

bool LGFXDigitalSMeterWidget::refresh(bool force)
{
  bool changed = force || this->transceiverPtr->changed > 0;
  if (changed)
  {
    if (force || (this->transceiverPtr->changed & TRX_CFLAG_SIGNAL_METER_DB_LEVEL))
    {
      this->refreshSMeter(this->transceiverPtr->signalMeterdBLevel);
      this->transceiverPtr->changed &= ~TRX_CFLAG_SIGNAL_METER_DB_LEVEL;
    }
  }
  return (changed);
}