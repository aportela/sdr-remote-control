#include "LGFXDigitalSMeterWidget.hpp"

#define DECIBELS_BETWEEN_EACH_BAR 3 // 3 db between each bar

#define BAR_NO_VALUE_BACKGROUND_COLOR 0x8410
#define BAR_LOW_VALUES_BACKGROUND_COLOR TFT_GREEN
#define BAR_HIGH_VALUES_BACKGROUND_COLOR TFT_RED

// #include <Fonts/Custom/Roboto_Thin_24.h>

LGFXDigitalSMeterWidget::LGFXDigitalSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr) : LGFXSMeterWidget(displayPtr, width, height, xOffset, yOffset, padding, currentTransceiverStatusPtr)
{
  if (displayPtr != nullptr && currentTransceiverStatusPtr != nullptr)
  {
    this->refresh(true);
  }
}

LGFXDigitalSMeterWidget::~LGFXDigitalSMeterWidget()
{
}

// TODO: PEAKS / OPTIMIZE
void LGFXDigitalSMeterWidget::drawBars(int8_t dB)
{
  for (int i = 0; i < _DIGITAL_SMETER_WIDGET_BAR_COUNT; i++)
  {
    int8_t currentdB = SMETER_MIN_DB + (DECIBELS_BETWEEN_EACH_BAR * (i + 1)); // current bar dB
    uint16_t barColor = BAR_NO_VALUE_BACKGROUND_COLOR;
    if (dB >= currentdB)
    {
      barColor = i < 18 ? BAR_LOW_VALUES_BACKGROUND_COLOR : BAR_HIGH_VALUES_BACKGROUND_COLOR;
    }
    uint16_t x = this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_BARS_X_OFFSET + (i * (_DIGITAL_SMETER_BAR_WITH_MARGIN_WIDTH));
    bool isCurrentLongBar = (i == 1 || i == 3 || i == 5 || i == 7 || i == 9 || i == 11 || i == 13 || i == 15 || i == 17 || i == 22 || i == 27 || i == 32 || i == 37);
    this->parentDisplayPtr->fillRect(x, this->yOffset + this->height - 2 - (isCurrentLongBar ? _DIGITAL_SMETER_WIDGET_HIGH_BAR_HEIGHT : _DIGITAL_SMETER_WIDGET_LOW_BAR_HEIGHT), _DIGITAL_SMETER_WIDGET_BAR_WIDTH, isCurrentLongBar ? _DIGITAL_SMETER_WIDGET_HIGH_BAR_HEIGHT : _DIGITAL_SMETER_WIDGET_LOW_BAR_HEIGHT, barColor);
  }
}

void LGFXDigitalSMeterWidget::init(void)
{
  this->parentDisplayPtr->drawFastVLine(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_LEFT_VERTICAL_LINE_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_Y_OFFSET, _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_LENGTH, TEXT_COLOR_ACTIVE);
  this->parentDisplayPtr->drawFastVLine(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_RIGHT_VERTICAL_LINE_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_Y_OFFSET, _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_LENGTH, TEXT_COLOR_ACTIVE);
  this->parentDisplayPtr->drawFastHLine(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_HORIZONTAL_LINE_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_HORIZONTAL_LINE_Y_OFFSET, _DIGITAL_SMETER_WIDGET_HORIZONTAL_LINE_LENGTH, TEXT_COLOR_ACTIVE);
  this->parentDisplayPtr->setTextSize(_DIGITAL_SMETER_WIDGET_SUNITS_RANGE_FONT_SIZE);
  this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND_COLOR);
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S1_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET);
  this->parentDisplayPtr->print("S1");
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S3_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET);
  this->parentDisplayPtr->print("3");
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S5_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET);
  this->parentDisplayPtr->print("5");
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S7_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET);
  this->parentDisplayPtr->print("7");
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S9_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET);
  this->parentDisplayPtr->print("9");
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S9_PLUS15_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET);
  this->parentDisplayPtr->print("+15");
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S9_PLUS30_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET);
  this->parentDisplayPtr->print("+30");
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S9_PLUS60_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET);
  this->parentDisplayPtr->print("+60dB");
  this->drawBars(SMETER_MIN_DB);
}

void LGFXDigitalSMeterWidget::update(int8_t dB)
{
  this->drawBars(dB);
}
