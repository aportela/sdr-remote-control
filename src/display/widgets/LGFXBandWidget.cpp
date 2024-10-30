#include "LGFXBandWidget.hpp"

#define _BAND_WIDGET_BACKGROUND_COLOR 0x19cc
#define _BAND_WIDGET_BORDER_COLOR TEXT_COLOR_ACTIVE

LGFXBandWidget::LGFXBandWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr) : LGFXTransceiverStatusWidget(displayPtr, width, height, xOffset, yOffset, padding, currentTransceiverStatusPtr)
{
  if (displayPtr != nullptr)
  {
    this->buttonBackgroundSprite = new lgfx::LGFX_Sprite(displayPtr);
    this->buttonBackgroundSprite->setColorDepth(16);
    this->buttonBackgroundSprite->createSprite(this->width, this->height);
    this->buttonBackgroundSprite->fillScreen(TFT_BLACK);

    this->buttonBackgroundSprite->fillRoundRect(0, 0, this->width, this->height, _BAND_WIDGET_BACKGROUND_RECT_RADIUS, _BAND_WIDGET_BACKGROUND_COLOR);
    this->buttonBackgroundSprite->drawRoundRect(0, 0, this->width, this->height, _BAND_WIDGET_BACKGROUND_RECT_RADIUS, _BAND_WIDGET_BORDER_COLOR);
    if (currentTransceiverStatusPtr != nullptr)
    {
      this->refresh(true);
    }
  }
}

LGFXBandWidget::~LGFXBandWidget()
{
  delete this->buttonBackgroundSprite;
  this->buttonBackgroundSprite = nullptr;
}

void LGFXBandWidget::createBackgroundButton(void)
{
}
void LGFXBandWidget::refreshLabels(uint16_t currentBandIndex)
{
  this->buttonBackgroundSprite->pushSprite(this->xOffset + this->padding, this->yOffset + this->padding);

  this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE, _BAND_WIDGET_BACKGROUND_COLOR);
  this->parentDisplayPtr->setTextSize(_BAND_WIDGET_MAIN_LABEL_FONT_SIZE);

  uint16_t centeredTextXOffset = (this->width - (this->padding * 2) - this->parentDisplayPtr->textWidth(RadioBands[currentBandIndex].label)) / 2;
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + centeredTextXOffset, this->yOffset + this->padding + _BAND_WIDGET_MAIN_LABEL_Y_OFFSET);
  this->parentDisplayPtr->print(RadioBands[currentBandIndex].label);
  this->parentDisplayPtr->setTextSize(_BAND_WIDGET_SUB_LABEL_FONT_SIZE);
  switch (RadioBands[currentBandIndex].type)
  {
  case RBT_AMATEUR:
    centeredTextXOffset = (this->width - (this->padding * 2) - this->parentDisplayPtr->textWidth("Amateur")) / 2;
    this->parentDisplayPtr->setCursor(this->xOffset + this->padding + centeredTextXOffset, this->yOffset + this->padding + _BAND_WIDGET_SUB_LABEL_Y_OFFSET);
    this->parentDisplayPtr->print("Amateur");
    break;
  case RBT_BROADCAST:
    centeredTextXOffset = (this->width - (this->padding * 2) - this->parentDisplayPtr->textWidth("Broadcast")) / 2;
    this->parentDisplayPtr->setCursor(this->xOffset + this->padding + centeredTextXOffset, this->yOffset + this->padding + _BAND_WIDGET_SUB_LABEL_Y_OFFSET);
    this->parentDisplayPtr->print("Broadcast");
    break;
  default:
    centeredTextXOffset = (this->width - (this->padding * 2) - this->parentDisplayPtr->textWidth("Other")) / 2;
    this->parentDisplayPtr->setCursor(this->xOffset + this->padding + centeredTextXOffset, this->yOffset + this->padding + _BAND_WIDGET_SUB_LABEL_Y_OFFSET);
    this->parentDisplayPtr->print("Other");
    break;
  }
}

bool LGFXBandWidget::refresh(bool force)
{
  bool changed = force;
  if (force || this->oldBandIndex != this->currentTransceiverStatusPtr->VFO[this->currentTransceiverStatusPtr->activeVFOIndex].currentBandIndex)
  {
    this->refreshLabels(this->currentTransceiverStatusPtr->VFO[this->currentTransceiverStatusPtr->activeVFOIndex].currentBandIndex);
    this->oldBandIndex = this->currentTransceiverStatusPtr->VFO[this->currentTransceiverStatusPtr->activeVFOIndex].currentBandIndex;
    changed = true;
  }
  return (changed);
}