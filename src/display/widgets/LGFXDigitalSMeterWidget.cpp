#include <cstdbool>
#include "LGFXDigitalSMeterWidget.hpp"

#define MIN_DB -54     // S0
#define MAX_DB 60      // S9+60
#define DB_BAR_STEPS 3 // 3 db between each bar

#define BAR_DISABLED_BACKGROUND_COLOR 0x8410

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

LGFXDigitalSMeterWidget::LGFXDigitalSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr) : LGFXTransceiverStatusWidget(displayPtr, width, height, xOffset, yOffset, padding, currentTransceiverStatusPtr)
{
  this->smeter = new SMeter(this->currentTransceiverStatusPtr->signalMeterdBLevel);
  if (displayPtr != nullptr)
  {
    this->expSprite = new lgfx::LGFX_Sprite(displayPtr);
    this->expSprite->setColorDepth(8);
    this->expSprite->createSprite(60, 28);
    if (currentTransceiverStatusPtr != nullptr)
    {
      this->refresh(true);
    }
  }
}

LGFXDigitalSMeterWidget::~LGFXDigitalSMeterWidget()
{
  delete this->smeter;
  this->smeter = nullptr;
  delete this->expSprite;
  this->expSprite = nullptr;
}

// TODO: PEAKS / OPTIMIZE
void LGFXDigitalSMeterWidget::drawBars(int8_t dB)
{
  for (int i = 0; i < _DIGITAL_SMETER_WIDGET_BAR_COUNT; i++)
  {
    int8_t currentdB = MIN_DB + (DB_BAR_STEPS * (i + 1)); // current bar dB
    uint16_t barColor = BAR_DISABLED_BACKGROUND_COLOR;
    if (dB >= currentdB)
    {
      barColor = i < 18 ? TFT_GREEN : TFT_RED;
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
  this->parentDisplayPtr->setTextSize(_DIGITAL_SMETER_WIDGET_FONT_SIZE);
  this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND_COLOR);
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_LABELS_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_TOP_LABELS_Y_OFFSET);
  this->parentDisplayPtr->print("1  3  5  7  9  +15 +30    +60");
  this->drawBars(MIN_DB);
}

void LGFXDigitalSMeterWidget::refreshSMeterDBLabel(bool force, int8_t dB)
{
  this->parentDisplayPtr->setTextSize(_DIGITAL_SMETER_WIDGET_S_LABEL_FONT_SIZE);
  this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND_COLOR);
  if (force)
  {
    this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_S_LABEL_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_S_LABEL_Y_OFFSET);
    this->parentDisplayPtr->print("S");
  }
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_S_LABEL_BASE_NUMBER_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_S_LABEL_Y_OFFSET);
  bool showDBExp = dB > 0;
  if (dB < -48)
  {
    this->parentDisplayPtr->print("0");
  }
  else if (dB < -42)
  {
    this->parentDisplayPtr->print("1");
  }
  else if (dB < -36)
  {
    this->parentDisplayPtr->print("2");
  }
  else if (dB < -30)
  {
    this->parentDisplayPtr->print("3");
  }
  else if (dB < -24)
  {
    this->parentDisplayPtr->print("4");
  }
  else if (dB < -18)
  {
    this->parentDisplayPtr->print("5");
  }
  else if (dB < -12)
  {
    this->parentDisplayPtr->print("6");
  }
  else if (dB < -6)
  {
    this->parentDisplayPtr->print("7");
  }
  else if (dB < 0)
  {
    this->parentDisplayPtr->print("8");
  }
  else
  {
    this->parentDisplayPtr->print("9");
  }
  if (!showDBExp)
  {
    // only refresh base dB label if previous value has exp label
    if (force || this->previousDBValue >= 0)
    {
      this->parentDisplayPtr->print("dB ");
    }
  }
  else
  {
    this->expSprite->fillScreen(TFT_BLACK);
    this->expSprite->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND_COLOR);
    this->expSprite->setTextSize(_DIGITAL_SMETER_WIDGET_S_SUB_LABEL_FONT_SIZE);
    this->expSprite->setCursor(0, 0);
    this->expSprite->printf("+%ddB", dB);
    this->expSprite->pushSprite(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_S_LABEL_EXP_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_S_LABEL_Y_OFFSET);
  }
}

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
  bool changed = force;
  if (force)
  {
    this->createSMeter();
  }
  this->smeter->set(this->currentTransceiverStatusPtr->signalMeterdBLevel);
  int8_t dBSmooth = this->smeter->get(true);
  if (force || this->previousDBValue != dBSmooth)
  {
    this->refreshSMeter(dBSmooth);
    this->refreshSMeterDBLabel(force, this->currentTransceiverStatusPtr->signalMeterdBLevel); // "digital" label always show real (not "smooth")
    this->previousDBValue = dBSmooth;
    changed = true;
  }
  return (changed);
}