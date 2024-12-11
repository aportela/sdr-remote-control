#include <cstdbool>
#include "LGFXSMeterWidget.hpp"

LGFXSMeterWidget::LGFXSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr) : LGFXTransceiverStatusWidget(displayPtr, width, height, xOffset, yOffset, padding, currentTransceiverStatusPtr)
{
  this->smeter = new SMeter(this->currentTransceiverStatusPtr->signalMeterdBLevel);
  if (displayPtr != nullptr)
  {
    this->dbExponentSprite = new lgfx::LGFX_Sprite(displayPtr);
    this->dbExponentSprite->setColorDepth(8);
    this->dbExponentSprite->createSprite(_SMETER_WIDGET_SIGNAL_EXPONENT_SPRITE_WIDTH, _SMETER_WIDGET_SIGNAL_EXPONENT_SPRITE_HEIGHT);
  }
}

LGFXSMeterWidget::~LGFXSMeterWidget()
{
  if (this->smeter != nullptr)
  {
    delete this->smeter;
    this->smeter = nullptr;
  }
  if (this->dbExponentSprite != nullptr)
  {
    delete this->dbExponentSprite;
    this->dbExponentSprite = nullptr;
  }
}

void LGFXSMeterWidget::refreshLabel(bool force, int8_t dB)
{
  this->parentDisplayPtr->setTextSize(_SMETER_WIDGET_SIGNAL_BASE_FONT_SIZE);
  this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND_COLOR);
  if (force)
  {
    this->printAtCursor(_SMETER_WIDGET_SIGNAL_BASE_LABEL_X_OFFSET, _SMETER_WIDGET_SIGNAL_BASE_LABEL_Y_OFFSET, "S");
  }
  this->setCursor(_SMETER_WIDGET_SIGNAL_BASE_VALUE_BASE_X_OFFSET, _SMETER_WIDGET_SIGNAL_BASE_VALUE_BASE_Y_OFFSET);
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
      // no exponent label, only show "dB" label
      this->parentDisplayPtr->print("dB "); // trailing whitespace is required for "full clearing" previous exponent label
    }
  }
  else
  {
    // show exponent label
    // TODO: ignore this fill/clear if previous value also have exponent
    this->dbExponentSprite->fillScreen(DEFAULT_BACKGROUND_COLOR);
    this->dbExponentSprite->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND_COLOR);
    this->dbExponentSprite->setTextSize(_SMETER_WIDGET_SIGNAL_EXPONENT_FONT_SIZE);
    this->dbExponentSprite->setCursor(0, 0);
    this->dbExponentSprite->printf("+%ddB", dB);
    this->dbExponentSprite->pushSprite(this->xOffset + this->padding + _SMETER_WIDGET_SIGNAL_EXPONENT_VALUE_BASE_X_OFFSET, this->yOffset + this->padding + _SMETER_WIDGET_SIGNAL_EXPONENT_VALUE_BASE_Y_OFFSET);
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
    if (force || labelUpdateRequired)
    {
      this->refreshLabel(force, dB); // "digital" label always show real (not "smooth")
    }
    this->previousDBValue = dB;
    this->previousDBSmoothedValue = dBSmooth;
    changed = true;
  }
  return (changed);
}