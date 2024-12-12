#include "LGFXDigitalSMeterWidget.hpp"

#define DIGITAL_SMETER_BAR_NO_VALUE_TEXT_COLOR TEXT_COLOR_NOT_ACTIVE
#define DIGITAL_SMETER_BAR_LOW_VALUE_TEXT_COLOR COLOR_GREEN
#define DIGITAL_SMETER_BAR_HIGH_VALUE_TEXT_COLOR COLOR_RED
#define DIGITAL_SMETER_CONTAINER_LINES_TEXT_COLOR TEXT_COLOR_ACTIVE
#define DIGITAL_SMETER_CONTAINER_FONTS_TEXT_COLOR TEXT_COLOR_ACTIVE
#define DIGITAL_SMETER_CONTAINER_FONTS_BACKGROUND_COLOR TEXT_BACKGROUND_COLOR

#define DECIBELS_BETWEEN_EACH_BAR 3 // 3 db between each bar

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
    uint16_t barColor = DIGITAL_SMETER_BAR_NO_VALUE_TEXT_COLOR;               // gray ("power off") color by default
    if (dB >= currentdB)
    {
      barColor = i < 18 ? DIGITAL_SMETER_BAR_LOW_VALUE_TEXT_COLOR : DIGITAL_SMETER_BAR_HIGH_VALUE_TEXT_COLOR; // values <= S9 have green color, values > S9  red
    }
    uint16_t x = _DIGITAL_SMETER_WIDGET_BARS_X_OFFSET + (i * (_DIGITAL_SMETER_BAR_WITH_MARGIN_WIDTH));
    bool isCurrentLongBar = (i == 1 || i == 3 || i == 5 || i == 7 || i == 9 || i == 11 || i == 13 || i == 15 || i == 17 || i == 22 || i == 27 || i == 32 || i == 37);
    this->fillRect(
        x,                                                                                                       // x
        (isCurrentLongBar ? _DIGITAL_SMETER_WIDGET_HIGH_BAR_Y_OFFSET : _DIGITAL_SMETER_WIDGET_LOW_BAR_Y_OFFSET), // y
        _DIGITAL_SMETER_WIDGET_BAR_WIDTH,                                                                        // width
        isCurrentLongBar ? _DIGITAL_SMETER_WIDGET_HIGH_BAR_HEIGHT : _DIGITAL_SMETER_WIDGET_LOW_BAR_HEIGHT,       // height
        barColor                                                                                                 // color
    );

    uint16_t x2 = _SMETER_WIDGET_SIGNAL_BASE_LABEL_X_OFFSET + (i * (3));
    this->fillRect(
        x2,      // x
        6,       // y
        2,       // width
        10,      // height
        barColor // color
    );
  }
}

void LGFXDigitalSMeterWidget::init(void)
{
  // left vertical container line
  this->drawFastVLine(_DIGITAL_SMETER_WIDGET_LEFT_VERTICAL_LINE_X_OFFSET, _DIGITAL_SMETER_WIDGET_HIGH_BAR_Y_OFFSET, _DIGITAL_SMETER_WIDGET_VERTICAL_LINE_HEIGHT, DIGITAL_SMETER_CONTAINER_LINES_TEXT_COLOR);
  // bottom horizontal container line
  this->drawFastHLine(_DIGITAL_SMETER_WIDGET_HORIZONTAL_LINE_X_OFFSET, _DIGITAL_SMETER_WIDGET_HORIZONTAL_LINE_Y_OFFSET, _DIGITAL_SMETER_WIDGET_HORIZONTAL_LINE_WIDTH, DIGITAL_SMETER_CONTAINER_LINES_TEXT_COLOR);
  // right vertical container line
  this->drawFastVLine(_DIGITAL_SMETER_WIDGET_RIGHT_VERTICAL_LINE_X_OFFSET, _DIGITAL_SMETER_WIDGET_HIGH_BAR_Y_OFFSET, _DIGITAL_SMETER_WIDGET_VERTICAL_LINE_HEIGHT, DIGITAL_SMETER_CONTAINER_LINES_TEXT_COLOR);

  // "S 1 3 4 7 9  +15  +30    +60dB" container labels
  this->parentDisplayPtr->setFont(_DIGITAL_SMETER_WIDGET_SUNITS_RANGE_FONT_FAMILY);
  this->parentDisplayPtr->setTextSize(_DIGITAL_SMETER_WIDGET_SUNITS_RANGE_FONT_SIZE);
  this->parentDisplayPtr->setTextColor(DIGITAL_SMETER_CONTAINER_FONTS_TEXT_COLOR, DIGITAL_SMETER_CONTAINER_FONTS_BACKGROUND_COLOR);
  this->printAtCursor(_DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S_X_OFFSET, _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "S");
  this->printAtCursor(_DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S1_X_OFFSET, _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "1");
  this->printAtCursor(_DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S3_X_OFFSET, _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "3");
  this->printAtCursor(_DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S5_X_OFFSET, _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "5");
  this->printAtCursor(_DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S7_X_OFFSET, _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "7");
  this->printAtCursor(_DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S9_X_OFFSET, _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "9");
  this->printAtCursor(_DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S9_PLUS15_X_OFFSET, _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "+15");
  this->printAtCursor(_DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S9_PLUS30_X_OFFSET, _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "+30");
  this->printAtCursor(_DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S9_PLUS60_X_OFFSET, _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "+60dB");
  this->restorePreviousFont();
  this->drawBars(SMETER_MIN_DB);
}

void LGFXDigitalSMeterWidget::update(int8_t dB)
{
  this->drawBars(dB);
}
