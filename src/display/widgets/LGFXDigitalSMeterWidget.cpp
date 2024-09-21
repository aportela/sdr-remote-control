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
#define TEXT_BACKGROUND_COLOR TFT_BLACK

#define MIN_DB -54     // S0
#define MAX_DB 60      // S9+60
#define DB_BAR_STEPS 3 // 3 db between each bar

#define BAR_DISABLED_BACKGROUND_COLOR 0x8410

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

void LGFXDigitalSMeterWidget::drawBars(int8_t dB)
{
  for (int i = 0; i < _DIGITAL_SMETER_WIDGET_BAR_COUNT; i++)
  {
    int8_t currentdB = MIN_DB + (DB_BAR_STEPS * (i + 1)); // current bar dB
    uint16_t barColor = BAR_DISABLED_BACKGROUND_COLOR;
    if (dB >= currentdB)
    {
      barColor = i < 19 ? TFT_GREEN : TFT_RED;
    }
    uint16_t x = this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_BARS_X_OFFSET + (i * (_DIGITAL_SMETER_WIDGET_BAR_WIDTH + _DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN));
    bool isCurrentLongBar = (i == 1 || i == 3 || i == 5 || i == 7 || i == 9 || i == 11 || i == 13 || i == 15 || i == 17 || i == 22 || i == 27 || i == 37);
    this->parentDisplayPtr->fillRect(x, isCurrentLongBar ? _DIGITAL_SMETER_WIDGET_HIGH_BARS_Y_OFFSET : _DIGITAL_SMETER_WIDGET_LOW_BARS_Y_OFFSET, _DIGITAL_SMETER_WIDGET_BAR_WIDTH, isCurrentLongBar ? _DIGITAL_SMETER_WIDGET_HIGH_BAR_HEIGHT : _DIGITAL_SMETER_WIDGET_LOW_BAR_HEIGHT, barColor);
  }
}

void LGFXDigitalSMeterWidget::createSMeter(void)
{
  this->parentDisplayPtr->drawFastVLine(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_LEFT_VERTICAL_LINE_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_Y_OFFSET, _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_LENGTH, TEXT_COLOR_ACTIVE);
  this->parentDisplayPtr->drawFastVLine(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_RIGHT_VERTICAL_LINE_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_Y_OFFSET, _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_LENGTH, TEXT_COLOR_ACTIVE);
  this->parentDisplayPtr->drawFastHLine(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_HORIZONTAL_LINE_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_HORIZONTAL_LINE_Y_OFFSET, _DIGITAL_SMETER_WIDGET_HORIZONTAL_LINE_LENGTH, TEXT_COLOR_ACTIVE);
  this->parentDisplayPtr->setTextSize(_DIGITAL_SMETER_FONT_SIZE);
  this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND_COLOR);
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_LABELS_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_LABELS_Y_OFFSET);
  this->parentDisplayPtr->print("1  3  5  7  9 +15 +30     +60");
  this->drawBars(MIN_DB);
  this->parentDisplayPtr->setTextSize(_DIGITAL_SMETER_WIDGET_S_LABEL_FONT_SIZE);
  this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND_COLOR);
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_S_LABEL_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_S_LABEL_Y_OFFSET);
  this->parentDisplayPtr->print("S");
  this->parentDisplayPtr->print("9");
}

/*
  SMeter ranges
  S0     -54 dB
         -51 dB
  S1     –48 dB
         -45 dB
  S2     –42 dB
         -39 dB
  S3 	   –36 dB
         -33 dB
  S4 	   –30 dB
         -27 dB
  S5 	   –24 dB
         -21 dB
  S6 	   –18 dB
         -15 dB
  S7 	   –12 dB
          -9 dB
  S8 	    –6 dB
          -3 dB
  S9 	     0 dB
  S9+10   10 dB
  ...
  S9+30 	30 dB
  ...
  S9+60 	60 dB
*/
void LGFXDigitalSMeterWidget::refreshSMeter(int8_t dB)
{
  uint8_t activeBars = 0;
  if (dB < MIN_DB)
  {
    activeBars = 0;
  }
  else if (dB >= MAX_DB)
  {
    activeBars = 38;
  }
  else
  {
    activeBars = dB - MIN_DB;
  }
  this->drawBars(dB);
}

bool LGFXDigitalSMeterWidget::refresh(bool force)
{
  bool changed = force || (this->transceiverPtr->changed & TRX_CFLAG_SIGNAL_METER_DB_LEVEL);
  if (changed)
  {
    this->refreshSMeter(this->transceiverPtr->signalMeterdBLevel);
    this->transceiverPtr->changed &= ~TRX_CFLAG_SIGNAL_METER_DB_LEVEL;
  }
  return (changed);
}