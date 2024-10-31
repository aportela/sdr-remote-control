#include "LGFXBandWidget.hpp"
#include <cmath>
#include <string>

#define SPEED_OF_LIGHT 3e8

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

void LGFXBandWidget::getHumanWaveLength(uint64_t frequency, char *label, size_t count)
{
  double waveLength = SPEED_OF_LIGHT / frequency;
  double waveLengthRemainder = std::fmod(SPEED_OF_LIGHT, frequency);
  if (waveLength >= 1.0)
  {
    if (waveLengthRemainder > 0)
    {
      snprintf(label, count, "%.2fm", waveLength);
    }
    else
    {
      snprintf(label, count, "%um", waveLength);
    }
  }
  else
  {
    if (waveLengthRemainder > 0)
    {
      snprintf(label, count, "%.2fcm", waveLength * 100);
    }
    else
    {
      snprintf(label, count, "%ucm", waveLength * 100);
    }
  }
}

void LGFXBandWidget::createBackgroundButton(void)
{
}

void LGFXBandWidget::refreshLabels(const char *mainLabel, const char *subLabel)
{
  this->buttonBackgroundSprite->pushSprite(this->xOffset + this->padding, this->yOffset + this->padding);
  this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE, _BAND_WIDGET_BACKGROUND_COLOR);

  this->parentDisplayPtr->setTextSize(_BAND_WIDGET_MAIN_LABEL_FONT_SIZE);
  uint16_t textWidth = this->parentDisplayPtr->textWidth(mainLabel);
  if (textWidth > (this->width - (this->padding * 2) - 8))
  {
    this->parentDisplayPtr->setTextSize(_BAND_WIDGET_MAIN_LABEL_FONT_SIZE - 1);
    textWidth = this->parentDisplayPtr->textWidth(mainLabel);
  }
  uint16_t centeredTextXOffset = (this->width - (this->padding * 2) - textWidth) / 2;
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + centeredTextXOffset, this->yOffset + this->padding + _BAND_WIDGET_MAIN_LABEL_Y_OFFSET);
  this->parentDisplayPtr->print(mainLabel);

  this->parentDisplayPtr->setTextSize(_BAND_WIDGET_SUB_LABEL_FONT_SIZE);
  textWidth = this->parentDisplayPtr->textWidth(subLabel);
  centeredTextXOffset = (this->width - (this->padding * 2) - textWidth) / 2;
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + centeredTextXOffset, this->yOffset + this->padding + _BAND_WIDGET_SUB_LABEL_Y_OFFSET);
  this->parentDisplayPtr->print(subLabel);
}

bool LGFXBandWidget::refresh(bool force)
{
  bool changed = force;
  bool bandIndexChanged = this->oldBandIndex != this->currentTransceiverStatusPtr->VFO[this->currentTransceiverStatusPtr->activeVFOIndex].currentBandIndex;
  bool frequencyChanged = this->currentTransceiverStatusPtr->VFO[this->currentTransceiverStatusPtr->activeVFOIndex].frequency != this->oldFrequency;
  if (force || bandIndexChanged || (this->currentTransceiverStatusPtr->VFO[this->currentTransceiverStatusPtr->activeVFOIndex].currentBandIndex == 0 && frequencyChanged))
  {
    char label[16] = {'\0'};
    if (this->currentTransceiverStatusPtr->VFO[this->currentTransceiverStatusPtr->activeVFOIndex].currentBandIndex > 0)
    {
      snprintf(label, sizeof(label), "%s", RadioBands[this->currentTransceiverStatusPtr->VFO[this->currentTransceiverStatusPtr->activeVFOIndex].currentBandIndex].label);
    }
    else
    {
      this->getHumanWaveLength(this->currentTransceiverStatusPtr->VFO[this->currentTransceiverStatusPtr->activeVFOIndex].frequency, label, sizeof(label));
    }
    if (strcmp(label, this->oldBandLabelStr) != 0)
    {
      char subLabel[16] = {'\0'};
      switch (RadioBands[this->currentTransceiverStatusPtr->VFO[this->currentTransceiverStatusPtr->activeVFOIndex].currentBandIndex].type)
      {
      case RBT_AMATEUR:
        snprintf(subLabel, sizeof(subLabel), "%s", "Amateur");
        break;
      case RBT_BROADCAST:
        snprintf(subLabel, sizeof(subLabel), "%s", "Broadcast");
        break;
      default:
        snprintf(subLabel, sizeof(subLabel), "%s", "Other");
        break;
      }
      this->refreshLabels(label, subLabel);
    }
    strncpy(this->oldBandLabelStr, label, sizeof(label));
    this->oldBandLabelStr[sizeof(label)] = '\0';
    changed = true;
  }
  this->oldFrequency = this->currentTransceiverStatusPtr->VFO[this->currentTransceiverStatusPtr->activeVFOIndex].frequency;
  this->oldBandIndex = this->currentTransceiverStatusPtr->VFO[this->currentTransceiverStatusPtr->activeVFOIndex].currentBandIndex;
  return (changed);
}