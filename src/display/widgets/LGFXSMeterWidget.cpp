#include <cstdbool>
#include "LGFXSMeterWidget.hpp"

LGFXSMeterWidget::LGFXSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr) : LGFXTransceiverStatusWidget(displayPtr, width, height, xOffset, yOffset, padding, currentTransceiverStatusPtr)
{
  this->smeter = new SMeter(this->currentTransceiverStatusPtr->signalMeterdBLevel);
  if (displayPtr != nullptr)
  {
    this->dbLabelExpSprite = new lgfx::LGFX_Sprite(displayPtr);
    this->dbLabelExpSprite->setColorDepth(8);
    this->dbLabelExpSprite->createSprite(60, 28);
  }
}

LGFXSMeterWidget::~LGFXSMeterWidget()
{
  if (this->smeter != nullptr)
  {
    delete this->smeter;
    this->smeter = nullptr;
  }
  if (this->dbLabelExpSprite != nullptr)
  {
    delete this->dbLabelExpSprite;
    this->dbLabelExpSprite = nullptr;
  }
}

void LGFXSMeterWidget::refreshLabel(bool force, int8_t dB)
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
    this->dbLabelExpSprite->fillScreen(TFT_BLACK);
    this->dbLabelExpSprite->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND_COLOR);
    this->dbLabelExpSprite->setTextSize(_DIGITAL_SMETER_WIDGET_S_SUB_LABEL_FONT_SIZE);
    this->dbLabelExpSprite->setCursor(0, 0);
    this->dbLabelExpSprite->printf("+%ddB", dB);
    this->dbLabelExpSprite->pushSprite(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_S_LABEL_EXP_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_S_LABEL_Y_OFFSET);
  }
}

bool LGFXSMeterWidget::refresh(bool force)
{
  bool changed = force;
  if (force)
  {
    this->init();
  }
  int dB = this->currentTransceiverStatusPtr->signalMeterdBLevel;
  this->smeter->set(dB);
  int8_t dBSmooth = this->smeter->get(true);
  bool labelUpdateRequired = this->previousDBValue != dB;
  bool smeterUpdateRequired = this->previousDBSmoothedValue != dBSmooth;
  if (force || smeterUpdateRequired || labelUpdateRequired)
  {
    if (smeterUpdateRequired)
    {
      this->update(dBSmooth);
    }
    if (labelUpdateRequired)
    {
      this->refreshLabel(force, dB); // "digital" label always show real (not "smooth")
    }
    this->previousDBValue = dB;
    this->previousDBSmoothedValue = dBSmooth;
    changed = true;
  }
  return (changed);
}