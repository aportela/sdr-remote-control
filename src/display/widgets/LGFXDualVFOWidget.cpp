#include <cstdbool>
#include "LGFXDualVFOWidget.hpp"
#include "../../DisplayConfiguration.hpp"

#ifdef DISPLAY_LOVYANN_ILI9488_480x320

#include "../ILI9488/ScreenConnectedDefines.hpp"

#elif defined(DISPLAY_LOVYANN_ST7789_240x320)

#include "../ST7789/ScreenConnectedDefines.hpp"

#else

#error NO DISPLAY DEFINED

#endif // DISPLAY_LOVYANN_ILI9488_480x320

#define TEXT_COLOR_ACTIVE TFT_WHITE
#define TEXT_COLOR_SECONDARY 0x528A
#define TEXT_COLOR_NOT_ACTIVE 0x18C3
#define TEXT_BACKGROUND_COLOR TFT_BLACK

LGFXDualVFOWidget::LGFXDualVFOWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, Transceiver *transceiverPtr) : LGFXWidget(displayPtr, width, height, xOffset, yOffset, padding), transceiverPtr(transceiverPtr)
{
  if (displayPtr != nullptr)
  {
  }
}

LGFXDualVFOWidget::~LGFXDualVFOWidget()
{
}

void LGFXDualVFOWidget::refreshVFOIndex(uint8_t number, bool isActive)
{
  this->parentDisplayPtr->setTextColor(isActive ? TEXT_COLOR_ACTIVE : TEXT_COLOR_SECONDARY, TEXT_BACKGROUND_COLOR);
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding, (this->yOffset + this->padding) + (number == 0 ? 0 : DUAL_VFO_WIDGET_SECONDARY_VFO_Y_OFFSET));
  this->parentDisplayPtr->setTextSize(_DUAL_VFO_WIDGET_FONT_SIZE);
  this->parentDisplayPtr->print(number == 0 ? "VFOA" : "VFOB");
}

void LGFXDualVFOWidget::refreshVFOFreq(uint8_t number, bool isActive, uint64_t frequency)
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
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DUAL_VFO_WIDGET_FREQUENCY_X_OFFSET, (this->yOffset + this->padding) + (number == 0 ? 0 : DUAL_VFO_WIDGET_SECONDARY_VFO_Y_OFFSET));
  this->parentDisplayPtr->setTextSize(_DUAL_VFO_WIDGET_FONT_SIZE);
  if (isActive)
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

void LGFXDualVFOWidget::refreshVFOMode(uint8_t number, bool isActive, TrxVFOMode mode)
{
  this->parentDisplayPtr->setTextColor(isActive ? TEXT_COLOR_ACTIVE : TEXT_COLOR_SECONDARY, TEXT_BACKGROUND_COLOR);
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DUAL_VFO_WIDGET_MODE_X_OFFSET, (this->yOffset + this->padding) + (number == 0 ? 0 : DUAL_VFO_WIDGET_SECONDARY_VFO_Y_OFFSET));
  this->parentDisplayPtr->setTextSize(_DUAL_VFO_WIDGET_FONT_SIZE);
  switch (mode)
  {
  case TRX_VFO_MD_DSB:
    this->parentDisplayPtr->print("DSB");
    break;
  case TRX_VFO_MD_LSB:
    this->parentDisplayPtr->print("LSB");
    break;
  case TRX_VFO_MD_USB:
    this->parentDisplayPtr->print("USB");
    break;
  case TRX_VFO_MD_CW_U:
    this->parentDisplayPtr->print("CWU"); // CW (upper sideband)
    break;
  case TRX_VFO_MD_FM:
    this->parentDisplayPtr->print("FM ");
    break;
  case TRX_VFO_MD_SAM:
    this->parentDisplayPtr->print("SAM"); // (synchronous AM, includes ECSS)
    break;
  case TRX_VFO_MD_RESERVED:
    this->parentDisplayPtr->print("  "); // reserved / not used
    break;
  case TRX_VFO_MD_CW_L:
    this->parentDisplayPtr->print("CWL"); // CW (lower sideband)
    break;
  case TRX_VFO_MD_WFM:
    this->parentDisplayPtr->print("WFM");
    break;
  case TRX_VFO_MD_BFM:
    this->parentDisplayPtr->print("BFM");
    break;
  default:
    this->parentDisplayPtr->print("???"); // error / invalid value
    break;
  }
}

void LGFXDualVFOWidget::refreshVFOFrequencyStep(uint8_t number, bool isActive, uint64_t step)
{
  uint16_t x = this->xOffset + this->padding + _DUAL_VFO_WIDGET_FREQUENCY_STEP_X_OFFSET;
  uint16_t y = (this->yOffset + this->padding) + (number == 0 ? _DUAL_VFO_WIDGET_PRIMARY_FREQUENCY_STEP_Y_OFFSET : _DUAL_VFO_WIDGET_SECONDARY_FREQUENCY_STEP_Y_OFFSET);
  // clear previous frequency step
  this->parentDisplayPtr->fillRect(x, y, 15 * (_DUAL_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _DUAL_VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN), _DUAL_VFO_WIDGET_FREQUENCY_STEP_HEIGHT, TFT_BLACK);
  if (step > 0)
  {
    switch (step)
    {
    case 1: // 1 hz
      x += ((_DUAL_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _DUAL_VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 14);
      break;
    case 10: // 10 hz
      x += ((_DUAL_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _DUAL_VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 13);
      break;
    case 100: // 100 hz
      x += ((_DUAL_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _DUAL_VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 12);
      break;
    case 1000: // 1 Khz
      x += ((_DUAL_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _DUAL_VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 10);
      break;
    case 10000: // 10 Khz
      x += ((_DUAL_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _DUAL_VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 9);
      break;
    case 100000: // 100 Khz
      x += ((_DUAL_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _DUAL_VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 8);
      break;
    case 1000000: // 1 Mhz
      x += ((_DUAL_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _DUAL_VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 6);
      break;
    case 10000000: // 10 Mhz
      x += ((_DUAL_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _DUAL_VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 5);
      break;
    case 100000000: // 100 Mhz
      x += ((_DUAL_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _DUAL_VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 4);
      break;
    case 1000000000: // 1 Ghz
      x += ((_DUAL_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _DUAL_VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN) * 2);
      break;
    case 10000000000: // 10 Ghz
      x += (_DUAL_VFO_WIDGET_FREQUENCY_STEP_WIDTH + _DUAL_VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN);
      break;
    case 100000000000: // 100 Ghz
      break;
    default:
      break;
    }
    this->parentDisplayPtr->fillRect(x, y, _DUAL_VFO_WIDGET_FREQUENCY_STEP_WIDTH, _DUAL_VFO_WIDGET_FREQUENCY_STEP_HEIGHT, isActive ? TEXT_COLOR_ACTIVE : TEXT_COLOR_SECONDARY);
  }
}

bool LGFXDualVFOWidget::refresh(bool force)
{
  bool changed = force;
  if (changed || this->transceiverPtr->changed > 0)
  {
    if (force || (this->transceiverPtr->changed & TRX_CFLAG_VFO_INDEX))
    {
      this->refreshVFOIndex(0, this->transceiverPtr->activeVFOIndex == 0);
      this->refreshVFOIndex(1, this->transceiverPtr->activeVFOIndex == 1);
      this->transceiverPtr->changed &= ~TRX_CFLAG_VFO_INDEX;
      changed = true;
    }
    if (force || (this->transceiverPtr->changed & TRX_CFLAG_ACTIVE_VFO_FREQUENCY))
    {
      this->refreshVFOFreq(0, this->transceiverPtr->activeVFOIndex == 0, this->transceiverPtr->VFO[0].frequency);
      this->transceiverPtr->changed &= ~TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
      changed = true;
    }
    if (force || (this->transceiverPtr->changed & TRX_CFLAG_ACTIVE_VFO_MODE))
    {
      this->refreshVFOMode(0, this->transceiverPtr->activeVFOIndex == 0, this->transceiverPtr->VFO[0].mode);
      this->transceiverPtr->changed &= ~TRX_CFLAG_ACTIVE_VFO_MODE;
      changed = true;
    }
    if (force || (this->transceiverPtr->changed & TRX_CFLAG_ACTIVE_VFO_STEP))
    {
      this->refreshVFOFrequencyStep(0, this->transceiverPtr->activeVFOIndex == 0, this->transceiverPtr->VFO[0].customStep);
      this->transceiverPtr->changed &= ~TRX_CFLAG_ACTIVE_VFO_STEP;
      changed = true;
    }
    if (force || (this->transceiverPtr->changed & TRX_CFLAG_SECONDARY_VFO_FREQUENCY))
    {
      this->refreshVFOFreq(1, this->transceiverPtr->activeVFOIndex == 1, this->transceiverPtr->VFO[1].frequency);
      this->transceiverPtr->changed &= ~TRX_CFLAG_SECONDARY_VFO_FREQUENCY;
      changed = true;
    }
    if (force || (this->transceiverPtr->changed & TRX_CFLAG_SECONDARY_VFO_MODE))
    {
      this->refreshVFOMode(1, this->transceiverPtr->activeVFOIndex == 1, this->transceiverPtr->VFO[1].mode);
      this->transceiverPtr->changed &= ~TRX_CFLAG_SECONDARY_VFO_MODE;
      changed = true;
    }
    if (force || (this->transceiverPtr->changed & TRX_CFLAG_SECONDARY_VFO_STEP))
    {
      this->refreshVFOFrequencyStep(1, this->transceiverPtr->activeVFOIndex == 1, this->transceiverPtr->VFO[1].customStep);
      this->transceiverPtr->changed &= ~TRX_CFLAG_SECONDARY_VFO_STEP;
      changed = true;
    }
  }
  return (changed);
}