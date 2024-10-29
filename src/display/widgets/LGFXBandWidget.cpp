#include "LGFXBandWidget.hpp"

#define TEXT_COLOR_MUTED TFT_RED

LGFXBandWidget::LGFXBandWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr) : LGFXTransceiverStatusWidget(displayPtr, width, height, xOffset, yOffset, padding, currentTransceiverStatusPtr)
{
  if (displayPtr != nullptr)
  {
    if (currentTransceiverStatusPtr != nullptr)
    {
      this->refresh(true);
    }
  }
}

LGFXBandWidget::~LGFXBandWidget()
{
}

void LGFXBandWidget::refreshLabels(bool force, uint16_t currentBandIndex)
{
  if (force)
  {
    this->parentDisplayPtr->fillRoundRect(this->xOffset + this->padding, this->yOffset + this->padding, this->width, this->height, _BAND_WIDGET_BACKGROUND_RECT_RADIUS, TEXT_COLOR_NOT_ACTIVE);
    this->parentDisplayPtr->drawRoundRect(this->xOffset + this->padding, this->yOffset + this->padding, this->width, this->height, _BAND_WIDGET_BACKGROUND_RECT_RADIUS, TEXT_COLOR_ACTIVE);
  }

  this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE, TEXT_COLOR_NOT_ACTIVE);
  this->parentDisplayPtr->setTextSize(_BAND_WIDGET_MAIN_LABEL_FONT_SIZE);
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + 20, this->yOffset + this->padding + 4);
  this->parentDisplayPtr->print(RadioBands[this->oldBandIndex].label);
  this->parentDisplayPtr->setTextSize(_BAND_WIDGET_SUB_LABEL_FONT_SIZE);
  switch (RadioBands[this->oldBandIndex].type)
  {
  case RBT_AMATEUR:
    this->parentDisplayPtr->setCursor(this->xOffset + this->padding + 16, this->yOffset + this->padding + 30);
    this->parentDisplayPtr->print("Amateur");
    break;
  case RBT_BROADCAST:
    this->parentDisplayPtr->setCursor(this->xOffset + this->padding + 12, this->yOffset + this->padding + 30);
    this->parentDisplayPtr->print("Broadcast");
    break;
  default:
    this->parentDisplayPtr->setCursor(this->xOffset + this->padding + 24, this->yOffset + this->padding + 30);
    this->parentDisplayPtr->print("Other");
    break;
  }
}

bool LGFXBandWidget::refresh(bool force)
{
  bool changed = force;
  if (force || this->oldBandIndex != this->currentTransceiverStatusPtr->VFO[this->currentTransceiverStatusPtr->activeVFOIndex].currentBandIndex)
  {
    this->refreshLabels(force, this->currentTransceiverStatusPtr->VFO[this->currentTransceiverStatusPtr->activeVFOIndex].currentBandIndex);
    this->oldBandIndex = this->currentTransceiverStatusPtr->VFO[this->currentTransceiverStatusPtr->activeVFOIndex].currentBandIndex;
    changed = true;
  }
  return (changed);
}