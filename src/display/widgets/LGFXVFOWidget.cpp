#include <cstdbool>
#include "LGFXVFOWidget.hpp"
#include "../../CommonDefines.hpp"

LGFXVFOWidget::LGFXVFOWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, uint8_t index, const TransceiverStatus *currentTransceiverStatusPtr) : LGFXTransceiverStatusWidget(displayPtr, width, height, xOffset, yOffset, padding, currentTransceiverStatusPtr)
{
  if (displayPtr != nullptr)
  {
    if (currentTransceiverStatusPtr != nullptr)
    {
      this->isActive = currentTransceiverStatusPtr->activeVFOIndex == index;
      this->index = index;
      this->refresh(true);
    }
  }
}

LGFXVFOWidget::~LGFXVFOWidget()
{
}

void LGFXVFOWidget::printIndex()
{
  this->parentDisplayPtr->setTextColor(this->isActive ? TEXT_COLOR_ACTIVE : TEXT_COLOR_SECONDARY, TEXT_BACKGROUND_COLOR);
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding, this->yOffset + this->padding);
  this->parentDisplayPtr->setTextSize(_VFO_WIDGET_FONT_SIZE);
  this->parentDisplayPtr->print(this->index == 0 ? VFO_PRIMARY_LABEL : VFO_SECONDARY_LABEL);
}

void LGFXVFOWidget::printFrequency(uint64_t frequency)
{
  char formattedFrequency[16];
  char nformattedFrequency[12];
  // test if this method is more optimized than div operators
  uint8_t resultIndex = 0;
  sprintf(nformattedFrequency, "%012llu", frequency);
  for (uint8_t i = 0; i < 12; ++i)
  {
    formattedFrequency[resultIndex++] = nformattedFrequency[i];
    if ((i + 1) % 3 == 0 && i < 11)
    {
      formattedFrequency[resultIndex++] = '.';
    }
  }
  formattedFrequency[resultIndex] = '\0';
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _VFO_WIDGET_FREQUENCY_X_OFFSET, this->yOffset + this->padding);
  this->parentDisplayPtr->setTextSize(_VFO_WIDGET_FONT_SIZE);
  if (this->isActive)
  {
    uint8_t firstNonZeroIndex = 0;
    while ((formattedFrequency[firstNonZeroIndex] == '0' || formattedFrequency[firstNonZeroIndex] == '.') && firstNonZeroIndex < 12)
    {
      firstNonZeroIndex++;
    }
    this->parentDisplayPtr->setTextColor(TEXT_COLOR_NOT_ACTIVE, TFT_BLACK);
    for (uint8_t i = 0; i < firstNonZeroIndex; i++)
    {
      this->parentDisplayPtr->print(formattedFrequency[i]);
    }
    this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE, TFT_BLACK);
    this->parentDisplayPtr->print(&formattedFrequency[firstNonZeroIndex]);
  }
  else
  {
    this->parentDisplayPtr->setTextColor(TEXT_COLOR_SECONDARY, TFT_BLACK);
    this->parentDisplayPtr->print(formattedFrequency);
  }
  if (frequency >= 1000000000)
  {
    this->parentDisplayPtr->print("GHz");
  }
  else if (frequency >= 1000000)
  {
    this->parentDisplayPtr->print("Mhz");
  }
  else if (frequency >= 1000)
  {
    this->parentDisplayPtr->print("Khz");
  }
  else
  {
    this->parentDisplayPtr->print("Hz ");
  }
}

void LGFXVFOWidget::getModeLabel(TrxVFOMode mode, char *label, size_t labelSize)
{
  switch (mode)
  {
  case TRX_VFO_MD_DSB:
    snprintf(label, labelSize, "%s", "DSB");
    break;
  case TRX_VFO_MD_LSB:
    snprintf(label, labelSize, "%s", "LSB");
    break;
  case TRX_VFO_MD_USB:
    snprintf(label, labelSize, "%s", "USB");
    break;
  case TRX_VFO_MD_CW_U:
    snprintf(label, labelSize, "%s", "CWU"); // CW (upper sideband)
    break;
  case TRX_VFO_MD_FM:
    snprintf(label, labelSize, "%s", "FM ");
    break;
  case TRX_VFO_MD_SAM:
    snprintf(label, labelSize, "%s", "SAM"); // (synchronous AM, includes ECSS)
    break;
  case TRX_VFO_MD_RESERVED:
    snprintf(label, labelSize, "%s", "   "); // reserved / not used
    break;
  case TRX_VFO_MD_CW_L:
    snprintf(label, labelSize, "%s", "CWL"); // CW (lower sideband)
    break;
  case TRX_VFO_MD_WFM:
    snprintf(label, labelSize, "%s", "WFM");
    break;
  case TRX_VFO_MD_BFM:
    snprintf(label, labelSize, "%s", "BFM");
    break;
  default:
    snprintf(label, labelSize, "%s", "???"); // error / invalid value
    break;
  }
}

void LGFXVFOWidget::printMode(TrxVFOMode mode)
{
  this->parentDisplayPtr->setTextColor(this->isActive ? TEXT_COLOR_ACTIVE : TEXT_COLOR_SECONDARY, TEXT_BACKGROUND_COLOR);
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _VFO_WIDGET_MODE_X_OFFSET, this->yOffset + this->padding);
  this->parentDisplayPtr->setTextSize(_VFO_WIDGET_FONT_SIZE);
  char label[4] = {'\0'};
  this->getModeLabel(mode, label, sizeof(label));
  this->parentDisplayPtr->print(label);
}

void LGFXVFOWidget::printFrequencyStep(uint64_t step)
{
  uint16_t x = this->xOffset + this->padding + _VFO_WIDGET_FREQUENCY_STEP_X_OFFSET;
  uint16_t y = (this->yOffset + this->padding) + _VFO_WIDGET_FREQUENCY_STEP_Y_OFFSET;
  // clear previous frequency step
  this->parentDisplayPtr->fillRect(x, y, 15 * (_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN), _VFO_WIDGET_FREQUENCY_STEP_HEIGHT, TFT_BLACK);
  bool hideStep = false;
  if (step > 0)
  {
    switch (step)
    {
    case 1: // 1 hz
      x += ((_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 14);
      break;
    case 10: // 10 hz
      x += ((_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 13);
      break;
    case 100: // 100 hz
      x += ((_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 12);
      break;
    case 1000: // 1 Khz
      x += ((_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 10);
      break;
    case 10000: // 10 Khz
      x += ((_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 9);
      break;
    case 100000: // 100 Khz
      x += ((_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 8);
      break;
    case 1000000: // 1 Mhz
      x += ((_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 6);
      break;
    case 10000000: // 10 Mhz
      x += ((_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 5);
      break;
    case 100000000: // 100 Mhz
      x += ((_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 4);
      break;
    case 1000000000: // 1 Ghz
      x += ((_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 2);
      break;
    case 10000000000: // 10 Ghz
      x += (_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN);
      break;
    case 100000000000: // 100 Ghz
      break;
    default:
      hideStep = true;
      break;
    }
    if (!hideStep)
    {
      this->parentDisplayPtr->fillRect(x, y, _VFO_WIDGET_FREQUENCY_STEP_WIDTH, _VFO_WIDGET_FREQUENCY_STEP_HEIGHT, this->isActive ? TEXT_COLOR_ACTIVE : TEXT_COLOR_SECONDARY);
    }
  }
}

bool LGFXVFOWidget::refresh(bool force)
{
  bool changed = force;
  bool activeStatusChanged = false;
  if (this->isActive != this->currentTransceiverStatusPtr->activeVFOIndex == this->index)
  {
    this->isActive = !this->isActive;
    activeStatusChanged = true;
  }
  if (force || activeStatusChanged)
  {
    this->printIndex();
    changed = true;
  }
  if (force || activeStatusChanged || this->currentTransceiverStatusPtr->VFO[this->index].frequency != this->previousFrequency)
  {
    this->printFrequency(this->currentTransceiverStatusPtr->VFO[this->index].frequency);
    this->previousFrequency = this->currentTransceiverStatusPtr->VFO[this->index].frequency;
    changed = true;
  }
  if (force || activeStatusChanged || this->currentTransceiverStatusPtr->VFO[this->index].mode != this->previousMode)
  {
    this->printMode(this->currentTransceiverStatusPtr->VFO[this->index].mode);
    this->previousMode = this->currentTransceiverStatusPtr->VFO[this->index].mode;
    changed = true;
  }
  if (force || activeStatusChanged || this->currentTransceiverStatusPtr->VFO[this->index].frequencyStep != this->previousFrequencyStep)
  {
    this->printFrequencyStep(this->currentTransceiverStatusPtr->VFO[this->index].frequencyStep);
    this->previousFrequencyStep = this->currentTransceiverStatusPtr->VFO[this->index].frequencyStep;
    changed = true;
  }
  return (changed);
}