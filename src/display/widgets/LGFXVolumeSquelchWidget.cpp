#include "LGFXVolumeSquelchWidget.hpp"

#define TEXT_COLOR_MUTED TFT_RED

LGFXVolumeSquelchWidget::LGFXVolumeSquelchWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr) : LGFXTransceiverStatusWidget(displayPtr, width, height, xOffset, yOffset, padding, currentTransceiverStatusPtr)
{
  if (displayPtr != nullptr)
  {
    if (currentTransceiverStatusPtr != nullptr)
    {
      this->refresh(true);
    }
  }
}

LGFXVolumeSquelchWidget::~LGFXVolumeSquelchWidget()
{
}

void LGFXVolumeSquelchWidget::refreshVolume(bool force, bool active, uint8_t value, bool muted)
{
  this->parentDisplayPtr->setTextSize(_VOLUME_SQUELCH_WIDGET_FONT_SIZE);
  this->parentDisplayPtr->setTextColor(active ? TEXT_COLOR_ACTIVE : TEXT_COLOR_NOT_ACTIVE, TEXT_BACKGROUND_COLOR);
  if (force)
  {
    this->parentDisplayPtr->setCursor(this->xOffset + this->padding, this->yOffset + this->padding);
    this->parentDisplayPtr->print("VOLUME");

    this->parentDisplayPtr->drawRect(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_VOLUME_BAR_X_OFFSET, this->yOffset + this->padding, _VOLUME_SQUELCH_WIDGET_BARS_WIDTH, _VOLUME_SQUELCH_WIDGET_BARS_HEIGHT, active ? TEXT_COLOR_ACTIVE : TEXT_COLOR_NOT_ACTIVE);
  }
  if (value > 0)
  {
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_VOLUME_BAR_X_OFFSET + 1, this->yOffset + this->padding + 1, value, _VOLUME_SQUELCH_WIDGET_BARS_HEIGHT - 2, TEXT_COLOR_SECONDARY);
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_VOLUME_BAR_X_OFFSET + 1 + value, this->yOffset + this->padding + 1, 100 - value, _VOLUME_SQUELCH_WIDGET_BARS_HEIGHT - 2, TEXT_BACKGROUND_COLOR);
  }
  else
  {
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_VOLUME_BAR_X_OFFSET + 1, this->yOffset + this->padding + 1, 100, _VOLUME_SQUELCH_WIDGET_BARS_HEIGHT - 2, TEXT_BACKGROUND_COLOR);
  }
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_LABEL_VOLUME_VALUE_X_OFFSET, this->yOffset + this->padding);
  if (muted)
  {
    this->parentDisplayPtr->setTextColor(TEXT_COLOR_MUTED, TEXT_BACKGROUND_COLOR);
  }
  else
  {
    this->parentDisplayPtr->setTextColor(active ? TEXT_COLOR_ACTIVE : TEXT_COLOR_NOT_ACTIVE, TEXT_BACKGROUND_COLOR);
  }
  this->parentDisplayPtr->printf("%03d", value);
}

void LGFXVolumeSquelchWidget::refreshSquelch(bool force, bool active, uint8_t value)
{
  this->parentDisplayPtr->setTextSize(_VOLUME_SQUELCH_WIDGET_FONT_SIZE);
  this->parentDisplayPtr->setTextColor(active ? TEXT_COLOR_ACTIVE : TEXT_COLOR_NOT_ACTIVE, TEXT_BACKGROUND_COLOR);
  if (force)
  {
    this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_LABEL_SQUELCH_X_OFFSET, this->yOffset + this->padding);
    this->parentDisplayPtr->print("SQUELCH");

    this->parentDisplayPtr->drawRect(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_SQUELCH_BAR_X_OFFSET, this->yOffset + this->padding, _VOLUME_SQUELCH_WIDGET_BARS_WIDTH, _VOLUME_SQUELCH_WIDGET_BARS_HEIGHT, active ? TEXT_COLOR_ACTIVE : TEXT_COLOR_NOT_ACTIVE);
  }
  if (value > 0)
  {
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_SQUELCH_BAR_X_OFFSET + 1, this->yOffset + this->padding + 1, value, _VOLUME_SQUELCH_WIDGET_BARS_HEIGHT - 2, TEXT_COLOR_SECONDARY);
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_SQUELCH_BAR_X_OFFSET + 1 + value, this->yOffset + this->padding + 1, 100 - value, _VOLUME_SQUELCH_WIDGET_BARS_HEIGHT - 2, TEXT_BACKGROUND_COLOR);
  }
  else
  {
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_SQUELCH_BAR_X_OFFSET + 1, this->yOffset + this->padding + 1, 100, _VOLUME_SQUELCH_WIDGET_BARS_HEIGHT - 2, TEXT_BACKGROUND_COLOR);
  }
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _VOLUME_SQUELCH_WIDGET_LABEL_SQUELCH_VALUE_X_OFFSET, this->yOffset + this->padding);
  this->parentDisplayPtr->printf("%03d", value);
}

void LGFXVolumeSquelchWidget::setMuted(bool force, uint8_t value)
{
}

bool LGFXVolumeSquelchWidget::refresh(bool force)
{
  bool changed = force;
  if (force || (this->oldAFGainValue != this->currentTransceiverStatusPtr->AFGain || this->oldAudioMutedValue != this->currentTransceiverStatusPtr->audioMuted || this->oldActiveVolumeValue != this->currentTransceiverStatusPtr->isVolumeControlActive))
  {
    this->refreshVolume(force, this->currentTransceiverStatusPtr->isVolumeControlActive, this->currentTransceiverStatusPtr->AFGain, this->currentTransceiverStatusPtr->audioMuted);
    this->oldAFGainValue = this->currentTransceiverStatusPtr->AFGain;
    this->oldAudioMutedValue = this->currentTransceiverStatusPtr->audioMuted;
    this->oldActiveVolumeValue = this->currentTransceiverStatusPtr->isVolumeControlActive;
    changed = true;
  }
  if (force || this->oldSquelchValue != this->currentTransceiverStatusPtr->squelch || this->oldActiveSquelchValue != this->currentTransceiverStatusPtr->isSquelchControlActive)
  {
    this->refreshSquelch(force, this->currentTransceiverStatusPtr->isSquelchControlActive, this->currentTransceiverStatusPtr->squelch);
    this->oldSquelchValue = this->currentTransceiverStatusPtr->squelch;
    this->oldActiveSquelchValue = this->currentTransceiverStatusPtr->isSquelchControlActive;
    changed = true;
  }
  return (changed);
}